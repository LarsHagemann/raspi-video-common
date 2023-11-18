#ifndef RASPI_VIDEO_COMMON_UTILS_HPP
#define RASPI_VIDEO_COMMON_UTILS_HPP

#include <cstdint>
#include <cstring>
#include <vector>

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

}

#endif
