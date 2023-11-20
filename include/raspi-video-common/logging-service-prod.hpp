#ifndef RASPI_VIDEO_COMMON_LOGGING_SERVICE_PROD_HPP
#define RASPI_VIDEO_COMMON_LOGGING_SERVICE_PROD_HPP

#include "raspi-video-common/logging-service.hpp"

template<ELoggingLevel TLevel 
  = RASPI_VIDEO_COMMON_CONFIG_LOGGING_LEVEL>
class CProdLoggingService : public CLoggingService {
public:
  constexpr static auto sLoggingLevel 
    = RASPI_VIDEO_COMMON_CONFIG_LOGGING_LEVEL;
public:
  explicit CProdLoggingService(
      std::unique_ptr<CLoggingServiceStream> stream
  ) : CLoggingService(stream) {}
  explicit CProdLoggingService(
      CLoggingServiceStream* stream
  ) : CLoggingService(stream) {}

  void debug(const char* message, ...) {
    if constexpr (
        static_cast<uint8_t>(sLoggingLevel) <= 
        static_cast<uint8_t>(ELoggingLevel::LEVEL_DEBUG)) {
      
      va_list args;
      va_start(args, message);
      log("DEBUG", message, args);
      va_end(args);
    }
  }

  void info(const char* message, ...) {
    if constexpr (
        static_cast<uint8_t>(sLoggingLevel) <= 
        static_cast<uint8_t>(ELoggingLevel::LEVEL_INFO)) {
      
      va_list args;
      va_start(args, message);
      log("INFO", message, args);
      va_end(args);
    }
  }
  void warning(const char* message, ...) {
    if constexpr (
        static_cast<uint8_t>(sLoggingLevel) <= 
        static_cast<uint8_t>(ELoggingLevel::LEVEL_WARNING)) {
      
      va_list args;
      va_start(args, message);
      log("WARNING", message, args);
      va_end(args);
    }
  }
  void error(const char* message, ...) {
    if constexpr (
        static_cast<uint8_t>(sLoggingLevel) <= 
        static_cast<uint8_t>(ELoggingLevel::LEVEL_ERROR)) {
      
      va_list args;
      va_start(args, message);
      log("ERROR", message, args);
      va_end(args);
    }
  }
  void fatal(const char* message, ...) {
    if constexpr (
        static_cast<uint8_t>(sLoggingLevel) <= 
        static_cast<uint8_t>(ELoggingLevel::LEVEL_FATAL)) {
      
      va_list args;
      va_start(args, message);
      log("FATAL", message, args);
      va_end(args);
    }
  }

};



#endif
