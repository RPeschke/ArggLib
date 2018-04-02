#ifndef validated_variable_h__
#define validated_variable_h__
#include  <functional>

namespace ArggLib {

  template <typename T, typename F>
  class validated_variable {
  public:
    validated_variable(T value, F fun) :m_value(std::move(value)), m_fun(std::move(fun)) {

      if (!fun(m_value)) {


        throw std::invalid_argument("Argument: '" + std::to_string(m_value) + "' is not valid as an Argument for validated_variable.");
      }
    }

    validated_variable(T value) :m_value(std::move(value)){

      if (!m_fun(m_value)) {


        throw std::invalid_argument("Argument: '" + std::to_string(m_value) + "' is not valid as an Argument for validated_variable.");
      }
    }

    operator T() {
      return m_value;
    }

    validated_variable<T, F>& operator=(T value) {
      if (!m_fun(value)) {


        throw std::invalid_argument("Argument: '" + std::to_string(value) + "' is not valid as an Argument for validated_variable.");
      }
      m_value = std::move(value);
      return *this;
    }
    F m_fun;
    T m_value;
  };


  template <typename T, typename F>
  auto make_validated_variable(T value, F fun) -> decltype(validated_variable<T, F>(std::move(value), std::move(fun))) {


    return validated_variable<T, F>(std::move(value), std::move(fun));
  }


  
  namespace ArggLib_impl {
    class positive_number {
    public:
      template <typename T>
      bool operator()(const T& e) {
        return e > 0;
      }
    };
    class negative_number {
    public:
      template <typename T>
      bool operator()(const T& e) {
        return e < 0;
      }
    };

    class not_negative {
    public:
      template <typename T>
      bool operator()(const T& e) {
        return e >= 0;
      }
    };
    class not_positive {
    public:
      template <typename T>
      bool operator()(const T& e) {
        return e <= 0;
      }
    };
    class any_number {
    public:
      template <typename T>
      bool operator()(const T& e) {
        return true;
      }
    };
  }

  template <typename T>
  auto make_validated_variable(T value)
    -> decltype(make_validated_variable(std::move(value), ArggLib_impl::any_number{}))
  {


    return make_validated_variable(std::move(value), ArggLib_impl::any_number{} );
  }


  using  double_positive = validated_variable<double, ArggLib_impl::positive_number>;
  using  double_negative = validated_variable<double, ArggLib_impl::negative_number>;
  using  double_not_negative = validated_variable<double, ArggLib_impl::not_negative>;
  using  double_not_positive = validated_variable<double, ArggLib_impl::not_positive>;
}

#endif // validated_variable_h__
