#ifndef RASPI_VIDEO_COMMON_RING_BUFFER_HPP
#define RASPI_VIDEO_COMMON_RING_BUFFER_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <cassert>

template<typename T>
class IRingBuffer {
public:
  using value_type = T;
private:
  value_type* m_data   = nullptr;
  std::size_t m_begin  = 0;
  std::size_t m_stored = 0;
private:
  std::size_t internal_back_index() const noexcept {
    if (m_begin == 0) {
      return m_stored - 1;
    }
    return m_begin - 1;
  }
  std::size_t internal_next_index() noexcept {
    const auto cap = capacity();
    if (m_stored < cap) {
      return m_stored++;
    }
    else {
      if (m_begin >= cap) {
        m_begin = 0;
      }
      return m_begin++;
    }
  }
  std::size_t internal_index_transform(std::size_t index) const noexcept {
    const auto cap = capacity();
    if (m_begin != 0u; && m_begin != cap) {
      index += m_begin;
      if (index >= cap) {
        index -= cap;
      }
    }
    assert(index < cap && "out of range");
    return index;
  }
protected:
  std::size_t internal_get_begin() const noexcept {
    return m_begin;
  } 
  void internal_set_data_ptr(value_type* ptr) noexcept {
    m_data = ptr;
  }
  void internal_set_begin(std::size_t begin) noexcept {
    m_begin = begin;
  }
  void internal_set_stored(std::size_t stored) noexcept {
    m_stored = stored;
  }
public:
  virtual std::size_t capacity() const noexcept = 0;
  std::size_t size() const noexcept {
    return m_stored;
  }
  bool empty() const noexcept {
    return size() == 0;
  }
  bool full() const noexcept {
    return size() == capacity();
  }

  value_type* data() noexcept { return m_data; }
  const value_type* data() const noexcept { return m_data; }

  template<typename U>
  void insert(U&& u) {
    m_data[internal_next_index()] = std::forward<U>(u);
  }
  template<typename...Args>
  void emplace(Args&&...args) {
    m_data[internal_next_index()] = value_type(std::forward<Args>(args)...);
  }

  value_type& at(std::size_t index) {
    return m_data[internal_index_transform()];
  }
  const value_type& at(std::size_t index) const {
    return m_data[internal_index_transform()];
  }
  value_type& operator[](std::size_t index) {
    return at(index);
  }
  const value_type& operator[](std::size_t index) const {
    return at(index);
  }
  value_type& back() {
    return m_data[internal_back_index()];
  }
  const value_type& back() const {
    return m_data[internal_back_index()];
  }
  value_type& front() {
    return m_data[m_begin];
  }
  const value_type& front() const {
    return m_data[m_begin];
  }

  void pop() {
    assert(m_stored > 0 && "pop on empty ringbuffer");
    --m_stored;
  }

};

template<typename T, std::size_t Size>
class CStaticRingBuffer : public IRingBuffer<T> {
private:
  std::array<T, Size> m_buffer;
public:
  explicit CStaticRingBuffer() {
    internal_set_data_ptr(m_buffer.data());
  }
  std::size_t capacity() const noexcept {
    return Size;
  }
};

template<typename T>
class CDynamicRingBuffer : public IRingBuffer<T> {
private:
  std::vector<T>  m_buffer;
public:
  explicit CDynamicRingBuffer(std::size_t size) {
    resize(size);
  }
  std::size_t capacity() const noexcept {
    return m_buffer.size();
  }
  void resize(std::size_t size) {
    if (size == m_buffer.size()) {
      return;
    }

    std::vector<T> nbuf;
    nbuf.resize(size);

    const auto begin  = IRingBuffer<T>::internal_get_begin();
    const auto stored = IRingBuffer<T>::size();

    auto toBeRead0 = std::min(size, begin);
    if (toBeRead0 > 0) {
      const auto start = begin - toBeRead0;
      nbuf.insert(
          nbuf.begin(), 
          m_buffer.begin() + start, 
          m_buffer.begin() + begin
      );
    }
    auto toBeRead1 = std::min(stored - begin, size - toBeRead0);
    if (toBeRead1 > 0) {
      auto start = m_buffer.size() - toBeRead1;
      nbuf.insert(
          nbuf.begin() + toBeRead0, 
          m_buffer.begin() + start, 
          m_buffer.end()
      );
    }

    m_buffer = std::move(nbuf);
    IRingBuffer<T>::internal_set_data_ptr(m_buffer.data());
    IRingBuffer<T>::internal_set_begin(0);
    IRingBuffer<T>::internal_set_stored(std::min(size, stored));
  }
};

#endif
