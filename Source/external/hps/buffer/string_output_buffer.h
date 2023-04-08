#ifndef JL2922_HPS_STRING_OUTPUT_BUFFER_H
#define JL2922_HPS_STRING_OUTPUT_BUFFER_H

#include <string>
#include "../serializer.h"
#include <cstring>

namespace hps {

constexpr size_t STRING_OUTPUT_BUFFER_SIZE = 1 << 10;

class StringOutputBuffer {
 public:
  StringOutputBuffer(std::string& str) : str(&str) { pos = 0; }

  void write(const char* content, size_t length) {
    if (pos + length > STRING_OUTPUT_BUFFER_SIZE) {
      const size_t n_avail = STRING_OUTPUT_BUFFER_SIZE - pos;
      write_core(content, n_avail);
      length -= n_avail;
      content += n_avail;
      flush();
      if (length > STRING_OUTPUT_BUFFER_SIZE) {
        str->append(content, length);
        return;
      }
    }
    write_core(content, length);
  }

  void write_char(const char ch) {
    if (pos == STRING_OUTPUT_BUFFER_SIZE) {
      flush();
    }
    buffer[pos] = ch;
    pos++;
  }

  void flush() {
    str->append(buffer, pos);
    pos = 0;
  }

  template <class T>
  StringOutputBuffer& operator<<(const T& t) {
    Serializer<T, StringOutputBuffer>::serialize(t, *this);
    return *this;
  }

 private:
  std::string* const str;

  char buffer[STRING_OUTPUT_BUFFER_SIZE];

  int pos;

  void write_core(const char* content, const size_t length) {
    memcpy(buffer + pos, content, length);
    pos += length;
  }
};

}  // namespace hps
#endif
