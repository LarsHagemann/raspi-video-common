#include <catch2/catch_all.hpp>
#include "raspi-video-common/ip-address.hpp"

TEST_CASE("ip-address class works", "[sample]") {
  const auto ipStr = "127.3.2.1";
  const auto ipUInt32 = 0xAABBCCDD;

  REQUIRE(CIpAddress(ipStr).get<std::string>() == ipStr);
  REQUIRE(CIpAddress(ipUInt32).get<uint32_t>() == ipUInt32);
  REQUIRE(CIpAddress(ipUInt32).get<std::string>() == "170.187.204.221");
  REQUIRE(CIpAddress(170, 187, 204, 221).get<uint32_t>() == ipUInt32);
  REQUIRE(CIpAddress(127, 3, 2, 1).get<std::string>() == ipStr);
}

