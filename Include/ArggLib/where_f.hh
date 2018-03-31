#ifndef where_f_h__
#define where_f_h__
#include "ArggLib/vector_helpers.hh"
#include "ArggLib/func.hh"
namespace ArggLib{
	namespace ArggLib_impl {

		template<typename T>
		class where_f_impl :public fun_impl_b0 {
		public:
			where_f_impl(T f1) : m_fun(std::move(f1)) {}
			T m_fun;

			template <typename T1>
			auto operator()(const T1& t1 ) {
				T1 vec(t1);
				vec.erase(std::remove_if(vec.begin(), vec.end(), m_fun), vec.end());
				return vec;
			}

		};

	}

	template <typename T>
	auto where_f(T f1) {
		return  ArggLib_impl::where_f_impl<T>(std::move(f1));
	}

#define _where_f(x)  ArggLib::where_f([&](cautor _x) { return x ; } )
}

#endif // where_f_h__
