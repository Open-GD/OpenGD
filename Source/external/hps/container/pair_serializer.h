#ifndef JL2922_HPS_PAIR_SERIALIZER_H
#define JL2922_HPS_PAIR_SERIALIZER_H

#include <iostream>
#include <utility>
#include "../basic_type/uint_serializer.h"
#include "../serializer.h"

namespace hps {

template <class T1, class T2, class B>
class Serializer<std::pair<T1, T2>, B> {
 public:
  static void serialize(const std::pair<T1, T2>& container, B& ob) {
    Serializer<T1, B>::serialize(container.first, ob);
    Serializer<T2, B>::serialize(container.second, ob);
  }

  static void parse(std::pair<T1, T2>& container, B& ib) {
    Serializer<T1, B>::parse(container.first, ib);
    Serializer<T2, B>::parse(container.second, ib);
  }
};

}  // namespace hps
#endif
