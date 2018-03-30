#ifndef vector_helpers_h__
#define vector_helpers_h__
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/ArggLib_impl/vector_helpers_impl.hh"
#include "ArggLib/func.hh"
#include "ArggLib/param.hh"


#define ArggLib_to_vector  ArggLib::fun() << ArggLib::ArggLib_impl::to_vector_impl() << ArggLib::_X
#define  ArggLib_sort_by(property) ArggLib::fun() << ArggLib::ArggLib_impl::sort_impl()<< ArggLib::_X << [](cautor e1, cautor e2) {return  e1.property < e2.property; }


namespace ArggLib {



	template <typename Container_t, typename T>
	void remove_erase( Container_t& vec, const T& value) {
		vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());

	}

  template <typename Container_t, typename T>
  auto contains(const Container_t& cont_v, const T& value) {
    return std::find(cont_v.begin(), cont_v.end(), value) != cont_v.end();
  }
  template < typename T>
  auto contains(const T& value) {
    return fun() << [value](const auto& cont_v) {return std::find(begin(cont_v), end(cont_v), value) != end(cont_v); };
  }


  template < typename F>
  auto contains_if(const F& value) {
	  return fun() << [value](const auto& cont_v) {return std::find_if(begin(cont_v), end(cont_v), value) != end(cont_v); };
  }

  template < typename T>
  auto contains_any( std::initializer_list<T> l) {
	  return fun() << [l](const auto& cont_v) {
		  
		  for (cautor e: cont_v){
			  if (contains(l,e)){
				  return true;
			  }

		  }

		  return false;
	  };
  }

  template < typename T>
  auto contains_all(std::initializer_list<T> l) {
	  return fun() << [l](const auto& cont_v) {
		  std::vector<T> vec(l);
		  if (vec.empty()) {
			  return false;
		  }
		  for (cautor e : cont_v) {

			  if (contains(vec, e)) {
				  remove_erase(vec, e);
				  
			  }
			  if (vec.empty()) {
				  return true;
			  }

		  }

		  return false;
	  };
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
