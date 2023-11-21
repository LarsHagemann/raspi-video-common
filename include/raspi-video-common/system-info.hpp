#ifndef RASPI_VIDEO_COMMON_SYSTEM_INFO_HPP
#define RASPI_VIDEO_COMMON_SYSTEM_INFO_HPP

#include <chrono>
#include <fstream>
#include <string>

namespace detail {

using clock_t      = std::chrono::steady_clock;
using time_point_t = clock_t::time_point;
using duration_t   = clock_t::duration;

template<typename T>
struct SCacheItem {
  T            data;
  time_point_t lastFetchTs = time_point_t(std::chrono::milliseconds(0));
};

template<typename T>
class CCache {
private:
  SCacheItem<T>   m_item;
  duration_t      m_cacheTime;
protected:
  virtual void fetch(T* t) = 0;
public:
  CCache() {}
  virtual ~CCache() = default;
  bool valid() const {
    return (clock_t::now() - m_item.lastFetchTs) < m_cacheTime;
  }
  T get() {
    const auto now = clock_t::now();
    const auto elapsed = now - m_item.lastFetchTs;
    if (elapsed >= m_cacheTime) {
      fetch(&m_item.data);
      m_item.lastFetchTs = now;
    }
    return m_item.data;
  }
  void setCacheTime(duration_t duration) {
    m_cacheTime = duration;
  }
};

class CCpuTemperatureCache : public CCache<float> {
private:
  std::ifstream m_cpuTemperatureFile;
  std::string  m_tmpString;
protected:
  void fetch(float* value) {
    m_cpuTemperatureFile.seekg(0, std::ios::beg);
    m_cpuTemperatureFile >> m_tmpString;
    *value = std::strtol(m_tmpString.c_str(), nullptr, 0) / 1000.f;
  }
public:
  CCpuTemperatureCache() {
    m_cpuTemperatureFile.open("/sys/class/thermal/thermal_zone0/temp");
    if (!m_cpuTemperatureFile.is_open()) {
      throw std::runtime_error(
          "Failed to open "
          "/sys/class/thermal/thermal_zone0/temp");
    }
    setCacheTime(std::chrono::milliseconds(500));
  }

};

struct SMemoryInfo {
  uint32_t totalMemory     = 0;
  uint32_t availableMemory = 0;
  uint32_t freeMemory      = 0;
  uint32_t totalSwapMemory = 0;
  uint32_t freeSwapMemory  = 0;
};

class CMemoryInfoCache : public CCache<SMemoryInfo> {
private:
  std::ifstream m_memoryFile;
  std::string m_tmpString;

  const std::string memTotalLexeme = "MemTotal";
  const std::string memFreeLexeme = "MemFree";
  const std::string memAvailableLexeme = "MemAvailable";
  const std::string swapTotalLexeme = "SwapTotal";
  const std::string swapFreeLexeme = "SwapFree";
protected:
  void fetch(SMemoryInfo* info) {
    auto test = [](
        const std::string& line,
        const std::string& lexeme,
        uint32_t& value) {

      if (line.find(lexeme) != std::string::npos) {
        char* endPtr;
        auto nbr = std::strtol(
            line.c_str() + lexeme.length() + 1,
            &endPtr,
            10
        );
        auto unit = std::string(endPtr);
        auto factor = 1;
        if (unit.find("kB") != std::string::npos) {
          factor = 1000;
        }
        else if (unit.find("mB") != std::string::npos) {
          factor = 1000 * 1000;
        }
        else if (unit.find("gB") != std::string::npos) {
          factor = 1000 * 1000 * 1000;
        }

        value = nbr * factor;
      }
    };
    m_memoryFile.seekg(0, std::ios::beg);
    while (std::getline(m_memoryFile, m_tmpString)) {
      test(m_tmpString, memTotalLexeme    , info->totalMemory);
      test(m_tmpString, memFreeLexeme     , info->freeMemory);
      test(m_tmpString, memAvailableLexeme, info->availableMemory);
      test(m_tmpString, swapTotalLexeme   , info->totalSwapMemory);
      test(m_tmpString, swapFreeLexeme    , info->freeSwapMemory);
    }
  }
public:
  CMemoryInfoCache() {
    m_memoryFile.open("/proc/meminfo");
    if (!m_memoryFile.is_open()) {
      throw std::runtime_error(
          "Failed to open "
          "/proc/meminfo");
    }
    setCacheTime(std::chrono::milliseconds(2000));
  }
};

}

struct CSystemInfo {
public:
  detail::CCpuTemperatureCache CpuTemperature;
  detail::CMemoryInfoCache     MemoryInfo;
};

#endif
