#ifndef ArggLibadd_h__
#define ArggLibadd_h__
#include "ArggLib/procReturn.hh"


namespace ArggLib {



	template< int N, typename T>
	struct add_impl {};

	template<typename T>
	struct add_impl<0, T> {
		add_impl<0, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename... args>
		procReturn operator()(NEXT_T&& next, args... ar) const {
			return next(t, ar...);
		}
	};

	template<typename T>
	struct add_impl<-1, T> {
		add_impl<-1, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename... args>
		procReturn operator()(NEXT_T&& next, args... ar) const {
			return next(ar..., t);
		}
	};

	template<typename T>
	struct add_impl<1, T> {
		add_impl<1, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename T1, typename... args>
		procReturn operator()(NEXT_T&& next, T1&& t1, args... ar) const {
			return next(std::forward<T1>(t1), t, ar...);
		}
	};


	template<typename T>
	struct add_impl<2, T> {
		add_impl<2, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename T1, typename T2, typename... args>
		procReturn operator()(NEXT_T&& next, T1&& t1, T2&& t2, args... ar) const {
			return next(std::forward<T1>(t1), std::forward<T2>(t2), t, ar...);
		}
	};

	template<typename T>
	struct add_impl<3, T> {
		add_impl<3, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename T1, typename T2, typename T3, typename... args>
		procReturn operator()(NEXT_T&& next, T1&& t1, T2&& t2, T3&& t3, args... ar) const {
			return next(std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3), t, ar...);
		}
	};


	template<typename T>
	struct add_impl<4, T> {
		add_impl<4, T>(const T& t_) : t(t_) {}
		const T t;
		template <typename NEXT_T, typename T1, typename T2, typename T3, typename T4, typename... args>
		procReturn operator()(NEXT_T&& next, T1&& t1, T2&& t2, T3&& t3, T4&& t4, args... ar) const {
			return next(std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<T4>(t4), t, ar...);
		}
	};

	template<int N, typename T>
	add_impl<N, T> add(T&& t) {
		return add_impl<N, typename std::remove_all_extents<T>::type >(std::forward<T>(t));
	}










}

#endif // ArggLibadd_h__
