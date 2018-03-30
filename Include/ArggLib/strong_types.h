#ifndef ArggLib_strong_types_h__
#define ArggLib_strong_types_h__


#include <string>

#ifdef _DEBUG


#define necessary_CONVERSION(x) x.value
#define Un_necessary_CONVERSION(x) x.value

#define TYPE_CLASS(name,type) \
class  name { \
public: \
  explicit name(const type& param_) :value(param_) {}\
  type value; \
  friend bool operator==(const name& lhs, const name& rhs) { return lhs.value == rhs.value;} \
  friend bool operator!=(const name& lhs, const name& rhs) { return lhs.value != rhs.value;} \
  friend bool operator<(const name& lhs, const name& rhs) { return lhs.value < rhs.value;} \
  friend bool operator>(const name& lhs, const name& rhs) { return lhs.value > rhs.value;} \
  friend name operator+(const name& lhs, const name& rhs) { return name(lhs.value + rhs.value);} \
}

template <typename out_t, typename in_t>
out_t strong_cast(const in_t& in_) {
  return out_t(necessary_CONVERSION(in_));
}


#else 
#define necessary_CONVERSION(x) x
#define Un_necessary_CONVERSION(x) x

#define TYPE_CLASS(name,type) \
typedef  type name


template <typename out_t, typename in_t>
const out_t& strong_cast(const in_t& in_) {
  return in_;
}

template <typename out_t, typename in_t>
out_t& strong_cast(in_t& in_) {
  return in_;
}
#endif // _DEBUG







#endif // strong_types_h__