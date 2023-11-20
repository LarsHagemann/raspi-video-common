#include "raspi-video-common/utils.hpp"

#include <cstdio>
#include <cctype>
#include <pthread.h>

std::vector<utils::SSegmentData> utils::segment(
    const void* data,
    size_t      length,
    size_t      segmentSize
    ) {
  const auto numSegments = length / segmentSize + 1;
  std::vector<utils::SSegmentData> result;
  result.reserve(numSegments);

  for (auto i = 0u; i < numSegments; ++i) {
    const auto len = i == numSegments - 1
      ? length - (i * segmentSize)
      : segmentSize;
    const auto offset = i * segmentSize;
    const auto begin = (const void*)((const char*)data + offset);
    result.emplace_back(utils::SSegmentData{ begin, offset, len });
  }

  return result;
}

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

std::string utils::format(const char* format, ...) {
  va_list args;
  va_start(args, format);
  auto result = utils::vformat(format, args);
  va_end(args);
  return result;
}

std::string utils::vformat(const char* format, va_list args) {
  char c = '\0';
  auto len = vsnprintf(&c, 1, format, args);
  std::string str;
  str.resize(len + 1);
  vsnprintf(str.data(), len + 1, format, args);
  return str;
}

void utils::setThreadName(
    std::thread& thread,
    const std::string& name) {
  
  auto handle = thread.native_handle();
  setThreadName(
      &handle,
      name
  );
}

void utils::setThreadName(
    std::jthread& thread,
    const std::string& name) {

  auto handle = thread.native_handle();

  setThreadName(
      &handle,
      name
  );
}

void utils::setThreadName(
    pthread_t* handle,
    const std::string& name) {
  
  pthread_setname_np(*handle, name.c_str());
}

