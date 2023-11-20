#include "raspi-video-common/logging-service.hpp"
#include "raspi-video-common/utils.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

void CLoggingService::log(const char* level, const char* message, va_list args) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream sstr;
  sstr << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
  auto formattedMessage = utils::vformat(message, args);
  auto msg = utils::format(
      "[%s %.8s]: %s\n",
      sstr.str().c_str(),
      level,
      formattedMessage.c_str());

  std::scoped_lock lock{ m_mutex };
  m_stream->write(msg.data(), msg.length() - 1);
}


