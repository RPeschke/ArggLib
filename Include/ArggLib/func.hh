#ifndef func_h__
#define func_h__
#include <vector>
#include <numeric>
#include <tuple>


template<size_t N>
struct Apply {
  template<typename F, typename T, typename... A>
  static inline auto apply(F && f, T && t, A &&... a) {
    return Apply<N - 1>::apply(::std::forward<F>(f), ::std::forward<T>(t),
      ::std::get<N - 1>(::std::forward<T>(t)), ::std::forward<A>(a)...
    );
  }
};

template<>
struct Apply<0> {
  template<typename F, typename T, typename... A>
  static inline auto apply(F && f, T &&, A &&... a) {
    return ::std::forward<F>(f)(::std::forward<A>(a)...);
  }
};

template<typename F, typename T>
inline auto apply(F && f, T && t) {
  return Apply< ::std::tuple_size< ::std::decay_t<T>
  >::value>::apply(::std::forward<F>(f), ::std::forward<T>(t));
}

std::vector<int> get_ints2(int  in) {

  std::vector<int> ret;
  for (int i = 0; i < in;++i) {
    ret.push_back(i);
  }

  return ret;
}

std::vector<int> get_ints3(int  start_, int end_,int step =1 ) {

  std::vector<int> ret;
  for (int i = start_; i < end_;i+=step) {
    ret.push_back(i);
  }

  return ret;
}


int sum_ints(const std::vector<int>& vec) {

  return std::accumulate(vec.begin(), vec.end(), 0);


}


class make_vec {
public:


  template<typename T>
  auto operator()(T&& t1, T&& t2,T&& t3) {
    std::vector<double> ret;
    for (double i = t1; i < t2;i+=t3) {
      ret.push_back(i);
    }

    return ret;
  }
};


template <typename T>
auto getvec4(T t) {
  vector<T> ret;
  for (T i = 0.0; i < t;++i) {
    ret.push_back(i);
  }

  return ret;
}


class sum_all {
public:
  template <typename T>
  auto operator()(T&& t) const {
    return accumulate(t.begin(), t.end(), T::value_type(0));
  }
  template <typename T1,typename T2>
  auto operator()(T1&& t,T2 && start_) const {
    return accumulate(t.begin(), t.end(), start_);
  }
};
template <typename T>
auto make_param_impl(T&& t) ->decltype(param_impl<T>(std::forward<T>(t)));

template <typename T>
class param_impl {
public:
  typename std::remove_reference<T>::type m_params;


  param_impl(const T& param_):m_params(param_) {

  }
  template<typename T1>
  auto operator<< (T1&& t) {
    return make_param_impl(std::tuple_cat(m_params, std::make_tuple(t)));
  }

};
template <typename T>
auto make_param_impl(T&& t) {
  return param_impl<T>(std::forward<T>(t));
}
class param {
public:
  template<typename T>
  auto operator<< (T&& t) {
    return make_param_impl(std::make_tuple(t));
  }
};

template <size_t N, typename T>
auto make_fun_impl(T&& t);


class fun {
public:
  template<typename T>
  auto operator* (T&& t) {
    return make_fun_impl<0>(std::forward<T>(t));
  }
  template<typename T>
  auto operator> (T&& t) {
    return make_fun_impl<0>(std::forward<T>(t));
  }

  template<typename T>
  auto operator<< (T&& t) {
    return make_fun_impl<0>(std::forward<T>(t));
  }
};


class _X {

};

class _run {

};
template <typename T, std::size_t N>
class fun_impl_b {
public:
  fun_impl_b(T&& t) :m_function(std::forward<T>(t)) {}

  template<typename... Param>
  auto operator()(Param&&... p) {
    return m_function(p...);
  }
  T m_function;

  template<typename T>
  auto operator > (T&& t) {
    return make_fun_impl<0>([=](auto&&... s) mutable{ return t(m_function(s...)); });
  }




  auto operator<<= (const _run& t) {
    return m_function();
  }
  auto operator<<= (_run& t) {
    return m_function();
  }
  auto operator<<= (_run&& t) {
    return m_function();
  }

  auto operator<= (const _run& t) {
    return m_function();
  }
  auto operator<= (_run& t) {
    return m_function();
  }
  auto operator<= (_run&& t) {
    return m_function();
  }

  auto operator<< ( const _X& t) {
    return make_fun_impl<N + 1>(m_function);
  }

};


template <typename T,std::size_t N>
class fun_impl_d: public fun_impl_b<T,N> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T,N>(std::forward<T>(t)) {
    static_assert(false, "Dummy function not implemented");
  
  }

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<N>([=](auto&&... s) { return m_function(t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<N + 1>(m_function);
  }
};


template <typename T>
class fun_impl_d<T, 1> : public fun_impl_b<T, 1> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T, 1>(std::forward<T>(t)) {}

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<1>([=](auto&& x1, auto&&... s) { return m_function(x1,t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<2>(m_function);
  }
};


template <typename T>
class fun_impl_d<T, 2> : public fun_impl_b<T, 2> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T, 2>(std::forward<T>(t)) {}

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<2>([=](auto&& x1, auto&& x2, auto&&... s) { return m_function(x1,x2, t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<3>(m_function);
  }
};


template <typename T>
class fun_impl_d<T, 3> : public fun_impl_b<T, 3> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T, 3>(std::forward<T>(t)) {}

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<3>([=](auto&& x1, auto&& x2, auto&& x3, auto&&... s) { return m_function(x1, x2,x3, t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<4>(m_function);
  }
};

template <typename T>
class fun_impl_d<T, 4> : public fun_impl_b<T, 4> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T, 4>(std::forward<T>(t)) {}

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<4>([=](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&&... s) { return m_function(x1, x2, x3,x4, t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<5>(m_function);
  }
};

template <typename T>
class fun_impl_d<T, 0> : public fun_impl_b<T, 0> {
public:
  fun_impl_d(T&& t) :fun_impl_b<T, 0>(std::forward<T>(t)) {}

  template<typename T>
  auto operator << (T&& t) {
    return make_fun_impl<0>([=](auto&&... s) { return m_function(t, s...); });
  }
  template<>
  auto operator<< <_X> (_X&& t) {
    return make_fun_impl<1>(m_function);
  }
};

class _void {
public:
  template < typename T, std::size_t N>
  auto operator>>=(fun_impl_d<T, N>& f) {
    return f();
  }

  template < typename T, std::size_t N>
  auto operator>>=(fun_impl_d<T, N>&& f) {
    return f();
  }
  template <typename T, std::size_t N>
  auto operator>>=(const  fun_impl_d<T, N>& f) {
    return f();
  }
};


template <typename IN, typename T, std::size_t N>
auto operator|(IN&& in_, fun_impl_d<T,N>& f) {
  return f(std::forward<IN>(in_));
}

template <typename IN, typename T, std::size_t N>
auto operator|(IN&& in_, fun_impl_d<T, N>&& f) {
  return f(std::forward<IN>(in_));
}
template <typename IN, typename T, std::size_t N>
auto operator|(IN&& in_,const  fun_impl_d<T, N>& f) {
  return f(std::forward<IN>(in_));
}

template <typename IN, typename T, std::size_t N>
auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>& f) {
  return  apply(f, in_.m_params);
}
template <typename IN, typename T, std::size_t N>
auto operator|(param_impl<IN>&& in_, fun_impl_d<T, N>&& f) {
  return  apply(f, in_.m_params);
}

template <typename IN, typename T, std::size_t N>
auto operator|(param_impl<IN>&& in_,const  fun_impl_d<T, N>& f) {
  return  apply(f, in_.m_params);
}
template <typename IN, typename T, std::size_t N>
auto operator|(const param_impl<IN>& in_, fun_impl_d<T, N>& f) {
  return  apply(f, in_.m_params);
}









template <typename IN, typename T, std::size_t N>
auto operator|(param_impl<IN>& in_, fun_impl_d<T, N>& f) {
  
  return apply(f, in_.m_params);
}

template <std::size_t N, typename T>
auto make_fun_impl(T&& t) {
  return fun_impl_d<T,N>(std::forward<T>(t));
}
#endif // func_h__
