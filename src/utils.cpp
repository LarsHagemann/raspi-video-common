#include "raspi-video-common/utils.hpp"

#include <cstdio>
#include <cctype>

void utils::hexPrint(
    const std::vector<uint8_t>& data,
    const SHexPrintConfig& config) {
  utils::hexPrint(data.data(), data.size(), config);
}

void utils::hexPrint(
    const uint8_t* data, 
    size_t len,
    const SHexPrintConfig& config) {

  if (config.enableHeader) {
    printf("%*c | ", config.lineWidth, ' ');
    for (auto i = 0u; i < config.stride; ++i) {
      printf("+%1X ", i);
    }
    printf("\n");
  }

  for (auto i = 0u; i < len; i += config.stride) {
    if (config.enableLineNumbers) {
      printf("%0*X | ", config.lineWidth, i);
    }
    for (auto j = i; j < i + config.stride && j < len; ++j) {
      printf("%02X ", data[j]);
    }
    printf("  ");
    for (auto j = i; j < i + config.stride && j < len; ++j) {
      if (std::isalnum(static_cast<unsigned int>(data[j]))) {
        printf("%c", data[j]);
      }
      else {
        printf(".");
      }
    }

    printf("\n");
  }

}
