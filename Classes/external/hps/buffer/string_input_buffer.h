#ifndef JL2922_HPS_STRING_INPUT_BUFFER_H
#define JL2922_HPS_STRING_INPUT_BUFFER_H

#include <string>
#include "../serializer.h"

namespace hps {

class StringInputBuffer {
 public:
  StringInputBuffer(const std::string& str) : str(&str) { pos = 0; }

  void read(char* content, size_t length) {
    str->copy(content, length, pos);
    pos += length;
  }

  char read_char() {
    const char ch = (*str)[pos];
    pos++;
    return ch;
  }

  template <class T>
  StringInputBuffer& operator>>(T& t) {
    Serializer<T, StringInputBuffer>::parse(t, *this);
    return *this;
  }

 private:
  const std::string* const str;

  size_t pos;
};

}  // namespace hps
#endif
