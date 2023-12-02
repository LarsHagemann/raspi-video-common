#ifndef RASPI_VIDEO_COMMON_TIMER_HPP
#define RASPI_VIDEO_COMMON_TIMER_HPP

#include <chrono>
#include <string>

template<typename TClock = std::chrono::steady_clock>
class CTimer {
public:
  using clock_t = TClock;
  using time_point_t = clock_t::time_point;
  using duration_t = clock_t::duration;
private:
  time_point_t m_begin;
  time_point_t m_end;

  duration_t   m_elapsed;
  std::string  m_title;
public:
  CTimer(const std::string& title = "timer") {
    start(title);
  }
  void start(const std::string& title = "timer") {
    m_begin = clock_t::now();
    m_title = title;
  }
  void end() {
    m_end = clock_t::now();
    m_elapsed = m_end - m_begin;
    printf(
        "%s took %zd milliseconds\n", 
        m_title.c_str(),
        getElapsed().count());
  }
  template<typename TDuration = std::chrono::milliseconds>
  TDuration getElapsed() const {
    return std::chrono::duration_cast<TDuration>(m_elapsed);
  }
};

#endif
