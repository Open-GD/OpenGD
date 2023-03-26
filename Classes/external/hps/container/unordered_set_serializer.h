#ifndef JL2922_HPS_UNORDERED_SET_SERIALIZER_H
#define JL2922_HPS_UNORDERED_SET_SERIALIZER_H

#include <iostream>
#include <unordered_set>
#include "../basic_type/uint_serializer.h"
#include "../serializer.h"

namespace hps {

template <class T, class B>
class Serializer<std::unordered_set<T>, B> {
 public:
  static void serialize(const std::unordered_set<T>& container, B& ob) {
    Serializer<size_t, B>::serialize(container.size(), ob);
    for (const T& elem : container) {
      Serializer<T, B>::serialize(elem, ob);
    }
  }

  static void parse(std::unordered_set<T>& container, B& ib) {
    size_t n_elems;
    Serializer<size_t, B>::parse(n_elems, ib);
    container.clear();
    container.reserve(n_elems);
    T elem;
    for (size_t i = 0; i < n_elems; i++) {
      Serializer<T, B>::parse(elem, ib);
      container.insert(std::move(elem));
    }
  }
};

}  // namespace hps
#endif
