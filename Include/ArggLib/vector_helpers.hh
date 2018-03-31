#ifndef vector_helpers_h__
#define vector_helpers_h__
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/ArggLib_impl/vector_helpers_impl.hh"
#include "ArggLib/func.hh"
#include "ArggLib/param.hh"



#define  ArggLib_sort_by(property) ArggLib::sort( [](cautor e1, cautor e2) {return  e1 < e2; } , [](cautor e){return e.property;} ,[](cautor e) {return e;} )


namespace ArggLib {



	template <typename T1 , typename T2, typename T3>
	inline auto sort(T1  comp, T2 get_proberty,T3 abs_f) {
		return  ArggLib::ArggLib_impl::sort_impl<T1,T2, T3>(std::move(comp),std::move(get_proberty) ,std::move(abs_f));
	}

	inline auto sort() {
		return  sort([](cautor e1, cautor e2) {return  e1 < e2; }, [](cautor e) { return e; }, [](cautor e) { return e; });
	}

	

	template <typename T>
	inline auto to_vector(T f1) {
		return  ArggLib::ArggLib_impl::to_vector_impl<T>(std::move(f1));
	}

	inline auto to_vector() {
		return  to_vector([](cautor e) {return ArggLib_impl::to_vector_impl_convert_element(e); });
	}






	template <typename T>
	inline auto to_string_f(T f_to_string) {
		return  ArggLib_impl::to_string_impl<T>{std::move(f_to_string) };

	}
	inline auto to_string_f() {
		return  to_string_f([](cautor e) { return ArggLib_impl::to_string(e); });

	}
	template <typename Container_t, typename T>
	void remove_erase( Container_t& vec, const T& value) {
		vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());

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
