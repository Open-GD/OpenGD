#ifndef JL2922_HPS_VECTOR_SERIALIZER_H
#define JL2922_HPS_VECTOR_SERIALIZER_H

#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
#include "../basic_type/uint_serializer.h"
#include "../serializer.h"
#include "bit_filters.h"
#include "is_unique_ptr.h"

namespace hps {

template <class T, class B>
class Serializer<
    std::vector<T>,
    B,
    typename std::
        enable_if<!std::is_floating_point<T>::value && !hps::is_unique_ptr<T>::value, void>::type> {
 public:
  static void serialize(const std::vector<T>& container, B& ob) {
    Serializer<size_t, B>::serialize(container.size(), ob);
    for (const T& elem : container) {
      Serializer<T, B>::serialize(elem, ob);
    }
  }

  static void parse(std::vector<T>& container, B& ib) {
    size_t n_elems;
    Serializer<size_t, B>::parse(n_elems, ib);
    container.resize(n_elems);
    for (size_t i = 0; i < n_elems; i++) {
      Serializer<T, B>::parse(container[i], ib);
    }
  }
};

template <class T, class B>
class Serializer<
    std::vector<T>,
    B,
    typename std::enable_if<std::is_floating_point<T>::value, void>::type> {
 public:
  static void serialize(const std::vector<T>& container, B& ob) {
    const size_t n_elems = container.size();
    Serializer<size_t, B>::serialize(n_elems, ob);
    const char* num_ptr = reinterpret_cast<const char*>(container.data());
    ob.write(num_ptr, n_elems * sizeof(T));
  }

  static void parse(std::vector<T>& container, B& ib) {
    size_t n_elems;
    Serializer<size_t, B>::parse(n_elems, ib);
    container.resize(n_elems);
    char* num_ptr = reinterpret_cast<char*>(container.data());
    ib.read(num_ptr, n_elems * sizeof(T));
  }
};

template <class B>
class Serializer<std::vector<bool>, B> {
 public:
  static void serialize(const std::vector<bool>& container, B& ob) {
    const size_t n_elems = container.size();
    Serializer<size_t, B>::serialize(n_elems, ob);
    char chunk = 0;
    size_t n_chunk_elems = 0;
    for (size_t i = 0; i < n_elems; i++) {
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

  static void parse(std::vector<bool>& container, B& ib) {
    size_t n_elems;
    Serializer<size_t, B>::parse(n_elems, ib);
    container.resize(n_elems);
    char chunk = ib.read_char();
    size_t n_chunk_elems = 0;
    for (size_t i = 0; i < n_elems; i++) {
      if (n_chunk_elems == 8) {
        chunk = ib.read_char();
        n_chunk_elems = 0;
      }
      container[i] = (chunk & BIT_FILTERS[n_chunk_elems]) == 0 ? false : true;
      n_chunk_elems++;
    }
  }
};

template <class T, class B>
class Serializer<std::vector<std::unique_ptr<T>>, B> {
 public:
  static void serialize(const std::vector<std::unique_ptr<T>>& container, B& ob) {
    const size_t n_elems = container.size();
    std::vector<bool> exist(n_elems, false);
    for (size_t i = 0; i < n_elems; i++) {
      if (container[i]) exist[i] = true;
    }
    Serializer<std::vector<bool>, B>::serialize(exist, ob);
    for (size_t i = 0; i < n_elems; i++) {
      if (container[i]) {
        Serializer<T, B>::serialize(*container[i], ob);
      }
    }
  }

  static void parse(std::vector<std::unique_ptr<T>>& container, B& ib) {
    std::vector<bool> exist;
    Serializer<std::vector<bool>, B>::parse(exist, ib);
    const size_t n_elems = exist.size();
    container.resize(n_elems);
    for (size_t i = 0; i < n_elems; i++) {
      if (exist[i]) {
        if (!container[i]) container[i].reset(new T());
        Serializer<T, B>::parse(*container[i], ib);
      } else {
        container[i].reset(nullptr);
      }
    }
  }
};

}  // namespace hps
#endif
