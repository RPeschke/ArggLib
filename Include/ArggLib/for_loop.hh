#ifndef for_loop_h__
#define for_loop_h__
#include "ArggLib/has_member_helper.hh"
#include "ArggLib/proc_tools.hh"
#include "ArggLib/string_helpers.hh"
#include "ArggLib/smart_ptr_helpers.hh"
#include "ArggLib/type_trates.hh"

namespace ArggLib {


  class for_loop_imple_0 {

  public:
    template <typename NEXT_T, typename T>
    __ENABLE_IF_ARITHMETIC(T, procReturn)
      operator()(NEXT_T&& next, T t) {
      for (typename ArggLib::remove_cvref_t<T> i = 0; i < t; ++i) {
        if (next(i) == stop_) {
          return stop_;
        }
      }
      return success;
    }

    template <typename NEXT_T, typename T>
    __ENABLE_IF(has_begin<T>(), procReturn) operator()(NEXT_T&& next, T& vec) {
      for (auto& e : vec) {
        if (next(e) == stop_) {
          return stop_;
        }
      }

      return success;
    }
    template <typename NEXT_T, typename T1, typename T2>
    procReturn  operator()(NEXT_T&& next, T1 start_, T2 end_) {
      for (_IS_BOTH_INT_OR_DEFAULT_DOUBLE(T1, T2) i = start_; i < end_; ++i) {
        if (next(i) == stop_) {
          return stop_;
        }
      }

      return success;
    }






    template <typename NEXT_T, typename T1, typename T2, typename T3>
    procReturn operator()(NEXT_T&& next, T1 start_, T2 end_, T3 step) {

      for (_IS_ALL_INT_OR_DEFAULT_DOUBLE(T1, T2, T3) i = start_; i < end_; i += step) {
        if (next(i) == stop_) {
          return stop_;
        }
      }

      return success;
    }
  };
  inline	auto for_loop() -> decltype(proc() >> for_loop_imple_0{}){
    return proc() >> for_loop_imple_0{};
  }


  template<typename END_T>
  class for_loop_imple_1 {
    using param_t = typename ArggLib::remove_cvref_t<END_T>;
  public:

    const param_t m_end;
    for_loop_imple_1<END_T>(const param_t& end__) : m_end(end__) {}
    for_loop_imple_1<END_T>(param_t&& end__) : m_end(std::move(end__)) {}
    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (param_t i = 0; i < m_end; ++i) {
        if (next(std::forward<ARGS>(args)..., i) == stop_) {
          return stop_;
        }
      }

      return success;
    }


  };

  template <typename T, __ENABLE_IF_ARITHMETIC(T, int) = 0>
  auto for_loop(T&& t) -> decltype(proc() >> for_loop_imple_1<T>(std::forward<T>(t))) {
    return proc() >> for_loop_imple_1<T>(std::forward<T>(t));
  }


  template<typename VEC_T>
  class for_loop_imple_1_vec_lv {

  public:
    VEC_T m_vec;
    for_loop_imple_1_vec_lv<VEC_T>(const VEC_T& vec__) : m_vec(vec__) {}
    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (auto &i : m_vec) {
        if (next(std::forward<ARGS>(args)..., i) == stop_) {
          return stop_;
        }
      }

      return success;
    }
  };

  template <typename T, __ENABLE_IF(has_begin<T>(), int) = 0>
  auto for_loop(T&& t) -> decltype (proc() >> for_loop_imple_1_vec_lv<T>(std::move(t)) ){
    return proc() >> for_loop_imple_1_vec_lv<T>(std::move(t));
  }

  template<typename T>
  class for_loop_imple_2 {
    using param_t = typename std::remove_all_extents<T>::type;
  public:
    const param_t m_end;
    const param_t m_start;
    for_loop_imple_2<T>(const T& start__, const T& end__) : m_start(start__), m_end(end__) {}
    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (param_t i = m_start; i < m_end; ++i) {
        if (next(std::forward<ARGS>(args)..., i) == stop_) {
          return stop_;
        }
      }

      return success;
    }


  };



  template <typename T1, typename T2>
  auto  for_loop(T1&& start_, T2&& end_) ->decltype(proc() >> for_loop_imple_2<typename ___IS_BOTH_INT<typename ArggLib::remove_cvref_t<T1>, typename ArggLib::remove_cvref_t<T2>, double>::type>(std::forward<T1>(start_), std::forward<T2>(end_))) {
    return proc()>> for_loop_imple_2<typename ___IS_BOTH_INT<typename ArggLib::remove_cvref_t<T1>, typename ArggLib::remove_cvref_t<T2>, double>::type>(std::forward<T1>(start_), std::forward<T2>(end_));
  }


  template<typename T>
  class for_loop_imple_3 {
    using param_t = typename std::remove_all_extents<T>::type;
  public:
    const param_t m_end;
    const param_t m_start;
    const param_t m_step;
    for_loop_imple_3<T>(const T& start__, const T& end__, const T& step_) : m_start(start__), m_end(end__), m_step(step_) {}
    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (param_t i = m_start; i < m_end; i += m_step) {
        if (next(std::forward<ARGS>(args)..., i) == stop_) {
          return stop_;
        }
      }

      return success;
    }


  };

  template <typename T1, typename T2, typename T3>
  auto for_loop(T1&& start_, T2&& end_, T3&& step_) -> decltype(proc() >> for_loop_imple_3<typename ___IS_ALL_INT<T1, T2, T3, double>::type>(std::forward<T1>(start_), std::forward<T2>(end_), std::forward<T3>(step_))) {
    return proc()>> for_loop_imple_3<typename ___IS_ALL_INT<T1, T2, T3, double>::type>(std::forward<T1>(start_), std::forward<T2>(end_), std::forward<T3>(step_));
  }


  template<typename T>
  class for_loop_impl_vec {
  public:
    const std::vector<T>& m_vec;
    for_loop_impl_vec(const std::vector<T>& vec) :m_vec(vec) {

    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (auto& e : m_vec) {
        auto ret = next(args..., e);
        if (ret != success) {
          return ret;
        }
      }

      return success;
    }


  };


  template<typename T>
  class for_loop_impl_vec_RV {
  public:
    const std::vector<T> m_vec;
    for_loop_impl_vec_RV(std::vector<T>&& vec) :m_vec(std::move(vec)) {

    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {
      for (auto& e : m_vec) {
        auto ret = next(args..., e);
        if (ret != success) {
          return ret;
        }
      }

      return success;
    }


  };

  
  template <typename T>
  auto  for_loop(const std::vector<T>& vec) -> decltype(proc() >> for_loop_impl_vec<T>(vec)) {
    return proc() >> for_loop_impl_vec<T>(vec);
  }

  template <typename T>
  auto  for_loop(std::vector<T>& vec) -> decltype(proc() >> for_loop_impl_vec<T>(vec)) {
    return proc() >> for_loop_impl_vec<T>(vec);
  }

  template <typename T>
  auto  for_loop(std::vector<T>&& vec) -> decltype(proc() >> for_loop_impl_vec_RV<T>(std::move(vec))) {
    return proc() >> for_loop_impl_vec_RV<T>(std::move(vec));
  }
  
  template <typename T>
  auto  for_loop(std::initializer_list<T> l) -> decltype(for_loop(std::vector<T>(std::move(l)))) {
    
    return for_loop(std::vector<T>(std::move(l)));
  }
}


#endif // for_loop_h__
