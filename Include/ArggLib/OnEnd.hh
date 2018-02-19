#ifndef ArggLibOnEnd_h__
#define ArggLibOnEnd_h__

namespace ArggLib {

	template<typename T>
	class OnEnd_impl {
	public:
		T m_fun;

		OnEnd_impl(T fun) :m_fun(std::move(fun)) {

		}

		procReturn End() {
			m_fun();
			return procReturn::success;
		}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {


			return next(std::forward<ARGS>(args)...);
		}


	};


	template <typename T>
	auto OnEnd(T fun) {
		return proc()>> OnEnd_impl<T>(std::move(fun));
	}
}

#endif // ArggLibOnEnd_h__
