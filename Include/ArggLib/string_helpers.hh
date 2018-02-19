#ifndef ArggLibstring_helpers_h__
#define ArggLibstring_helpers_h__

#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "ArggLib/cautor.hh"

namespace ArggLib {
  using string = std::string;
  using cstringr = const string&;
  using cstring = const string;
  using stringr = string&;

  using strings = std::vector<string>;
  using cstrings = const std::vector<string>;
  using cstringsr = const std::vector<string>&;
  using stringsr = std::vector<string>&;


  // trim from start (in place)
  static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
      return !std::isspace(ch);
    }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
      return !std::isspace(ch);
    }).base(), s.end());
  }

  // trim from both ends (in place)
  static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
  }

  template <typename T>
  void split_string(cstringr input_str, const char delimiter, T&& outFun) {
    std::stringstream str_stream(input_str);
    std::string buffer;
    while (std::getline(str_stream, buffer, delimiter))
    {
      trim(buffer);
      outFun(buffer);
    }
  }

  inline strings split_string2vector(cstringr input_str, const char delimiter) {
    strings ret;
    split_string(input_str, delimiter, [&ret](cautor in) { ret.push_back(in); });
    return ret;
  }
}



#endif // ArggLibstring_helpers_h__
