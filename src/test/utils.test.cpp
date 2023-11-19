#include <catch2/catch_all.hpp>
#include "raspi-video-common/utils.hpp"

TEST_CASE("utils::ZeroMemory", "[common][utils]") {
    REQUIRE(utils::ZeroMemory<uint8_t>() == 0);
    REQUIRE(utils::ZeroMemory<uint16_t>() == 0);
    REQUIRE(utils::ZeroMemory<uint32_t>() == 0);
    REQUIRE(utils::ZeroMemory<uint64_t>() == 0);
}

