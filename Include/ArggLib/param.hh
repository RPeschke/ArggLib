#ifndef param_h__
#define param_h__
#include <type_traits>
#include <numeric>
#include <tuple>



#if(_MSC_VER >1910 && !defined(__CLING__))
#define ___ArggLib_Apply(Fun, par) std::apply(Fun, par)

#else
#ifndef ArggLibApply_h__
#include "ArggLib/deprecated/apply.hh"
#endif

#define ___ArggLib_Apply(Fun, par) ArggLib::deprecated::apply(Fun, par)

#endif
namespace ArggLib {
	
	template <typename T>
	class param_impl;

	template <typename T>
	auto make_param_impl(T&& t) ->decltype(param_impl<T>(std::forward<T>(t)));

	class param_base {};



	template <typename T>
  class param_impl :public param_base {
  public:
    typename std::remove_reference<T>::type m_params;


    param_impl(const T& param_) :m_params(param_) {

    }
    template<typename T1>
    auto operator<< (T1&& t)->decltype(make_param_impl(std::tuple_cat(m_params, std::make_tuple(t)))) {
      return make_param_impl(std::tuple_cat(m_params, std::make_tuple(t)));
    }


    template <typename F>
    auto operator|(F&& f) const->decltype(___ArggLib_Apply(std::forward<F>(f),m_params)){
			return  ___ArggLib_Apply(std::forward<F>(f),m_params);
		}
	};





	template <typename T>
	auto make_param_impl(T&& t) ->decltype(param_impl<T>(std::forward<T>(t))) {
		return param_impl<T>(std::forward<T>(t));
	}
	class param :public param_base {
	public:
		template<typename T>
		auto operator<< (T&& t) const ->decltype(make_param_impl(std::make_tuple(std::forward<T>(t)))) {
			return make_param_impl(std::make_tuple(std::forward<T>(t)));
		}


		template<typename F>
		auto operator | (F&& f) const ->decltype (f()) {
			return f();
		}
	};



	void __is_not_param(param&);
	void __is_not_param(param&&);
	void __is_not_param(const param&);

	template<typename... ARGS>
	int __is_not_param(ARGS&&...);


}
#endif // param_h__
