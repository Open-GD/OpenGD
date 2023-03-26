#ifndef JL2922_HPS_SERIALIZER_H
#define JL2922_HPS_SERIALIZER_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace hps {

template <class T, class B, class Enable = void>
class Serializer {
 public:
  static void serialize(const T& t, B& buf) { t.serialize(buf); }

  static void parse(T& t, B& buf) { t.parse(buf); }
};

}  // namespace hps
#endif
