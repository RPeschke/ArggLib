#ifndef type_trates_h__
#define type_trates_h__
#include <type_traits>
#include <future>
#include <utility>
#define  ARGGLIB__REQUIRES(X) typename std::enable_if < X , int > ::type = 0
namespace ArggLib {
  template< class T > struct remove_reference { typedef T type; };
  template< class T > struct remove_reference<T&> { typedef T type; };
  template< class T > struct remove_reference<T&&> { typedef T type; };
  template< class T >
  using remove_reference_t = typename remove_reference<T>::type;



  template< class T > struct remove_const { typedef T type; };
  template< class T > struct remove_const<const T> { typedef T type; };
  template< class T >
  using remove_const_t = typename remove_const<T>::type;

  template< class T > struct remove_volatile { typedef T type; };
  template< class T > struct remove_volatile<volatile T> { typedef T type; };
  template< class T >
  using remove_volatile_t = typename remove_volatile<T>::type;


  template< class T >
  struct remove_cv {
    typedef typename ArggLib::remove_volatile<typename ArggLib::remove_const<T>::type>::type type;
  };
  template< class T >
  using remove_cv_t = typename remove_cv<T>::type;

	template< class T >
	struct remove_cvref {
		typedef ArggLib::remove_cv_t<ArggLib::remove_reference_t<T>> type;
	};
	template< class T >
	using remove_cvref_t = typename remove_cvref<T>::type;




  template <typename T>
  std::false_type is_future(T&&);

  template <typename T>
  std::true_type is_future(std::future<T>&&);
  template <typename T>
  std::true_type is_future(std::future<T>&);

  template <typename T>
  std::true_type is_future(const std::future<T>&);

	template <typename T>
	class is_future_type {
	public:
		enum { value = decltype(is_future(std::declval<	T>()))::value };
	};


  template <typename... T>
  std::false_type __has_member_t(T&&... );

  template <typename T>
  auto __has_member_t(T&& t) -> decltype(t.t, std::true_type{});

  template <typename T>
  class _has_member_t_ {
  public:
    enum { value = decltype(__has_member_t(std::declval<	T>()))::value };
  };



    template <typename T>
    class has_custom_to_proc_function {
    public:
      enum { value = 0 };
    };


}

#endif // type_trates_h__
