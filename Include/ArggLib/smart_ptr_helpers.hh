#ifndef smart_ptr_helpers_h__
#define smart_ptr_helpers_h__

#include <memory>
#define Unew ArggLib::ArggLib_impl::__unew_start << new 
#define Snew ArggLib::ArggLib_impl::__Snew_start << new 

namespace ArggLib {
	namespace ArggLib_impl {

		enum _unew_ {
			__unew_start
		};

		template <typename T>
		auto operator<<(_unew_, T* in_) {
			return std::unique_ptr<T>(in_);
		}



		enum _Snew_ {
			__Snew_start
		};

		template <typename T>
		auto operator<<(_Snew_, T* in_) {
			return std::shared_ptr<T>(in_);
		}
	}
}
#endif // smart_ptr_helpers_h__
