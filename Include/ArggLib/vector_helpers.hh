#ifndef vector_helpers_h__
#define vector_helpers_h__
#include <algorithm>

namespace ArggLib {


template <typename Container_t, typename T>
auto contains(const Container_t& cont_v, const T& value){
  return std::find(cont_v.begin(), cont_v.end(), value) != cont_v.end();
}



//#define  ArggLib_sort_by(conteiner, proterty) [](auto hir) { std::sort(hir.begin(), hir.end(), [](cautor e1, cautor e2) {return  e1.proterty < e2.proterty; }); return hir; }(conteiner)

class sort_impl {
public:
template <typename Container_t, typename Predicate_t>
auto operator()(Container_t hir, Predicate_t F) {

  std::sort(hir.begin(), hir.end(), F);
  return hir;
}
};
#define  ArggLib_sort_by(property) fun() << sort_impl()<< _X << [](cautor e1, cautor e2) {return  e1.property < e2.property; }

template <typename T>
auto  to_vector_impl_convert_element(T&& t) {
  return t;

};


template <typename T1, typename T2>
auto  to_vector_impl_convert_element(std::pair<T1, T2>& t) {

  return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

};
template <typename T1, typename T2>
auto  to_vector_impl_convert_element(std::pair<T1, T2>&& t) {
  std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>> ret(t.first, t.second);
  return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

};

template <typename T1, typename T2>
auto  to_vector_impl_convert_element(const std::pair<T1, T2>& t) {

  return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

};


class to_vector_impl {
public:
  template <typename T>
  auto operator()(T& container_v) {
    std::vector<
        std::remove_const_t<
            std::remove_reference_t<
              decltype(to_vector_impl_convert_element(*container_v.begin()))>
                >
                > ret;
    for (auto e : container_v) {
      ret.push_back(to_vector_impl_convert_element(e));
    }

    return ret;
  }

  template <typename T, typename F_t>
  auto operator()(T& container_v, F_t to_vec) {
    std::vector<std::remove_const_t<decltype(to_vec(*container_v.begin()))>> ret;
    for (auto e : container_v) {
      ret.push_back(to_vec(e));
    }

    return ret;
  }
};

#define ArggLib_to_vector  ArggLib::fun() << ArggLib::to_vector_impl() << ArggLib::_X


}
#endif // vector_helpers_h__
