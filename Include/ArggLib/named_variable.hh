#ifndef ArggLibnamed_variable_h__
#define ArggLibnamed_variable_h__
#include <string>

namespace ArggLib {

  template <typename T>
  class named_variable {
  public:
    named_variable(std::string name_, T value_) :name(std::move(name_)), value(std::move(value_)) {}
    const std::string name;
    T value;
    using type_t = T;
  };

  template <typename T>
  std::ostream& operator<<(std::ostream& out, const named_variable<T>& t) {
    out << t.name<< ": {"<<  t.value << "}";
    return out;
  }

  template <typename T>
  std::istream& operator>>(std::istream& in,  named_variable<T>& t) {
    in >> t.value;
    return in;
  }

  template <typename T> 
  auto make_named_variable(std::string name, T value) ->decltype(named_variable<T>(std::move(name), std::move(value))) {
    return named_variable<T>(std::move(name), std::move(value));
  }

  using named_string = named_variable<std::string>;
  using named_int = named_variable<int>;
  using named_double = named_variable<double>;

}

#endif // ArggLibnamed_variable_h__
