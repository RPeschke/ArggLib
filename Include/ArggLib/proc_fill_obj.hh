#ifndef ArggLib_impl_proc_fill_obj_h___
#define ArggLib_impl_proc_fill_obj_h___
#include <type_traits>
#include <iostream>


namespace ArggLib {

	template <typename T>
	procReturn __helper_fill_obj(std::string& str, T&& t) {
		str += std::to_string(t);
		return success;
	}

	template <typename T>
	procReturn __helper_fill_obj(double& str, T&& t) {
		str += t;
		return success;
	}
	template <typename T>
	procReturn __helper_fill_obj(int& str, T&& t) {
		str += t;
		return success;
	}
	template <typename T>
	class proc_fill_obj {
	public:
		std::shared_ptr<T> m_value;

		proc_fill_obj(T t) :m_value(Snew T(t)) {}



		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) { 
		
			auto ret = __helper_fill_obj(*m_value, args...);
			if (ret != success) {
				return ret;
			}

			return next(std::forward<ARGS>(args)...);
		}
		T End() {
			
			return *m_value;
		}
	};


	proc_fill_obj<std::string> __helper_to_proc(const char* t)  {
		return proc_fill_obj<std::string>(t);
	}

	proc_fill_obj<double> __helper_to_proc(double  t) {
		return proc_fill_obj<double>(t);
	}
	proc_fill_obj<int> __helper_to_proc(int  t) {
		return proc_fill_obj<int>(t);
	}

}

#endif //ArggLib_impl_proc_fill_obj_h___