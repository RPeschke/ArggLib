#ifndef ArggLib_impl_async_proc_h__
#define ArggLib_impl_async_proc_h__
#include <future>
#include <thread>
#include <type_traits>
#include "ArggLib/param.hh"
#include "ArggLib/complete_lib.hh"


namespace ArggLib {
	template <typename NEXT_T,typename Args_T>
	class async_functor {
	public:

		NEXT_T next;
		Args_T args;

		async_functor(const NEXT_T& next_, const Args_T&  args_) :next(  next_),  args(args_ ) {
		
		}
		auto operator()() {
			return ___ArggLib_Apply(next, args);
			
		}
	};

	template <typename NEXT_T, typename Args_T>
	auto make_async_functor(NEXT_T&& n, Args_T&& arg) {
		return async_functor< ArggLib::remove_cvref_t<NEXT_T>, ArggLib::remove_cvref_t<Args_T>>{std::forward<NEXT_T>( n) , std::forward<Args_T>(  arg )};
	}


	class proc_transperant {
	public:
		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {
			return next(std::forward<ARGS>(args));
		}
	};

	class proc_async_impl {
	public: 

		std::shared_ptr<std::packaged_task<procReturn()>> m_task;
		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

			auto asc =  make_async_functor(deep_copy_innerLamda(next), std::make_tuple(args...));
			m_task = Snew std::packaged_task<procReturn()> ( std::move(asc));

			return success			;
		}
		auto End() {
			auto result = m_task->get_future();  // get a future
			std::thread(std::move(*m_task)).detach(); // launch on a thread
			return result;
		}
	};

	auto proc_async() {
		return ArggLib:: proc() >> proc_async_impl();
	}



}


#endif  //ArggLib_impl_async_proc_h__