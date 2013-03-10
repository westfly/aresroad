#ifndef WESTFLY_CIRCLE_BUFFER_H_
#define WESTFLY_CIRCLE_BUFFER_H_
#include "types.h"
#include "boost/scoped_array.hpp"
#include <string>
#include <glog/logging.h>
#include <cstring>
// #include <memory>
namespace westfly {
template <int Size=1024>
class CircleBuffer {
 public:
  CircleBuffer(const int size = Size)
  :capacity_(size), write_index_(0),
  read_index_(0), length_(0) {
    buffer_.reset(new char[size + 1]);
  }
  ~CircleBuffer() {
  }
  inline size_t GetSize() {
    return length_;
  }
  size_t WriteLine(const char* buf) {
    size_t len = strlen(buf);
    return Write(buf, len);
  }
  size_t Write(const std::string& str) {
    size_t len = str.size();
    return Write(str.c_str(), len);
  }
  size_t Write(const char* buf, size_t len) {
    // buffer is writable
    if (len > (capacity_- length_)) {
      VLOG(1)<< "no engue space";
      return 0;
    }
    char* ptr = buffer_.get();
    strncpy(ptr + write_index_, buf, len);
    length_ += len;
    // cycle
    write_index_ = (write_index_ + len )% capacity_;
    return len;
  }
  size_t Read(char*buf, size_t len) {
    if (!length_) {
      return 0;
    }
    char* ptr = strncpy(buf, buffer_.get(), len);
    *ptr = '\0'; // terminator
    size_t slen = ptr - buf;
    assert(slen < len);
    read_index_ = (read_index_ + slen) % capacity_;
    return slen;
  }
  size_t ReadLine(char* buf, size_t len) {
    size_t slen = strnlen(buffer_.get()+read_index_, length_);
    return Read(buf, slen);
  }
  size_t ReadAll(char* buf, size_t len) {
    // buffer is readable
    if (!length_ ||(len < length_)) {
      // empty or 
      return 0;
    }
    strncpy(buf, buffer_.get(), length_);
    size_t slen = length_;
    Reset();
    return slen;
  }
 private:
  inline void Reset() {
    length_ = 0;
    write_index_ = 0;
    read_index_ = 0;
  }
 private:
  // char *buffer_;
  boost::scoped_array<char > buffer_;
  uint32_t capacity_;
  uint32_t write_index_;
  uint32_t read_index_;
  volatile uint32_t length_;
 private:
  DISALLOW_COPY_AND_ASSIGN(CircleBuffer);
};
}  // westfly
typedef westfly::CircleBuffer<1024> SmallBuffer;
typedef westfly::CircleBuffer<100*1024> LargeBuffer;
#endif
