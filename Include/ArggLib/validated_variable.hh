#ifndef validated_variable_h__
#define validated_variable_h__


namespace ArggLib {

  template <typename T,typename F> 
  class validated_variable {
  public:
    validated_variable(T value, F fun ):m_value(std::move(value)), m_fun(std::move(fun)) {
    
      if (!fun(m_value)) {


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
  auto make_validated_variable(T value, F fun ) {


    return validated_variable<T, F>(std::move(value), std::move(fun));
  }

  template <typename T>
  auto make_validated_variable(T value) {


    return make_validated_variable(std::move(value), [](auto) {return true; });
  }

}

#endif // validated_variable_h__
