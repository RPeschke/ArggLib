#ifndef ArggLibEvaluate_h__
#define ArggLibEvaluate_h__

namespace ArggLib {

	template <typename... T> 
		std::false_type is_evalable_and_returns_impl(T&& ...);

 template <typename F,typename... ARGS> 
 auto is_evalable_and_returns_impl(const F& t, ARGS&&... args) -> decltype(_____try_and_set_return_type<std::true_type>(t(args...)));
 template <typename F, typename... ARGS>
 auto is_evalable_and_returns_impl(F& t, ARGS&&... args)  -> decltype(_____try_and_set_return_type<std::true_type>(t(args...)));
 template <typename F, typename... ARGS>
 auto is_evalable_and_returns_impl(F&& t, ARGS&&... args)  -> decltype(_____try_and_set_return_type<std::true_type>(t(args...)));
 template <typename F, typename... ARGS>
 auto is_evalable_and_returns() ->decltype(is_evalable_and_returns_impl(____get_T<F>()( ____get_T<ARGS>()...)));




 template<typename FUN, typename NEXT, typename... ARGS>
 procReturn do_ignore_imput_and_ignore_return(FUN&& f, NEXT&& n, ARGS&&... args){
	 f();
	 return n(std::forward<ARGS>(args)...);
 }

 template<typename... FUN>
 auto do_process_imput_and_ignore_return(FUN&&... args) {

	 return do_ignore_imput_and_ignore_return(std::forward<FUN>(args)...);
 }

 template<typename FUN, typename NEXT, typename... ARGS>
 auto do_process_imput_and_ignore_return(FUN&& f, NEXT&& n, ARGS&&... args) -> decltype(f(args...),____get_T<procReturn>()) {
	 f(args...);
	 return n(std::forward<ARGS>(args)...);
 }


 template<typename... FUN>
 auto do_Ignore_imput_and_return(FUN&&... args) {

	 return do_process_imput_and_ignore_return(std::forward<FUN>(args)...);
 }

 template<typename FUN, typename NEXT, typename... ARGS>
 auto do_Ignore_imput_and_return(FUN&& f, NEXT&& n, ARGS&&... args) -> decltype(_____try_and_set_return_type<procReturn>(f())) {

	 return n(std::forward<ARGS>(args)..., f());
 }



 template<typename... FUN>
 auto do_process_imput_and_return(FUN&&... args) {
	 return do_Ignore_imput_and_return(std::forward<FUN>(args)...);
	
 }

 template<typename FUN,typename NEXT,typename... ARGS>
 auto do_process_imput_and_return(FUN&& f,NEXT&& n, ARGS&&... args ) -> decltype(_____try_and_set_return_type<procReturn>(f(args...))){

	 return n(std::forward<ARGS>(args)..., f(args...));
 }



	template<typename T>
	class Evaluate_impl {
	public:
		T m_fun;

		Evaluate_impl(T fun) :m_fun(std::move(fun)) {

		}

		template <typename NEXT_T, typename... ARGS>
		//__ENABLE_IF(is_evalable_and_returns<T, ____get_T<ARGS>()...>(), procReturn) 
			auto operator()(NEXT_T&& next, ARGS&&... args) {

				return do_process_imput_and_return(m_fun, next, args...);
			//return next(std::forward<ARGS>(args)..., m_fun(args...));
		}

// 		template <typename NEXT_T, typename... ARGS>
// 		auto operator()(NEXT_T&& next, ARGS&&... args)  {
// 
// 			m_fun();
// 			return next(std::forward<ARGS>(args)...);
// 		}

	};



	template <typename T>
	Evaluate_impl<T> Evaluate(T fun) {
		return Evaluate_impl<T>(std::move(fun));
	}
}

#endif // ArggLibEvaluate_h__
