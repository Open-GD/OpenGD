#ifndef JL2922_HPS_CHAR_ARRAY_INPUT_BUFFER_H
#define JL2922_HPS_CHAR_ARRAY_INPUT_BUFFER_H

#include <cstring>
#include "../serializer.h"

namespace hps {

class CharArrayInputBuffer {
 public:
  CharArrayInputBuffer(const char* arr) : arr(arr) { pos = 0; }

  void read(char* content, size_t length) {
    strncpy(content, &arr[pos], length);
    pos += length;
  }

  char read_char() {
    const char ch = arr[pos];
    pos++;
    return ch;
  }

  template <class T>
  CharArrayInputBuffer& operator>>(T& t) {
    Serializer<T, CharArrayInputBuffer>::parse(t, *this);
    return *this;
  }

 private:
  const char* const arr;

  size_t pos;
};

}  // namespace hps
#endif
