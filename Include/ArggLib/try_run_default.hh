#ifndef ArggLib_try_run_default_h__
#define ArggLib_try_run_default_h__
#include <type_traits>
#define  __ARGGLIB_IMPL_TRY_FUNCTION_CASCADE(name,function_Name) \
      template <typename T, typename... N> \
      T&& return_first(T&& t, N&&... n) { \
        return t; \
      } template  <typename... N1>  auto try_call_fnction2(N1&&... n) -> decltype(return_first(n...))  {        return return_first(n...); } \
      template <typename default_T, typename Fun_t, typename ARG> auto  try_call_fnction2(default_T&& def, Fun_t&& fun, ARG && arg) -> decltype( function_Name()) { return function_Name(); }  \
      template <typename... N>  auto try_call_fnction3(N&&... n) -> decltype(try_call_fnction2(n...)) {  return try_call_fnction2(n...); } \
      template <typename default_T, typename Fun_t, typename ARG> auto  try_call_fnction3(default_T&& def, Fun_t&& fun, ARG && arg) -> decltype( function_Name(arg)) {  return function_Name(arg); } \
      auto operator()() -> decltype(try_call_fnction3(m_def, m_fun, m_arg))  {   return try_call_fnction3(m_def, m_fun, m_arg); } \
      class dummy_dont_use 
#define  __ARGGLIB_IMPL_TRY_VARIABLES(name, function_Name)       default_T1 m_def;  Fun_t1 m_fun;  ARG1 m_arg; 


#define __ARGGLIB_IMPL_TRY_CLASS(name, function_Name)   \
    template <typename default_T1, typename Fun_t1, typename ARG1> \
    struct name##_try_run_or_default_impl { \
      __ARGGLIB_IMPL_TRY_VARIABLES(name, function_Name); \
      __ARGGLIB_IMPL_TRY_FUNCTION_CASCADE(name, function_Name); \
    }

#define __ARGGLIB_IMPL_TRY_CLASS_maker_function(name, function_Name) \
        template <typename default_T, typename Fun_t, typename ARG> auto name##_make_types_try(default_T&& def, Fun_t&& fun, ARG && arg) ->decltype(name##_try_run_or_default_impl< default_T,   Fun_t,  ARG  >{def, fun, arg }) { return name##_try_run_or_default_impl<default_T,  Fun_t,  ARG  >{def, fun, arg }; }  class dummy_dont_use

#define __ARGGLIB_IMPL_TRY_CREATE_ENTRY_POINT(name, function_Name) \
    template <typename default_T, typename Fun_t, typename ARG> auto name(default_T&& def, Fun_t&& fun, ARG && arg) ->decltype( try_call_fnction4(ArggLib_impl::name##_make_types_try(def, fun, arg))) { \
      auto x13 = ArggLib_impl::name##_make_types_try(def, fun, arg); \
    return try_call_fnction4(x13); \
} class dummy_dont_use

#define __ARGGLIB_IMPL_TRY_CREATE_NEW(name, function_Name) \
  namespace ArggLib_impl { \
__ARGGLIB_IMPL_TRY_CLASS(name, function_Name); \
__ARGGLIB_IMPL_TRY_CLASS_maker_function(name, function_Name); \
} \
__ARGGLIB_IMPL_TRY_CREATE_ENTRY_POINT(name, function_Name)


namespace ArggLib {

  namespace ArggLib_impl {
    template <typename T, typename std::enable_if<std::is_same<typename std::result_of<T()>::type, void>::value, int>::type = 0 >
    auto try_call_fnction4(T&& t) ->decltype(t.m_def){
      t();
      return t.m_def;
    }

    template <typename T, typename std::enable_if<!std::is_same<typename std::result_of<T()>::type, void>::value, int>::type = 0 >
    auto try_call_fnction4(T&& t) ->decltype(t()){
      return t();
    }
  }

  __ARGGLIB_IMPL_TRY_CREATE_NEW(try_run_or_default_End, fun.End);
  __ARGGLIB_IMPL_TRY_CREATE_NEW(try_run_or_default_functor, fun);

}
#endif //ArggLib_try_run_default_h__