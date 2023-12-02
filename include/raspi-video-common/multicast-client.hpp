#ifndef RASPI_VIDEO_DATALAKE_MULTICAST_CLIENT_HPP
#define RASPI_VIDEO_DATALAKE_MULTICAST_CLIENT_HPP

#include <optional>
#include <array>
#include <vector>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "raspi-video-common/ip-address.hpp"

class CMulticastClient {
public:
  static constexpr size_t max_packet_size = 65536;
  static constexpr size_t s_BufferSize = 256;
private:
  std::array<int8_t, s_BufferSize>  m_buffer;
  int                                m_socket;
  struct ip_mreq                     m_mcRequest;
  struct sockaddr_in                 m_addr;
public:
  explicit CMulticastClient(
      const CIpAddress& groupIp,
      uint16_t port
  );
  ~CMulticastClient();

  std::optional<std::vector<int8_t>> receive();
  int receive(char* data, size_t length);
};

#endif
