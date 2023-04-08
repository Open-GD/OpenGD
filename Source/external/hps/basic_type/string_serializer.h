#ifndef JL2922_HPS_STRING_SERIALIZER_H
#define JL2922_HPS_STRING_SERIALIZER_H

#include <iostream>
#include <string>
#include "../serializer.h"
#include "uint_serializer.h"

namespace hps {

template <class B>
class Serializer<std::string, B> {
 public:
  static void serialize(const std::string& str, B& ob) {
    const size_t n_bytes = str.size();
    Serializer<size_t, B>::serialize(n_bytes, ob);
    ob.write(str.data(), n_bytes);
  }

  static void parse(std::string& str, B& ib) {
    size_t n_bytes;
    Serializer<size_t, B>::parse(n_bytes, ib);
    str.resize(n_bytes);
    ib.read(&str[0], n_bytes);
  }
};

}  // namespace hps
#endif
