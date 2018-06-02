#ifndef sub_h__
#define sub_h__
#include "ArggLib/type_trates.hh"


namespace ArggLib {
	namespace ArggLib_impl {
		template <typename T>
		class sub_impl {
			T m_fun;
		public:

			sub_impl(T&& fun) :m_fun(std::move(fun)) {}
			sub_impl(const T& fun) :m_fun(fun) {}
			template <typename NEXT_T, typename... ARGS>
			auto operator()(NEXT_T&& next, ARGS&&... args) {

				auto p = proc() >> m_fun;

				return next(p(std::forward<ARGS>(args)...));
			}
		};
	}
	template <typename T>
	auto sub(T&& fun) {
		return proc()>> ArggLib_impl::sub_impl<ArggLib::remove_reference_t<T>>( std::forward<T>(fun));
	}
}
#endif // sub_h__
