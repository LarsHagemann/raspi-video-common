#ifndef RASPI_VIDEO_COMMON_LOGGING_SERVICE_HPP
#define RASPI_VIDEO_COMMON_LOGGING_SERVICE_HPP

#include <cstdarg>
#include <memory>
#include <iostream>

enum class ELoggingLevel : uint8_t {
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARNING,
  LEVEL_ERROR,
  LEVEL_FATAL
};

#ifndef RASPI_VIDEO_COMMON_CONFIG_LOGGING_LEVEL
#define RASPI_VIDEO_COMMON_CONFIG_LOGGING_LEVEL ELoggingLevel::LEVEL_WARNING
#endif

class CLoggingServiceStream {
protected:
  std::unique_ptr<CLoggingServiceStream> m_child;
public:
  virtual ~CLoggingServiceStream() = default;

  virtual void write(const char* data, size_t length) = 0;

  void child(std::unique_ptr<CLoggingServiceStream> ch) {
    m_child = std::move(ch);
  }
  void child(CLoggingServiceStream* ch) {
    m_child = std::unique_ptr<CLoggingServiceStream>(ch);
  }
};

class COwningLoggingServiceOStream : public CLoggingServiceStream {
private:
  std::unique_ptr<std::ostream> m_stream;
public:
  COwningLoggingServiceOStream(std::unique_ptr<std::ostream> stream)
    : m_stream(std::move(stream)) {}
  COwningLoggingServiceOStream(std::ostream* stream)
    : m_stream(std::unique_ptr<std::ostream>(stream)) {}

  void write(const char* data, size_t length) override {
    m_stream->write(data, length);
    if (m_child) {
      m_child->write(data, length);
    }
  }
};

class CLoggingServiceOStream : public CLoggingServiceStream {
private:
  std::ostream* m_stream;
public:
  CLoggingServiceOStream(std::ostream* stream)
    : m_stream(stream) {}

  void write(const char* data, size_t length) override {
    m_stream->write(data, length);
    if (m_child) {
      m_child->write(data, length);
    }
  }
};

class CLoggingService {
private:
  std::unique_ptr<CLoggingServiceStream> m_stream;
protected:
  void log(const char* level, const char* message, va_list args);
public:
  explicit CLoggingService(
      std::unique_ptr<CLoggingServiceStream> stream
  ) : m_stream(std::move(stream))
  {}
  explicit CLoggingService(
      CLoggingServiceStream* stream
  ) : m_stream(std::unique_ptr<CLoggingServiceStream>(stream))
  {}
  virtual ~CLoggingService() = default;

  virtual void debug(const char* message, ...) = 0;
  virtual void info(const char* message, ...) = 0;
  virtual void warning(const char* message, ...) = 0;
  virtual void error(const char* message, ...) = 0;
  virtual void fatal(const char* message, ...) = 0;
};

#endif
