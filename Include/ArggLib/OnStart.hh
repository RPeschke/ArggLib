#ifndef ArggLibOnStart_h__
#define ArggLibOnStart_h__

namespace ArggLib {
	template<typename T>
	class OnStart_impl {
	public:
		T m_fun;

		OnStart_impl(T fun) :m_fun(std::move(fun)) {

		}

		procReturn Start() {
			m_fun();
			return procReturn::success;
		}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {


			return next(std::forward<ARGS>(args)...);
		}


	};


	template <typename T>
  auto OnStart(T fun) {
		return proc()>> OnStart_impl<T>(std::move(fun));
	}
}


#endif // ArggLibOnStart_h__
