#ifndef RASPI_VIDEO_COMMON_UTILS_HPP
#define RASPI_VIDEO_COMMON_UTILS_HPP

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <string>

namespace utils {
  
  template<
    class T,
    size_t TSize = sizeof(T)>
  void ZeroMemory(T& t) {
    memset(&t, 0, TSize);
  }

  template<
    class T,
    size_t TSize = sizeof(T)>
  T ZeroMemory() {
    T t;
    ZeroMemory(t);
    return t;
  }

  template<typename T,
           typename U>
  bool isBitSet(T bitset, U bit) {
    return (static_cast<uint64_t>(bitset) &
            static_cast<uint64_t>(bit)) == static_cast<uint64_t>(bit);
  }

  struct SSegmentData {
    const void* begin;
    size_t      offset;
    size_t      length;
  };

  extern std::vector<SSegmentData> segment(
      const void* data, 
      size_t length, 
      size_t segmentSize);

  struct SHexPrintConfig {
    const uint8_t stride = 16; // how many bytes in a row
    const uint8_t lineWidth = 4; // width of the line number indicator
    const bool    enableLineNumbers = true; // Enable/Disable line numbers
    const bool    enableHeader = false; // Enable/Disable header
  };

  extern void hexPrint(
      const uint8_t* data, 
      size_t len, 
      const SHexPrintConfig& config = SHexPrintConfig());
  extern void hexPrint(
      const std::vector<uint8_t>& data,
      const SHexPrintConfig& config = SHexPrintConfig());

  extern std::string format(const char* format, ...);
  extern std::string vformat(const char* format, va_list args);

}

#endif
