// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::string base64_decode(std::string const& encoded_string);

#endif
