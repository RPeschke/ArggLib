#ifndef ArggLibDrop_h__
#define ArggLibDrop_h__
#include "ArggLib/procReturn.hh"
#include "ArggLib/proc.hh"


namespace ArggLib {



	template<int N>
	struct drop {
		template <typename NEXT_T, typename... args>
		procReturn operator()(NEXT_T&& next, args&&... ar) const {

			static_assert(N < 0,"drop<n> with n < 5");
			
			auto dummy = drop <sizeof...(ar)+N>();
			
			static_assert(sizeof...(ar) >= -N, "sizeof...(ar)+N > 5");
			return dummy(next, std::forward<args>(ar)...);;
		}
	};

	template<>
	struct drop<0> {
		template <typename NEXT_T, typename T, typename... args>
			procReturn operator()(NEXT_T&& next, T&& , args&&... ar) const {

			return next(std::forward<args>(ar)...);
		}
	};



	template<>
	struct drop<1> {
		template <typename NEXT_T, typename T, typename T2, typename... args>
			procReturn operator()(NEXT_T&& next, T&& i, T2&&, args&&... ar) const {

			return next(std::forward<T>(i), std::forward<args>(ar)...);
		}
	};

	template<>
	struct drop<2> {
		template <typename NEXT_T, typename T, typename T2, typename T3, typename... args>
			procReturn operator()(NEXT_T&& next, T&& i, T2&& t2, T3&& t3, args&&... ar) const {

			return next(std::forward<T>(i), std::forward<T2>(t2), std::forward<args>(ar)...);
		}
	};

	template<>
	struct drop<3> {
		template <typename NEXT_T, typename T, typename T2, typename T3, typename T4, typename... args>
			procReturn operator()(NEXT_T&& next, T&& i, T2&& t2, T3&& t3, T4&& t4, args&&... ar) const {

			return next(std::forward<T>(i), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<args>(ar)...);
		}
	};


	template<>
	struct drop<4> {
		template <typename NEXT_T, typename T, typename T2, typename T3, typename T4, typename T5, typename... args>
			procReturn operator()(NEXT_T&& next, T&& i, T2&& t2, T3&& t3, T4&& t4, T5&& t5, args&&... ar) const {

			return next(std::forward<T>(i), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<T4>(t4), std::forward<args>(ar)...);
		}
	};


}


#endif // ArggLibDrop_h__
