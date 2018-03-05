#ifndef vector_helpers_impl_h__
#define vector_helpers_impl_h__


#include <algorithm>
#include "ArggLib/cautor.hh"



namespace ArggLib {
  namespace ArggLib_impl {

    template<typename T, typename T1>
    class make_vec_impl_2 {
    public:

      make_vec_impl_2(T t_, T1 t1_) :t(std::move(t_)), t1(std::move(t1_)) {}
      T t;
      T1 t1;
      template <typename T2>
      auto operator<(T2 t2) const {
        std::vector<T> ret;
        if (t1 > 0) {
          if (t > t1) {
            throw std::invalid_argument("Second Argument smaller then the First on");
          }
          for (auto i = t; i < t2; i = i + t1) {
            ret.push_back(i);

          }
        } else if (t1 < 0) {
          if (t < t1) {
            throw std::invalid_argument("Second Argument bigger then the First on");
          }
          for (auto i = t; i > t2; i = i + t1) {
            ret.push_back(i);

          }
        }  else {
          throw std::invalid_argument("Second Argument musst not be zero");
        }

        return ret;
      }
      template <typename T2>
      auto operator<=(T2 t2) const {
        std::vector<T> ret;
        if (t1 > 0) {
          if (t > t1) {
            throw std::invalid_argument("Second Argument smaller then the First on");
          }
          for (auto i = t; i <= t2; i = i + t1) {
            ret.push_back(i);

          }
        }
        else if (t1 < 0) {
          if (t < t1) {
            throw std::invalid_argument("Second Argument bigger then the First on");
          }
          for (auto i = t; i >= t2; i = i + t1) {
            ret.push_back(i);

          }
        }
        else {
          throw std::invalid_argument("Second Argument musst not be zero");
        }

        return ret;
      }
    };

    template<typename T>
    class make_vec_impl_1 {
    public:

      make_vec_impl_1(T t_) :t(std::move(t_)) {}
      T t;
      template <typename T1>
      auto operator<<(T1 t1) const {
        if (t > t1) {
          throw std::invalid_argument("Second Argument smaller then the First on");
        }
        std::vector<T> ret;

        for (auto i = t; i < t1; ++i) {
          ret.push_back(i);

        }

        return ret;
      }
      template <typename T1>
      auto operator<<=(T1 t1) const {
        if (t > t1) {
          throw std::invalid_argument("Second Argument smaller then the First on");
        }
        std::vector<T> ret;
        for (auto i = t; i <= t1; ++i) {
          ret.push_back(i);

        }

        return ret;
      }
      template <typename T1>
      auto operator<(T1 t1) const {

        return make_vec_impl_2<T, T1>(t, std::move(t1));
      }

    };

    //   auto operator<<(const __to_vector_0& in_, int t) {
    //     return make_vec_impl_1<double>(std::move(t));
    //   }
    //#define  ArggLib_sort_by(conteiner, proterty) [](auto hir) { std::sort(hir.begin(), hir.end(), [](cautor e1, cautor e2) {return  e1.proterty < e2.proterty; }); return hir; }(conteiner)

    class sort_impl {
    public:
      template <typename Container_t, typename Predicate_t>
      auto operator()(Container_t hir, Predicate_t F) {

        std::sort(hir.begin(), hir.end(), F);
        return hir;
      }
    };

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
  }


}


#endif // vector_helpers_impl_h__
