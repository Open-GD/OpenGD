#ifndef JL2922_HPS_STREAM_INPUT_BUFFER_H
#define JL2922_HPS_STREAM_INPUT_BUFFER_H

#include <cstring>
#include <iostream>
#include "../serializer.h"

namespace hps {

constexpr size_t STREAM_INPUT_BUFFER_SIZE = 1 << 16;

class StreamInputBuffer {
 public:
  StreamInputBuffer(std::istream& stream) : stream(&stream) {
    stream.seekg(0, stream.beg);
    load();
  }

  void read(char* content, size_t length) {
    if (pos + length > STREAM_INPUT_BUFFER_SIZE) {
      const size_t n_avail = STREAM_INPUT_BUFFER_SIZE - pos;
      read_core(content, n_avail);
      length -= n_avail;
      content += n_avail;
      if (length > STREAM_INPUT_BUFFER_SIZE) {
        stream->read(content, length);
        load();
        return;
      }
      load();
    }
    read_core(content, length);
  }

  char read_char() {
    if (pos == STREAM_INPUT_BUFFER_SIZE) {
      load();
    }
    const char ch = buffer[pos];
    pos++;
    return ch;
  }

  template <class T>
  StreamInputBuffer& operator>>(T& t) {
    Serializer<T, StreamInputBuffer>::parse(t, *this);
    return *this;
  }
  
  //usefull to debug and profile the file size
  inline size_t tellg() const {
      return (size_t)stream->tellg() - (size_t)STREAM_INPUT_BUFFER_SIZE + (size_t)pos;
  }

 private:
  std::istream* const stream;

  char buffer[STREAM_INPUT_BUFFER_SIZE];

  size_t pos;

  void read_core(char* content, const size_t length) {
    memcpy(content, buffer + pos, length);
    pos += length;
  }

  void load() {
    stream->read(buffer, STREAM_INPUT_BUFFER_SIZE);
    pos = 0;
  }
};

}  // namespace hps
#endif
