#ifndef ArggLibOnEnd2_h__
#define ArggLibOnEnd2_h__
namespace ArggLib {
  template<typename T>
  class OnEnd_impl2 {
  public:
    T m_fun;

    OnEnd_impl2(T fun) :m_fun(std::move(fun)) {

    }

    
    auto End() ->decltype(m_fun()) {

      return   m_fun();
    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {


      return next(std::forward<ARGS>(args)...);
    }


  };


  template <typename T>
  auto OnEnd2(T fun) ->decltype(proc() >> OnEnd_impl2<T>(std::move(fun))) {
    return proc() >> OnEnd_impl2<T>(std::move(fun));
  }


  template<typename T>
  class OnEnd_impl3 {
  public:
    T m_fun;

    OnEnd_impl3(T fun) :m_fun(std::move(fun)) {

    }

    template <typename... ARGS>
    auto End(ARGS&&... args) ->decltype(m_fun(args... )) {

      return   m_fun(args...);
    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {


      return next(std::forward<ARGS>(args)...);
    }


  };


  template <typename T>
  auto OnEnd3(T fun) ->decltype(proc() >> OnEnd_impl3<T>(std::move(fun))) {
    return proc() >> OnEnd_impl3<T>(std::move(fun));
  }

}
 #endif //ArggLibOnEnd2_h__