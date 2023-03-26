#ifndef JL2922_HPS_HPS_H
#define JL2922_HPS_HPS_H

#include <iostream>
#include <sstream>
#include "basic_type/basic_type.h"
#include "buffer/buffer.h"
#include "container/container.h"
#include "serializer.h"

namespace hps {

// Serialize data t to an STL ostream.
template <class T>
void to_stream(const T& t, std::ostream& stream) {
  StreamOutputBuffer ob(stream);
  ob << t;
  ob.flush();
}

// Parse from an STL istream and save to the data t passed in.
// Recommended for repeated use inside a loop.
template <class T>
void from_stream(std::istream& stream, T& t) {
  StreamInputBuffer ib(stream);
  ib >> t;
}

// Parse from an STL istream and return the data.
template <class T>
T from_stream(std::istream& stream) {
  T t;
  from_stream<T>(stream, t);
  return t;
}

// Serialize data t to the STL string passed in.
// Recommended for repeated use inside a loop.
template <class T>
void to_string(const T& t, std::string& str) {
  str.clear();
  StringOutputBuffer ob(str);
  ob << t;
  ob.flush();
}

// Serialize data t to an STL string and return it.
template <class T>
std::string to_string(const T& t) {
  std::string str;
  to_string(t, str);
  return str;
}

// Parse from an STL string and save to the data t passed in.
// Recommended for repeated use inside a loop.
template <class T>
void from_string(const std::string& str, T& t) {
  StringInputBuffer ib(str);
  ib >> t;
}

// Parse from an STL string and return the data.
template <class T>
T from_string(const std::string& str) {
  T t;
  from_string<T>(str, t);
  return t;
}

// Parse from a char array and save to the data t passed in.
// Recommended for repeated use inside a loop.
template <class T>
void from_char_array(const char* arr, T& t) {
  CharArrayInputBuffer ib(arr);
  ib >> t;
}

// Parse from a char array and return the data.
template <class T>
T from_char_array(const char* arr) {
  T t;
  from_char_array<T>(arr, t);
  return t;
}

}  // namespace hps
#endif
