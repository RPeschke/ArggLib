#ifndef ArggLib_impl_do_begin_do_end_h__
#define ArggLib_impl_do_begin_do_end_h__
#include "ArggLib/has_member_helper.hh"
#include "ArggLib/try_run_default.hh"
#include "ArggLib/type_trates.hh"
#include <future>
#include <type_traits>


namespace ArggLib {




	namespace ArggLib_impl {



		CREATE_TEST_FOR_MEMBER(hasEnd, End());
		CREATE_TEST_FOR_MEMBER(hasStart, Start());

		CREATE_TEST_FOR_MEMBER(has_t, t);
		CREATE_TEST_FOR_MEMBER(has_n, n);

    

    template <typename... N>
    auto do_end0a_check_for_return(N&&... n) ->decltype(success){
      return success;
    }




    template <typename N, typename T , __ENABLE_IF_V2 (hasEnd<N>)>
    T&& do_end0a_check_for_return(N&& n, T&& value) {
      	n.End(value);
        return value;

    }




		template <typename... N>
		procReturn do_end0(N&&... n) {
			return do_end0a_check_for_return(n...);
		}


    // function has return but no input 
		template <typename N, typename T >
		auto do_end0(N&& n, T&& value) ->decltype(n.End()) {
			return	n.End();

		}


    template <typename N, typename T >
    T&& do_end1a_no_return_value(N&& n, T&& value) {
      n.End(value);
      return value;
    }


		template <typename... N>
		auto do_end1(N&&... n)  -> decltype(do_end0(n...)){
			return do_end0(n...);
		}

    // function hat input und return 
		template <typename N ,typename T >
		auto do_end1(N&& n,T&& value) ->decltype(n.End(value)) {
			return	n.End(value);

		}



    //remaining  element
		template <typename... N>
		auto do_end2(N&&... n) -> decltype(try_run_or_default_End(std::forward<N>( n)...)){


			return try_run_or_default_End(std::forward<N>(n)...);
		}


    // last elements 
		template <typename Def_t, typename N,typename T>
		auto do_end2(Def_t&& def, N&& n,T&& value) ->decltype(try_run_or_default_End(std::forward<Def_t>(def), n.n, std::forward<T>(value))){

			return try_run_or_default_End(std::forward<Def_t>(def), n.n, std::forward<T>(value));
		}
// 
// 		template <typename N>
// 		__ENABLE_IF(has_n<const N>(), procReturn) do_end2(const N& n) {
// 			return	do_end1(n.n);
// 
// 		}
// 
// 		template <typename N>
// 		__ENABLE_IF(has_n< N>(), procReturn) do_end2(N&& n) {
// 			return	do_end1(n.n);
// 
// 		}

		template <typename P, typename V, ARGGLIB__REQUIRES ( !ArggLib::is_future_type<V>::value ) >
		auto do_end3(P&& p, V&& v) ->decltype(do_end2(v, std::forward<P>(p), v)) {


			return  do_end2(v, std::forward<P>(p), v);
		}
#ifndef  __CLING__  
    namespace ArggLib_impl {
      template <typename P,typename V>
      class poorMansDotThen {
      public:
        ArggLib::remove_cvref_t<P> p;
        ArggLib::remove_cvref_t<V> v;
        poorMansDotThen(ArggLib::remove_cvref_t<P>  p_, ArggLib::remove_cvref_t<V> v_) :p(std::move(p_)), v(std::move(v_)) {}
        auto operator()()  {
          auto v2 = v.get(); 
          return  do_end2(v2, p, v2); 
        }
      };
    }

		template <typename P, typename V, ARGGLIB__REQUIRES (ArggLib::is_future_type<V>::value) >
		auto do_end3(P&& p, V&& v) ->decltype(std::async(ArggLib_impl::poorMansDotThen<P, V>{p, std::move(v)})){
			return std::async(ArggLib_impl::poorMansDotThen<P, V>{p, std::move(v)});
  	}
#endif // __CLING__
		template <typename... T1>
		auto do_end3a(T1&&... p) -> decltype(do_end2(std::forward<T1>(p)...)) {
			return  do_end2(std::forward<T1>(p)...);
		}
		template <typename P, typename V>
		auto do_end3a(P&& p, V&& v) -> decltype(v.wait(),v) {
			return v;
		}
/*
		template <typename P, typename V ,typename std::is_same< ArggLib::remove_cvref <V>::type , std::future >
		auto do_end3(P&& p, V&& v) {
			do_end2(p, v);
		}
		*/
    inline const procReturn& getDefaultReturn(){
      static const procReturn defaultReturn = success;
      return defaultReturn;
    }

		template <typename... P>
		auto unfold_end(P&&... p) -> decltype(do_end3(p..., getDefaultReturn())) {

			return do_end3(p..., getDefaultReturn());;
		}

		template <typename P, ARGGLIB__REQUIRES(_has_member_t_<P>::value)>
		auto  unfold_end(P&& p) 
#ifdef  __CLING__  // remove as soon as c++ 14 is avalible for cling
      ->decltype(do_end3(p, unfold_end(p.t)))
#endif //  __CLING__            
    {

			
			//auto ret = unfold_end(p.t);
			return  do_end3(p, unfold_end(p.t));
		}


		template <typename... P>
		auto unfold_end1(P&&... p) -> decltype(do_end3(p..., success)) {

			return do_end3(p..., success);;
		}

		template <typename P>
		auto  unfold_end1(P&& p) -> decltype(p.t, do_end3(p, unfold_end(p.t))) {


			auto ret = unfold_end1(p.t);
			return  do_end3(p, std::move(ret));
		}


// 
// 		template <typename P>
// 		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_end(const P& p) {
// 
// 			unfold_end(p.t);
// 			return do_end2(p);
// 		}
// 
// 
// 		template <typename P>
// 		__ENABLE_IF(has_t< P>(), procReturn)  unfold_end(P&& p) {
// 
// 			unfold_end(p.t);
// 			return do_end2(p);
// 		}






		template <typename N>
		procReturn do_Start1(N&& n) {
			return success;
		}

		template <typename N>
		__ENABLE_IF(hasStart<N>(), procReturn) do_Start1(N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart<const N>(), procReturn) do_Start1(const N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart< N>(), procReturn) do_Start1(N&& n) {
			return n.Start();

		}



		template <typename N>
		__ENABLE_IF_NOT(has_n<N>(), procReturn)  do_Start2(N&& n) {
			return do_Start1(n);
		}

		template <typename N>
		__ENABLE_IF(has_n<N>(), procReturn) do_Start2(N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n<const N>(), procReturn) do_Start2(const N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n< N>(), procReturn) do_Start2(N&& n) {
			return	do_Start1(n.n);

		}


		template <typename P>
		procReturn unfold_Start(P&& p) {
			do_Start2(p);
			return success;
		}

		template <typename P>
		__ENABLE_IF(has_t<P>(), procReturn)  unfold_Start(P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_Start(const P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t< P>(), procReturn)  unfold_Start(P&& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}

	}

}



#endif // ArggLib_impl_do_begin_do_end_h__
