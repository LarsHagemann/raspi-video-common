set(
  RASPI_VIDEO_COMMON_LIB_SOURCE

  lib.cpp
)

add_library(
  raspi-video-common-lib

  ${RASPI_VIDEO_COMMON_LIB_SOURCE}
)
target_include_directories(
  raspi-video-common-lib

  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../include/
)
