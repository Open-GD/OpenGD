#ifndef JL2922_HPS_FLOAT_SERIALIZER_H
#define JL2922_HPS_FLOAT_SERIALIZER_H

#include <iostream>
#include <type_traits>
#include "../serializer.h"

namespace hps {

template <class T, class B>
class Serializer<T, B, typename std::enable_if<std::is_floating_point<T>::value, void>::type> {
 public:
  static void serialize(const T& num, B& ob) {
    const char* num_ptr = reinterpret_cast<const char*>(&num);
    ob.write(num_ptr, sizeof(num));
  }

  static void parse(T& num, B& ib) {
    char* num_ptr = reinterpret_cast<char*>(&num);
    ib.read(num_ptr, sizeof(num));
  }
};

}  // namespace hps
#endif
