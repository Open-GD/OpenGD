#ifndef JL2922_HPS_ARRAY_SERIALIZER_H
#define JL2922_HPS_ARRAY_SERIALIZER_H

#include <array>
#include <iostream>
#include <type_traits>
#include "../serializer.h"
#include "bit_filters.h"

namespace hps {

template <class T, class B, size_t N>
class Serializer<
    std::array<T, N>,
    B,
    typename std::enable_if<!std::is_floating_point<T>::value, void>::type> {
 public:
  static void serialize(const std::array<T, N>& container, B& ob) {
    for (const T& elem : container) {
      Serializer<T, B>::serialize(elem, ob);
    }
  }

  static void parse(std::array<T, N>& container, B& ib) {
    for (size_t i = 0; i < N; i++) {
      Serializer<T, B>::parse(container[i], ib);
    }
  }
};

template <class T, class B, size_t N>
class Serializer<
    std::array<T, N>,
    B,
    typename std::enable_if<std::is_floating_point<T>::value, void>::type> {
 public:
  static void serialize(const std::array<T, N>& container, B& ob) {
    const char* num_ptr = reinterpret_cast<const char*>(container.data());
    ob.write(num_ptr, N * sizeof(T));
  }

  static void parse(std::array<T, N>& container, B& ib) {
    char* num_ptr = reinterpret_cast<char*>(container.data());
    ib.read(num_ptr, N * sizeof(T));
  }
};

template <class B, size_t N>
class Serializer<std::array<bool, N>, B> {
 public:
  static void serialize(const std::array<bool, N>& container, B& ob) {
    if (N == 0) return;
    char chunk = 0;
    size_t n_chunk_elems = 0;
    for (size_t i = 0; i < N; i++) {
      if (n_chunk_elems == 8) {
        ob.write_char(chunk);
        chunk = 0;
        n_chunk_elems = 0;
      }
      if (container[i]) {
        chunk |= BIT_FILTERS[n_chunk_elems];
      }
      n_chunk_elems++;
    }

    if (n_chunk_elems > 0) {
      ob.write_char(chunk);
    }
  }

  static void parse(std::array<bool, N>& container, B& ib) {
    char chunk = ib.read_char();
    size_t n_chunk_elems = 0;
    for (size_t i = 0; i < N; i++) {
      if (n_chunk_elems == 8) {
        chunk = ib.read_char();
        n_chunk_elems = 0;
      }
      container[i] = (chunk & BIT_FILTERS[n_chunk_elems]) == 0 ? false : true;
      n_chunk_elems++;
    }
  }
};

}  // namespace hps
#endif
