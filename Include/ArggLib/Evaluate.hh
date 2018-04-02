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



 template <typename F, typename... T>
 auto __try_to_evaluate_impl(F&& f, T&&... args) -> decltype(std::is_same<decltype(f(args...)), void>::type());
 template <typename... T>
 std::false_type __try_to_evaluate(T...);
 template <typename F, typename... T>
 auto __try_to_evaluate(F&& f, T&&... args) ->decltype(_____try_and_set_return_type<std::true_type>(__try_to_evaluate_impl(f, args...)));



 template<typename FUN, typename NEXT, typename... ARGS>
 procReturn do_ignore_imput_and_ignore_return(FUN&& f, NEXT&& n, ARGS&&... args){
	 f();
	 return n(std::forward<ARGS>(args)...);
 }

 template<typename... FUN>
 auto do_process_imput_and_ignore_return(FUN&&... args) -> decltype(do_ignore_imput_and_ignore_return(std::forward<FUN>(args)...)) {

	 return do_ignore_imput_and_ignore_return(std::forward<FUN>(args)...);
 }

 template<typename FUN, typename NEXT, typename... ARGS>
 auto do_process_imput_and_ignore_return(FUN&& f, NEXT&& n, ARGS&&... args) -> decltype(_____try_and_set_return_type<procReturn>(__try_to_evaluate_impl(f, args...))) {
	 f(args...);
	 return n(std::forward<ARGS>(args)...);
 }


 template<typename... FUN>
 auto do_Ignore_imput_and_return(FUN&&... args) -> decltype (do_process_imput_and_ignore_return(std::forward<FUN>(args)...) ){

	 return do_process_imput_and_ignore_return(std::forward<FUN>(args)...);
 }

 template<typename FUN, typename NEXT, typename... ARGS>
 auto do_Ignore_imput_and_return(FUN&& f, NEXT&& n, ARGS&&... args) -> decltype(_____try_and_set_return_type<procReturn>(f())) {

	 return n(std::forward<ARGS>(args)..., f());
 }



 template<typename... FUN>
 auto do_process_imput_and_return(FUN&&... args) -> decltype(do_Ignore_imput_and_return(std::forward<FUN>(args)...)) {
	 return do_Ignore_imput_and_return(std::forward<FUN>(args)...);
	
 }

 template<typename FUN,typename NEXT,typename... ARGS>
 auto do_process_imput_and_return(FUN&& f,NEXT&& n, ARGS&&... args ) -> decltype(_____try_and_set_return_type<procReturn>(f(args...))){
	 auto ret = f(args...);
	 return n(std::forward<ARGS>(args)..., ret);
 }



	template<typename T>
	class Evaluate_impl {
	public:
		T m_fun;

		Evaluate_impl(T fun) :m_fun(std::move(fun)) {}

		template <typename NEXT_T, typename... ARGS>
		auto operator()(NEXT_T&& next, ARGS&&... args) -> decltype(do_process_imput_and_return(m_fun, next, args...)) {
				return do_process_imput_and_return(m_fun, next, args...);
		}



	};



	template <typename T>
	auto Evaluate(T fun) -> decltype(proc() >> Evaluate_impl<T>(std::move(fun))) {
		return proc() >>  Evaluate_impl<T>(std::move(fun));
	}
}

#endif // ArggLibEvaluate_h__
