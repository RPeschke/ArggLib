#ifndef ArggLibWhere_h__
#define ArggLibWhere_h__
#include "ArggLib/procReturn.hh"
#include "ArggLib/proc.hh"

namespace ArggLib {
	namespace ArggLib_impl {
		template<typename T>
		class Where_impl {
		public:
			T m_fun;

			Where_impl(T fun) :m_fun(std::move(fun)) {

			}

			template <typename NEXT_T, typename... ARGS>
			procReturn operator()(NEXT_T&& next, ARGS&&... args) {

				if (m_fun(args...)) {
					return next(std::forward<ARGS>(args)...);
				}

				return procReturn::success;


			}


		};
	}
	template <typename T>
	auto Where(T fun) -> decltype(proc() >> ArggLib_impl::Where_impl<T>(std::move(fun))) {
		return proc()>> ArggLib_impl::Where_impl<T>(std::move(fun));
	}

  enum where_begin_e{
  __whereBegin_enum
  };

  template<typename T>
  auto operator*(where_begin_e, T&& t) -> decltype(Where(std::forward<T>(t))) {
    return Where(std::forward<T>(t));
  }
#define _where __whereBegin_enum * [&](cautor _x)
#define _where_p(x)  ArggLib::Where([&](cautor _x){ return x; } )
}

#endif // ArggLibWhere_h__
