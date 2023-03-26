#ifndef JL2922_HPS_INT_SERIALIZER_H
#define JL2922_HPS_INT_SERIALIZER_H

#include <iostream>
#include <type_traits>
#include "uint_serializer.h"

namespace hps {

// Serialize and parse with unsigned integer serializer through zigzag encoding.

template <class T, class B>
class Serializer<
    T,
    B,
    typename std::enable_if<std::is_signed<T>::value && std::is_integral<T>::value, void>::type> {
 public:
  static void serialize(const T& num, B& ob) {
    const size_t n_bits = sizeof(num) * 8;
    using UT = typename std::make_unsigned<T>::type;
    UT zigzaged_num = (num << 1) ^ (num >> (n_bits - 1));
    Serializer<UT, B>::serialize(zigzaged_num, ob);
  }

  static void parse(T& num, B& ib) {
    using UT = typename std::make_unsigned<T>::type;
    UT zigzaged_num;
    Serializer<UT, B>::parse(zigzaged_num, ib);
    num = (-(zigzaged_num & 1)) ^ (zigzaged_num >> 1);
  }
};

}  // namespace hps
#endif
