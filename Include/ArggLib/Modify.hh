#ifndef ArggLibModify_h__
#define ArggLibModify_h__





namespace ArggLib{
	template<typename T>
	class Modify_impl {
	public:
		T m_fun;

		Modify_impl(T fun) :m_fun(std::move(fun)) {

		}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {


			return next(m_fun(args...));
		}


	};
	template <typename T>
	auto Modify(T fun) -> decltype(proc() >> Modify_impl<T>(std::move(fun)) ){
		return proc() >> Modify_impl<T>(std::move(fun)) ;
	}
}

#endif // ArggLibModify_h__