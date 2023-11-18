#ifndef RASPI_VIDEO_COMMON_IP_ADDRESS_HPP
#define RASPI_VIDEO_COMMON_IP_ADDRESS_HPP

#include <string>
#include <cstdint>

#include <sys/socket.h>

struct SIP_4UInt8 {
  uint8_t a{ 0 };
  uint8_t b{ 0 };
  uint8_t c{ 0 };
  uint8_t d{ 0 };
};

class CIpAddress {
private:
  uint32_t m_addr;
public:
  explicit CIpAddress(const char* ip);
  explicit CIpAddress(const std::string& ip);
  explicit CIpAddress(uint32_t ip);
  explicit CIpAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

  uint32_t toUInt32() const noexcept;
  SIP_4UInt8 toSIP_4UInt8() const noexcept;
  std::string toString() const noexcept;
  struct in_addr toInAddr() const noexcept;

  template<typename T>
  [[nodiscard]] T get() const noexcept {
    static_assert(
        std::is_same_v<T, uint32_t> ||
        std::is_same_v<T, SIP_4UInt8> ||
        std::is_same_v<T, struct in_addr> ||
        std::is_same_v<T, std::string>,
        "Cannot convert <CIpAddress> to requested type");

    if constexpr (std::is_same_v<T, uint32_t>) {
      return toUInt32();
    }
    if constexpr (std::is_same_v<T, SIP_4UInt8>) {
      return toSIP_4UInt8();
    }
    if constexpr (std::is_same_v<T, struct in_addr>) {
      return toInAddr();
    }
    if constexpr (std::is_same_v<T, std::string>) {
      return toString();
    }
  }
};

#endif
