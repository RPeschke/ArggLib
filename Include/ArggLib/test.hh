#ifndef test_h__
#define test_h__


template <typename ReturnType> class clFunc0 {
  typedef ReturnType(*FuncPtr)();
public:
  typedef ReturnType Type;
};

template <typename ReturnType>
inline clFunc0<ReturnType> ResultType(ReturnType(*FuncPtr)()) {
  return clFunc0<ReturnType>();
}

#define FUNC_TYPE( func_name ) decltype( ResultType( &func_name ) )::Type

template <typename T, typename... ARGS>
T ____test(ARGS&&...);

template<typename T>
int  ___reset_test_TGraph(T&& g) {
  g.Set(0);
  g.Clear();
  return 1;
}

template<typename T>
auto enable_if_isTGraph_impl(T&& t)->decltype(____test<std::true_type>(std::declval<T>().GetN())) {
  return std::true_type();
}

template<typename... T>
std::false_type enable_if_isTGraph_impl(T&&... t) {
  return	std::false_type();
}


template <typename T>
auto __isTGraph() ->decltype(enable_if_isTGraph_impl(typename std::declval<T>())) {
  return enable_if_isTGraph_impl(T());
}


// template <typename T>
// struct __isTGraph_t{
// 	using type = decltype(ResultType(&__isTGraph<int>))::Type
// };

// template <typename T>
// int ___reset(T& g);
// 
// 
// template <typename T>
// std::enable_if< decltype(ResultType(typename &__isTGraph<T>))::Type, int>::type  ___reset(T& g) {
//   g.Set(0);
//   g.Clear();
// }



#endif // test_h__
