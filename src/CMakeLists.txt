set(
  RASPI_VIDEO_COMMON_LIB_SOURCE

  ${CMAKE_CURRENT_LIST_DIR}/ip-address.cpp
  ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
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

