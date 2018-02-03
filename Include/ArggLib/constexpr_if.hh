#ifndef constexpr_if_h__
#define constexpr_if_h__



#define __CONSTEXPR_IF(COND) ArggLib::_constexpr_if<COND>([&](auto) 
#define __CONSTEXPR_ELSE , [&](auto) 
#define __CONSTEXPR_ENDIF );
namespace ArggLib {

	template <typename C, typename True_Fun,typename False_fun>
	typename std::enable_if<C::value, True_Fun&&>::type select(True_Fun&& t, False_fun&&) {
		return std::forward<True_Fun>(t);
	}


	template <typename C, typename True_Fun, typename False_fun>
	typename std::enable_if<!C::value, False_fun&&>::type select(True_Fun&& , False_fun&& f) {
		return std::forward<False_fun>(f);
	}

	template <typename C, typename True_Fun, typename False_fun>
	void _constexpr_if(True_Fun&& t, False_fun&& f) {
	  return	select<C>(std::forward<True_Fun>(t), std::forward<False_fun>(f))(C::value);
	}

}

#endif // constexpr_if_h__
