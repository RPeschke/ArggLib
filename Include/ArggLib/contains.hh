#ifndef contains_h__
#define contains_h__
#include  <algorithm>




namespace ArggLib {






	template <typename Container_t, typename T>
	bool contains(const Container_t& cont_v, const T& value) {
		return std::find(cont_v.begin(), cont_v.end(), value) != cont_v.end();
	}

  template <typename Container_t, typename T>
  bool contains_if(const Container_t& cont_v, T&& pred) {
    return std::find_if(cont_v.begin(), cont_v.end(), pred) != cont_v.end();
  }

}
#endif // contains_h__
