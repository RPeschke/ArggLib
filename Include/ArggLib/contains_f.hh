#ifndef contains_f_h__
#define contains_f_h__

#include "ArggLib/vector_helpers.hh"
#include "ArggLib/contains.hh"

namespace ArggLib {







	template < typename T>
	auto contains(const T& value) {
		return fun() << [value](const auto& cont_v) {return std::find(begin(cont_v), end(cont_v), value) != end(cont_v); };
	}


	template < typename F>
	auto contains_if(const F& value) {
		return fun() << [value](const auto& cont_v) {return std::find_if(begin(cont_v), end(cont_v), value) != end(cont_v); };
	}

	template < typename T>
	auto contains_any(std::initializer_list<T> l) {
		return fun() << [l](const auto& cont_v) {

			for (cautor e : cont_v) {
				if (contains(l, e)) {
					return true;
				}

			}

			return false;
		};
	}

	template < typename T>
	auto contains_all(std::initializer_list<T> l) {
		return fun() << [l](const auto& cont_v) {
			std::vector<T> vec(l);
			if (vec.empty()) {
				return false;
			}
			for (cautor e : cont_v) {

				if (contains(vec, e)) {
					remove_erase(vec, e);

				}
				if (vec.empty()) {
					return true;
				}

			}

			return false;
		};
	}




}
#endif // contains_f_h__