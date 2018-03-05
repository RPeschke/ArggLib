#ifndef vector_helpers_h__
#define vector_helpers_h__
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/ArggLib_impl/vector_helpers_impl.hh"


#define ArggLib_to_vector  ArggLib::fun() << ArggLib::ArggLib_impl::to_vector_impl() << ArggLib::_X
#define  ArggLib_sort_by(property) ArggLib::fun() << ArggLib::ArggLib_impl::sort_impl()<< ArggLib::_X << [](cautor e1, cautor e2) {return  e1.property < e2.property; }


namespace ArggLib {


template < typename T>
auto contains( const T& value){
  return fun() << [value](const auto& cont_v) {return std::find(cont_v.begin(), cont_v.end(), value) != cont_v.end(); };
}






enum  __make_vec_enum {
  _to_vector

};


template <typename T>
auto operator<<(__make_vec_enum in_, T t) {
  return ArggLib_impl::make_vec_impl_1<T>(std::move(t));
}

}
#endif // vector_helpers_h__
