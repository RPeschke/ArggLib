#ifndef compileTimeHash_h__
#define compileTimeHash_h__

#include <cstdint>

namespace ArggLib {


  namespace detail
  {
    // FNV-1a 32bit hashing algorithm.
    constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
    {
      return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
    int constexpr length(const char* str) {
      return *str ? 1 + length(str + 1) : 0;
    }
  }    // namespace detail

  constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
  {
    return detail::fnv1a_32(s, count);
  }



  constexpr std::uint32_t toHash(const char* s  ){
    return detail::fnv1a_32(s,detail::length(s));
  }
}


#endif // compileTimeHash_h__
