#ifndef ArggLibfunc_h__
#define ArggLibfunc_h__

#include <vector>
#include <numeric>
#include <tuple>
#include "ArggLib/param.hh"
#include <type_traits>
#include "ArggLib/type_trates.hh"
#include "ArggLib/add_parameter.hh"

namespace ArggLib {


  template <typename T, std::size_t N>
  class fun_impl_d;


	template <size_t N, typename T>
  fun_impl_d<T, N> make_fun_impl(T&& t) ;


	class fun {
	public:
		template<typename T>
		auto operator* (T&& t) ->decltype(make_fun_impl<0>(std::forward<T>(t)) ){
			return make_fun_impl<0>(std::forward<T>(t));
		}
		template<typename T>
		auto operator> (T&& t) ->decltype(make_fun_impl<0>(std::forward<T>(t))) {
			return make_fun_impl<0>(std::forward<T>(t));
		}

		template<typename T>
		auto operator<< (T&& t) ->decltype(make_fun_impl<0>(std::forward<T>(t))) {
			return make_fun_impl<0>(std::forward<T>(t));
		}
	};




	enum func_pipeline_op
	{
		_X
	};



	class fun_impl_b0 {

	};
	template <typename T, std::size_t N>
	class fun_impl_b :public fun_impl_b0 {
	public:
		T m_function;
		fun_impl_b(T&& t) :m_function(std::forward<T>(t)) {}

		template<typename... Param_T>
		auto operator()(Param_T&&... p) ->decltype(this->m_function(p...) ){
			return this->m_function(p...);
		}
#ifndef  __CLING__ 
		auto  operator->()  
		{

			return  &m_function;
		}
#endif
		T& operator * () 
		{
			return m_function;
		}








		auto operator<< (func_pipeline_op t) -> decltype (make_fun_impl<N + 1>(m_function)) {
			return make_fun_impl<N + 1>(m_function);
		}

	};



	template <typename T, std::size_t N>
	class fun_impl_d : public fun_impl_b<T, N> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, N>(std::forward<T>(t)) {
			//static_assert(false, "Dummy function not implemented");

		}

		template<typename T1>
		int operator << (T1&& t1) {
			//static_assert(false, "");
			//return make_fun_impl<N>([t1, _function = this->m_function](auto&&... s) mutable { return _function(t1, s...); });
			return 0;
		}

		auto operator<<  (func_pipeline_op t) -> decltype(make_fun_impl<N + 1>(this->m_function)) {
			//static_assert(false, "");
			return make_fun_impl<N + 1>(this->m_function);
		}
	};

	template<typename T, typename Func_t>
	class fun_impl_d1_inner {
		T t1;
		Func_t m_func;
	public:
		fun_impl_d1_inner(T t1_, Func_t fun_) :t1(std::move(t1_)), m_func(std::move(fun_)) {}

		template <typename X1,  typename... ARGS>
		auto operator()(X1&& x1,  ARGS&&... args) ->decltype(m_func(std::forward<X1>(x1),  t1, std::forward<ARGS>(args)...)) {
			return m_func(std::forward<X1>(x1), t1, std::forward<ARGS>(args)...);
		}
	};
	template<typename T, typename Func_t>
	auto make_fun_impl_d1_inner(T t1_, Func_t fun_) -> decltype(fun_impl_d1_inner<T, Func_t>(std::move(t1_), std::move(fun_))) {
		return fun_impl_d1_inner<T, Func_t>(std::move(t1_), std::move(fun_));
	}
	template <typename T>
	class fun_impl_d<T, 1> : public fun_impl_b<T, 1> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 1>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) ->decltype(make_fun_impl<1>(make_fun_impl_d1_inner(t1, this->m_function))) {
			return make_fun_impl<1>(make_fun_impl_d1_inner(t1, this->m_function));
		}

		auto operator<<  (func_pipeline_op t) ->decltype( make_fun_impl<2>(this->m_function)) {
			return make_fun_impl<2>(this->m_function);
		}
	};

	template<typename T, typename Func_t>
	class fun_impl_d2_inner {
		T t1;
		Func_t m_func;
	public:
		fun_impl_d2_inner(T t1_, Func_t fun_) :t1(std::move(t1_)), m_func(std::move(fun_)) {}

		template <typename X1, typename X2,  typename... ARGS>
		auto operator()(X1&& x1, X2&& x2, ARGS&&... args) ->decltype(m_func(std::forward<X1>(x1), std::forward<X2>(x2),  t1, std::forward<ARGS>(args)...)) {
			return m_func(std::forward<X1>(x1), std::forward<X2>(x2), t1, std::forward<ARGS>(args)...);
		}
	};
	template<typename T, typename Func_t>
	auto make_fun_impl_d2_inner(T t1_, Func_t fun_) -> decltype(fun_impl_d2_inner<T, Func_t>(std::move(t1_), std::move(fun_))) {
		return fun_impl_d2_inner<T, Func_t>(std::move(t1_), std::move(fun_));
	}
	template <typename T>
	class fun_impl_d<T, 2> : public fun_impl_b<T, 2> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 2>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator<< (T1&& t1) ->decltype(make_fun_impl<2>(make_fun_impl_d2_inner(t1, this->m_function))) {
			return make_fun_impl<2>(make_fun_impl_d2_inner(t1, this->m_function));
		}
		
		auto operator<< (func_pipeline_op t)  -> decltype(make_fun_impl<3>(this->m_function) ){
			return make_fun_impl<3>(this->m_function);
		}
	};

	template<typename T, typename Func_t>
	class fun_impl_d3_inner {
		T t1;
		Func_t m_func;
	public:
		fun_impl_d3_inner(T t1_, Func_t fun_) :t1(std::move(t1_)), m_func(std::move(fun_)) {}

		template <typename X1, typename X2, typename X3, typename... ARGS>
		auto operator()(X1&& x1, X2&& x2, X3&& x3, ARGS&&... args) ->decltype(m_func(std::forward<X1>(x1), std::forward<X2>(x2), std::forward<X3>(x3), t1, std::forward<ARGS>(args)...)) {
			return m_func(std::forward<X1>(x1), std::forward<X2>(x2), std::forward<X3>(x3),  t1, std::forward<ARGS>(args)...);
		}
	};
	template<typename T, typename Func_t>
	auto make_fun_impl_d3_inner(T t1_, Func_t fun_) -> decltype(fun_impl_d3_inner<T, Func_t>(std::move(t1_), std::move(fun_))) {
		return fun_impl_d3_inner<T, Func_t>(std::move(t1_), std::move(fun_));
	}
	template <typename T>
	class fun_impl_d<T, 3> : public fun_impl_b<T, 3> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 3>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) ->decltype(make_fun_impl<3>(make_fun_impl_d3_inner(t1, this->m_function))) {
			return make_fun_impl<3>(make_fun_impl_d3_inner(t1, this->m_function));
		}
		
		auto operator<<  (func_pipeline_op t) ->decltype(make_fun_impl<4>(this->m_function)) {
			return make_fun_impl<4>(this->m_function);
		}
	};

	template<typename T, typename Func_t>
	class fun_impl_d4_inner {
		T t1;
		Func_t m_func;
	public:
		fun_impl_d4_inner(T t1_, Func_t fun_) :t1(std::move(t1_)), m_func(std::move(fun_)) {}

		template <typename X1 , typename X2 , typename X3, typename X4, typename... ARGS>
		auto operator()(X1&& x1, X2&& x2, X3&& x3 ,X4 && x4,  ARGS&&... args) ->decltype(m_func(std::forward<X1>(x1), std::forward<X2>(x2), std::forward<X3>(x3), std::forward<X4>(x4), t1, std::forward<ARGS>(args)...)) {
			return m_func( std::forward<X1>(x1), std::forward<X2>(x2), std::forward<X3>(x3), std::forward<X4>(x4), t1, std::forward<ARGS>(args)...);
		}
	};
	template<typename T, typename Func_t>
	auto make_fun_impl_d4_inner(T t1_, Func_t fun_) -> decltype(fun_impl_d4_inner<T, Func_t>(std::move(t1_), std::move(fun_))) {
		return fun_impl_d4_inner<T, Func_t>(std::move(t1_), std::move(fun_));
	}
	template <typename T>
	class fun_impl_d<T, 4> : public fun_impl_b<T, 4> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 4>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) ->decltype(make_fun_impl<4>(make_fun_impl_d4_inner(t1, this->m_function))) {
			return make_fun_impl<4>(make_fun_impl_d4_inner(t1, this->m_function));
		}
		
		auto operator<<  (func_pipeline_op t) ->decltype(make_fun_impl<5>(this->m_function)) {
			return make_fun_impl<5>(this->m_function);
		}
	};

	template<typename T, typename Func_t> 
	class fun_impl_d0_inner {
		T t1;
		Func_t m_func;
	public:
		fun_impl_d0_inner(T t1_, Func_t fun_) :t1(std::move(t1_)), m_func(std::move(fun_)) {}

		template <typename... ARGS>
		auto operator()(ARGS&&... args) -> decltype(m_func(t1, args...)){
			return m_func(t1, args...);
		}
	};
	template<typename T, typename Func_t>
	auto make_fun_impl_d0_inner(T t1_, Func_t fun_) -> decltype(fun_impl_d0_inner<T, Func_t>(std::move(t1_), std::move(fun_))) {
		return fun_impl_d0_inner<T, Func_t>(std::move(t1_),std::move(fun_));
	}

	template <typename T>
	class fun_impl_d<T, 0> : public fun_impl_b<T, 0> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 0>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1)-> decltype(make_fun_impl<0>(make_fun_impl_d0_inner(t1, this->m_function))) {
			return make_fun_impl<0>(make_fun_impl_d0_inner(t1, this->m_function));
		}
		
		auto operator<< (func_pipeline_op t) ->decltype(make_fun_impl<1>(this->m_function)) {
			return make_fun_impl<1>(this->m_function);
		}
	};

	template<typename T1, typename T2>
	class fun_chained_function_call {
		T1 t1;
		T2 t2;
	public:
		fun_chained_function_call(T1 t1_, T2 t2_) :t1(t1_), t2(t2_) {}
		template <typename... S>
		auto operator()(S&&... s) ->decltype(t2(t1(std::forward<S>(s)...))){
			return t2(t1( std::forward<S>(s)...));
		}
	};
	template<typename T1, typename T2>
	auto make_fun_chained_function_call(T1 t1,T2 t2) ->decltype(fun_chained_function_call<T1, T2>(std::move(t1), std::move(t2))) {
		return fun_chained_function_call<T1, T2>( std::move(t1), std::move(t2));
	}
	template<typename T1, typename T2,
		typename std::enable_if<std::is_base_of<fun_impl_b0, ArggLib::remove_cvref_t<T1> >::value, int>::type = 0,
		typename std::enable_if<std::is_base_of<fun_impl_b0, ArggLib::remove_cvref_t<T2>  >::value, int>::type = 0 >
	auto operator > (T1&& t1, T2&& t2) -> decltype(make_fun_impl<0>(make_fun_chained_function_call(t1, t2))){
		return make_fun_impl<0>(make_fun_chained_function_call(t1,t2));
	}



	template <typename IN, typename T, 
		typename std::enable_if<!std::is_same<ArggLib::param, ArggLib::remove_cvref_t<IN> >::value, int>::type = 0 ,
		typename std::enable_if<std::is_base_of<fun_impl_b0,  ArggLib::remove_cvref_t<T> >::value, int>::type = 0 >
	auto operator|(IN&& in_, T&& f) ->decltype(f(std::forward<IN>(in_)))
	{
		return f(std::forward<IN>(in_));
	}

// 	template <typename IN, typename T, std::size_t N, typename std::enable_if<!std::is_same<ArggLib::param, IN>::value, int>::type = 0>
// 	auto operator|(IN&& in_, fun_impl_d<T, N>& f)
// 	{
// 		return f(std::forward<IN>(in_));
// 	}
// 
// 	template <typename IN, typename T, std::size_t N, typename std::enable_if<!std::is_same<ArggLib::param, IN>::value, int>::type = 0>
// 	auto operator|(IN&& in_, fun_impl_d<T, N>&& f)
// 	{
// 		return f(std::forward<IN>(in_));
// 	}
// 	template <typename IN, typename T, std::size_t N , typename std::enable_if<!std::is_same<ArggLib::param,IN>::value , int>::type = 0>
// 	auto  operator|(IN&& in_, const  fun_impl_d<T, N>& f) 
// 	{
// 		return f(std::forward<IN>(in_));
// 	}

	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>& f) ->decltype(apply(f, in_.m_params)) {
		return  apply(f, in_.m_params);
	}
	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>&& f) ->decltype(apply(f, in_.m_params)) {
		return  apply(f, in_.m_params);
	}

	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, const  fun_impl_d<T, N>& f) ->decltype(apply(f, in_.m_params)) {
		return  apply(f, in_.m_params);
	}
	template <typename IN, typename T, std::size_t N>
	auto operator|(const param_impl<IN>& in_, fun_impl_d<T, N>& f) ->decltype(apply(f, in_.m_params)){
		return  apply(f, in_.m_params);
	}


// 	template < typename T, std::size_t N>
// 	auto operator|(param&& in_, fun_impl_d<T, N>&& f) {
// 		return  f();
// 	}
// 
// 	template < typename T, std::size_t N>
// 	auto operator|(param&&  in_, const  fun_impl_d<T, N>& f) {
// 		return  f();
// 	}
// 	template < typename T, std::size_t N>
// 	auto operator|(param&&  in_, fun_impl_d<T, N>& f) {
// 		return  f();
// 	}
	

	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>& in_, fun_impl_d<T, N>& f) ->decltype(apply(f, in_.m_params)) {

		return apply(f, in_.m_params);
	}





	

	template <std::size_t N, typename T>
  fun_impl_d<T, N>  make_fun_impl(T&& t) {
		return fun_impl_d<T, N>(std::forward<T>(t));
	}

}
#endif // ArggLibfunc_h__
