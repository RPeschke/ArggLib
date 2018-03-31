#ifndef type_trates_h__
#define type_trates_h__
#include <type_traits>

namespace ArggLib {
	template< class T >
	struct remove_cvref {
		typedef std::remove_cv_t<std::remove_reference_t<T>> type;
	};
	template< class T >
	using remove_cvref_t = typename remove_cvref<T>::type;
}

#endif // type_trates_h__
