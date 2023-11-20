#ifndef RASPI_VIDEO_COMMON_LOGGING_SERVICE_DEV_HPP
#define RASPI_VIDEO_COMMON_LOGGING_SERVICE_DEV_HPP

#include "raspi-video-common/logging-service.hpp"

class CDevLoggingService : public CLoggingService {
private:
  ELoggingLevel m_level;

  void logImpl(
      ELoggingLevel level,
      const char* levelStr, 
      const char* message, 
      va_list args) {
    
    if (static_cast<uint8_t>(m_level) <=
        static_cast<uint8_t>(level)) {
      
      log(levelStr, message, args);
    }
  }
public:
  explicit CDevLoggingService(
      ELoggingLevel level,
      std::unique_ptr<CLoggingServiceStream> stream
  ) : CLoggingService(std::move(stream))
    , m_level(level) {}
  explicit CDevLoggingService(
      ELoggingLevel level,
      CLoggingServiceStream* stream
  ) : CLoggingService(stream)
    , m_level(level) {}

  void debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    logImpl(ELoggingLevel::LEVEL_DEBUG, "DEBUG", message, args);
    va_end(args);
  }
  void info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    logImpl(ELoggingLevel::LEVEL_INFO, "INFO", message, args);
    va_end(args);
  }
  void warning(const char* message, ...) {
    va_list args;
    va_start(args, message);
    logImpl(ELoggingLevel::LEVEL_WARNING, "WARNING", message, args);
    va_end(args);
  }
  void error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    logImpl(ELoggingLevel::LEVEL_ERROR, "ERROR", message, args);
    va_end(args);
  }
  void fatal(const char* message, ...) {
    va_list args;
    va_start(args, message);
    logImpl(ELoggingLevel::LEVEL_FATAL, "FATAL", message, args);
    va_end(args);
  }
};

#endif
