#ifndef l_array_h__
#define l_array_h__



template<typename T, typename Internal_itt_t>
class l_array;

template <typename T, typename Internal_itt_t>
class l_itt {
public:
  using type = typename T;
  using itter_t = typename Internal_itt_t;

  l_itt() :m_end(true) {}
  l_itt(const T& start_,
    const Internal_itt_t& start_itt,
    std::function<void(T&, itter_t& it)> inc,
    std::function<bool(const T&, const itter_t& it)> eq,
    l_array<T, itter_t> * array__
  ) :
    i(start_),
    it(start_itt),
    m_inc(std::move(inc)),
    m_eq(std::move(eq)),
    m_end(false),
    m_array(array__) {

  }
  T& operator*() {
    return i;
  }
  const T& operator*()const {
    return i;
  }




  bool operator!=(const l_itt<T, itter_t>& rhs) const {
    if (rhs.m_end) {
      return m_eq(i, it);
    }

    return it != rhs.it;
  }

  bool operator==(const l_itt<T, itter_t>& rhs)const {
    return !(*this != rhs);
  }

  l_itt& operator++() {
    ++it;
    if (it == m_array->m_current->it && m_array->m_current != this) {
      i = *(*m_array->m_current);
    } else {
      m_inc(i, it);
      m_array->m_current = this;
    }
    return *this;
  }

  const bool m_end = false;
  itter_t it;
  T i;
  l_array<T, itter_t> * m_array;
  std::function<void(T&, itter_t& it)> m_inc = [](auto&, auto&) {};
  std::function<bool(const T&, const itter_t& it)> m_eq = [](const auto&, const auto&) {return true;};
};

template<typename T, typename Internal_itt_t>
class l_array {
public:
  using type = typename T;


  l_array(const T& start_,
    Internal_itt_t start_itt_,
    const std::function<void(T&, Internal_itt_t&)>& inc,
    const  std::function<bool(const T&, const Internal_itt_t&)>& eq)
    : m_start(start_, start_itt_, inc, eq, this),
    m_current(&m_start) {
  }




  const l_itt<T, Internal_itt_t> m_start;
  const l_itt<T, Internal_itt_t>* m_current;

};

template <typename T, typename Internal_itt_t>
l_itt<T, Internal_itt_t> begin(const l_array<T, Internal_itt_t>& l_a) {
  return l_a.m_start;
}


template <typename T, typename Internal_itt_t>
auto end(const l_array<T, Internal_itt_t>& l_a) {
  return l_itt<T, Internal_itt_t>();
}

template<typename T>
auto make_l_sequence(T&& start__, T&& end__, T&& step__) {

  return l_array<T, int>(start__, 0, [step__](auto& i, auto& it) {  i += step__; }, [end__](const auto& i, const auto& it) {return i < end__; });
}




template<typename T, typename itt_t, typename F>
auto transform_l_sequence(const l_array<T, itt_t>& arr, F&& f) {

  return l_array<T, l_itt<T, itt_t> >(
    f(*arr.m_start), begin(arr),
    [f](auto& i, auto& it) {
    i = f(*it);
  },
    [end_ = end(arr)](const auto& i, const auto& it) {
    return it != end_;
  }
  );
}
#endif // l_array_h__
