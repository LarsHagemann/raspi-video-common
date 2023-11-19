#include "raspi-video-common/ip-address.hpp"
#include "raspi-video-common/utils.hpp"

#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

CIpAddress::CIpAddress(uint32_t ip)
  : m_addr(htonl(ip))
{}

CIpAddress::CIpAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
  : CIpAddress(
      static_cast<uint32_t>((a << 24) & 0xFF) |
      static_cast<uint32_t>((b << 16) & 0xFF) |
      static_cast<uint32_t>((c <<  8) & 0xFF) |
      static_cast<uint32_t>((d <<  0) & 0xFF))
{}

CIpAddress::CIpAddress(const char* ip) {
  auto addr = utils::ZeroMemory<struct in_addr>();
  if (!inet_aton(ip, &addr)) {
    throw std::runtime_error("Invalid ip address");
  }
  m_addr = addr.s_addr;
}

CIpAddress::CIpAddress(const std::string& ip) 
  : CIpAddress(ip.c_str())
{}

uint32_t CIpAddress::toUInt32() const noexcept {
  return m_addr;
}

SIP_4UInt8 CIpAddress::toSIP_4UInt8() const noexcept {
  return SIP_4UInt8{
    .a = { static_cast<uint8_t>((m_addr >> 24) & 0xFF) },
    .b = { static_cast<uint8_t>((m_addr >> 16) & 0xFF) },
    .c = { static_cast<uint8_t>((m_addr >>  8) & 0xFF) },
    .d = { static_cast<uint8_t>((m_addr >>  0) & 0xFF) }
  };
}

std::string CIpAddress::toString() const noexcept {
  auto ui8 = toSIP_4UInt8();
  char ipStr[16];
  auto written = sprintf(ipStr, "%d.%d.%d.%d", ui8.a, ui8.b, ui8.c, ui8.d);
  return std::string{ ipStr, static_cast<size_t>(written) };
}

struct in_addr CIpAddress::toInAddr() const noexcept {
  auto addr = utils::ZeroMemory<struct in_addr>();
  addr.s_addr = m_addr;
  return addr;
}

