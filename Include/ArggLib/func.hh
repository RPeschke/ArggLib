#ifndef ArggLibfunc_h__
#define ArggLibfunc_h__

#include <vector>
#include <numeric>
#include <tuple>

namespace ArggLib {

	inline std::vector<int> get_ints2(int  in) {

		std::vector<int> ret;
		for (int i = 0; i < in; ++i) {
			ret.push_back(i);
		}

		return ret;
	}

  inline	std::vector<int> get_ints3(int  start_, int end_, int step = 1) {

		std::vector<int> ret;
		for (int i = start_; i < end_; i += step) {
			ret.push_back(i);
		}

		return ret;
	}


  inline	int sum_ints(const std::vector<int>& vec) {

		return std::accumulate(vec.begin(), vec.end(), 0);


	}





	template <typename T>
	auto getvec4(T t) {
		std::vector<T> ret;
		for (T i = 0.0; i < t; ++i) {
			ret.push_back(i);
		}

		return ret;
	}


	class sum_all {
	public:
		template <typename T>
		auto operator()(T&& t) const {
			return accumulate(t.begin(), t.end(), typename T::value_type(0));
		}
		template <typename T1, typename T2>
		auto operator()(T1&& t, T2 && start_) const {
			return accumulate(t.begin(), t.end(), start_);
		}
	};


	template <size_t N, typename T>
	auto make_fun_impl(T&& t);


	class fun {
	public:
		template<typename T>
		auto operator* (T&& t) {
			return make_fun_impl<0>(std::forward<T>(t));
		}
		template<typename T>
		auto operator> (T&& t) {
			return make_fun_impl<0>(std::forward<T>(t));
		}

		template<typename T>
		auto operator<< (T&& t) {
			return make_fun_impl<0>(std::forward<T>(t));
		}
	};




	enum func_pipeline_op
	{
		_X
	};

	class _run {

	};
	template <typename T, std::size_t N>
	class fun_impl_b {
	public:
		fun_impl_b(T&& t) :m_function(std::forward<T>(t)) {}

		template<typename... Param_T>
		auto operator()(Param_T&&... p)  {
			return m_function(p...);
		}
		T m_function;

		template<typename T1>
		auto operator > (T1&& t1) {
			return make_fun_impl<0>([=](auto&&... s) mutable { return t1(m_function(s...)); });
		}




		auto operator<<= (const _run& t) {
			return m_function();
		}
		auto operator<<= (_run& t) {
			return m_function();
		}
		auto operator<<= (_run&& t) {
			return m_function();
		}

		auto operator<= (const _run& t) {
			return m_function();
		}
		auto operator<= (_run& t) {
			return m_function();
		}
		auto operator<= (_run&& t) {
			return m_function();
		}

		auto operator<< (func_pipeline_op t) {
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
		auto operator << (T1&& t1) {
			return make_fun_impl<N>([t1, _function = this->m_function](auto&&... s) mutable { return _function(t1, s...); });
		}

		auto operator<<  (func_pipeline_op t) {
			return make_fun_impl<N + 1>(this->m_function);
		}
	};


	template <typename T>
	class fun_impl_d<T, 1> : public fun_impl_b<T, 1> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 1>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) {
			return make_fun_impl<1>([t1, _function = this->m_function](auto&& x1, auto&&... s) mutable{ return _function(x1, t1, s...); });
		}

		auto operator<<  (func_pipeline_op t) {
			return make_fun_impl<2>(this->m_function);
		}
	};


	template <typename T>
	class fun_impl_d<T, 2> : public fun_impl_b<T, 2> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 2>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) {
			return make_fun_impl<2>([t1, _function = this->m_function](auto&& x1, auto&& x2, auto&&... s) mutable { return _function(x1, x2, t1, s...); });
		}
		
		auto operator<< (func_pipeline_op t) {
			return make_fun_impl<3>(this->m_function);
		}
	};


	template <typename T>
	class fun_impl_d<T, 3> : public fun_impl_b<T, 3> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 3>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) {
			return make_fun_impl<3>([t1, _function = this->m_function](auto&& x1, auto&& x2, auto&& x3, auto&&... s)mutable { return _function(x1, x2, x3, t1, s...); });
		}
		
		auto operator<<  (func_pipeline_op t) {
			return make_fun_impl<4>(this->m_function);
		}
	};

	template <typename T>
	class fun_impl_d<T, 4> : public fun_impl_b<T, 4> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 4>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) {
			return make_fun_impl<4>([t1, _function = this->m_function](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&&... s) mutable { return _function(x1, x2, x3, x4, t1, s...); });
		}
		
		auto operator<<  (func_pipeline_op t) {
			return make_fun_impl<5>(this->m_function);
		}
	};

	template <typename T>
	class fun_impl_d<T, 0> : public fun_impl_b<T, 0> {
	public:
		fun_impl_d(T&& t) :fun_impl_b<T, 0>(std::forward<T>(t)) {}

		template<typename T1>
		auto operator << (T1&& t1) {
			return make_fun_impl<0>([t1, _function=this->m_function](auto&&... s) mutable { return _function(t1, s...); });
		}
		
		auto operator<< (func_pipeline_op t) {
			return make_fun_impl<1>(this->m_function);
		}
	};

	class _void {
	public:
		template < typename T, std::size_t N>
		auto operator>>=(fun_impl_d<T, N>& f) {
			return f();
		}

		template < typename T, std::size_t N>
		auto operator>>=(fun_impl_d<T, N>&& f) {
			return f();
		}
		template <typename T, std::size_t N>
		auto operator>>=(const  fun_impl_d<T, N>& f) {
			return f();
		}
	};


	template <typename IN, typename T, std::size_t N, typename std::enable_if<!std::is_same<ArggLib::param, IN>::value, int>::type = 0>
	auto operator|(IN&& in_, fun_impl_d<T, N>& f)
	{
		return f(std::forward<IN>(in_));
	}

	template <typename IN, typename T, std::size_t N, typename std::enable_if<!std::is_same<ArggLib::param, IN>::value, int>::type = 0>
	auto operator|(IN&& in_, fun_impl_d<T, N>&& f)
	{
		return f(std::forward<IN>(in_));
	}
	template <typename IN, typename T, std::size_t N , typename std::enable_if<!std::is_same<ArggLib::param,IN>::value , int>::type = 0>
	auto  operator|(IN&& in_, const  fun_impl_d<T, N>& f) 
	{
		return f(std::forward<IN>(in_));
	}

	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>& f) {
		return  apply(f, in_.m_params);
	}
	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>&& f) {
		return  apply(f, in_.m_params);
	}

	template <typename IN, typename T, std::size_t N>
	auto operator|(param_impl<IN>&& in_, const  fun_impl_d<T, N>& f) {
		return  apply(f, in_.m_params);
	}
	template <typename IN, typename T, std::size_t N>
	auto operator|(const param_impl<IN>& in_, fun_impl_d<T, N>& f) {
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
	auto operator|(param_impl<IN>& in_, fun_impl_d<T, N>& f) {

		return apply(f, in_.m_params);
	}





	

	template <std::size_t N, typename T>
	auto make_fun_impl(T&& t) {
		return fun_impl_d<T, N>(std::forward<T>(t));
	}

}
#endif // ArggLibfunc_h__
