#ifndef reverse_h__
#define reverse_h__


namespace ArggLib {
	// -------------------------------------------------------------------
	// --- Reversed iterable

	template <typename T>
	struct reversion_wrapper { T& iterable; };

	template <typename T>
#ifdef _WIN32
	auto begin(reversion_wrapper<T> w) { return std::rbegin(w.iterable); }
#else 
  auto begin(reversion_wrapper<T> w) ->decltype(w.iterable.rbegin()) { return w.iterable.rbegin(); }
#endif // _WIN32


	template <typename T>
#ifdef _WIN32
	auto end(reversion_wrapper<T> w) { return std::rend(w.iterable); }
#else 
auto end(reversion_wrapper<T> w) ->decltype(w.iterable.rend()) { return w.iterable.rend(); }
#endif // _WIN32
	template <typename T>
	reversion_wrapper<T> reverse(T&& iterable) { return{ iterable }; }

}
#endif // reverse_h__
