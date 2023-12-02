set(
  RASPI_VIDEO_COMMON_LIB_SOURCE

  ${CMAKE_CURRENT_LIST_DIR}/ip-address.cpp
  ${CMAKE_CURRENT_LIST_DIR}/logging-service.cpp
  ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/multicast-client.cpp
  ${CMAKE_CURRENT_LIST_DIR}/streaming-image-builder.cpp
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


add_executable(
    raspi-video-common-test

    test/ip-address.test.cpp
    test/utils.test.cpp
)
target_link_libraries(
    raspi-video-common-test

    PRIVATE
    raspi-video-common-lib
    Catch2::Catch2WithMain
)
target_include_directories(
    raspi-video-common-test

    PRIVATE
    Catch2::Catch2WithMain
)

add_test(
    NAME raspi-video-common-test
    COMMAND raspi-video-common-test --reporter "JUnit::out=../test-reports/sample-tests.xml" --reporter "console::out=-::colour-mode=ansi"
)

