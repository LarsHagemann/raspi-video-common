#include "raspi-video-common/multicast-client.hpp"
#include "raspi-video-common/utils.hpp"

#include <unistd.h>

CMulticastClient::CMulticastClient(
      const CIpAddress& groupIp,
      uint16_t port) {
  m_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (m_socket < 0) {
    throw std::runtime_error("CMulticastClient: socket()");
  }

  int reuse = 1;
  if (setsockopt(
        m_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse,
        sizeof(reuse)) < 0) {

    throw std::runtime_error("CMulticastClient: setsockopt(REUSE)");
  }

  utils::ZeroMemory(m_addr);
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  m_addr.sin_port = htons(port);
   
  if (bind(m_socket, (struct sockaddr*)&m_addr, sizeof(sockaddr_in)) < 0) {
    throw std::runtime_error("CMulticastClient: bind()");
  }

  utils::ZeroMemory(m_mcRequest);
  m_mcRequest.imr_multiaddr = groupIp.get<struct in_addr>();
  m_mcRequest.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(
        m_socket,
        IPPROTO_IP,
        IP_ADD_MEMBERSHIP,
        (struct ip_mreq*)&m_mcRequest,
        sizeof(ip_mreq)) < 0) {

    throw std::runtime_error("CMulticastClient: setsockopt(ADD)");
  }
}

CMulticastClient::~CMulticastClient() {
  setsockopt(
      m_socket,
      IPPROTO_IP,
      IP_DROP_MEMBERSHIP,
      (struct ip_mreq*)&m_mcRequest,
      sizeof(ip_mreq));
  close(m_socket);
}



std::optional<std::vector<int8_t>> CMulticastClient::receive() {
  const auto result = receive(
      (char*)m_buffer.data(),
      s_BufferSize
  );
  if (result >= 0) {
    return std::vector<int8_t>{
      m_buffer.begin(),
      m_buffer.begin() + result
    };
  }
  
  return std::nullopt;
}

int CMulticastClient::receive(char* data, size_t length) {
  auto len = static_cast<unsigned int>(sizeof(sockaddr_in));
  return recvfrom(
      m_socket,
      data,
      length,
      0,
      (struct sockaddr*)&m_addr,
      &len);
}

