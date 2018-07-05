#ifndef reactive_processor_h__
#define reactive_processor_h__
#include <functional>

namespace ArggLib {
	inline int get_unique_id() {
		static int i = 0;
		return i++;
	}
	enum class react_proc_return: int {
		success,
		stop
	};
	using reactive_fun_t = std::function<react_proc_return()>;
	using reactive_fun_t_void = std::function<void()>;
	
	namespace ArggLib_impl {
		class reactive_fun_noop {
		public:
			inline react_proc_return operator()() {
				return react_proc_return::success;
			}
		};

		template <typename T, typename std::enable_if<std::is_same<typename std::result_of<T()>::type, void>::value, int>::type = 0>
		auto to_reactive_fun_t(T&& t) {
			return [t]() mutable {t(); return react_proc_return::success; };
		}
		template <typename T, typename std::enable_if<std::is_same<typename std::result_of<T()>::type, react_proc_return>::value, int>::type = 0>
		auto to_reactive_fun_t(T&& t)  {
			return std::forward<T>(t);
		}
	}

	class reactive_processor_impl_c {
		reactive_fun_t m_begin = ArggLib_impl::reactive_fun_noop();
		reactive_fun_t m_process = ArggLib_impl::reactive_fun_noop();
		reactive_fun_t m_end = ArggLib_impl::reactive_fun_noop();
		int m_ID;
	public:
		reactive_processor_impl_c() { m_ID = get_unique_id(); };
		template <typename F1, typename F2 ,typename F3>
		reactive_processor_impl_c(F1&& begin_f, F2&& process_f, F3&& end_f,int ID) : 
			m_begin( ArggLib_impl::to_reactive_fun_t( std::forward<F1>(begin_f))), 
			m_process(ArggLib_impl::to_reactive_fun_t(std::forward<F2>(process_f))),
			m_end(ArggLib_impl::to_reactive_fun_t(std::forward<F2>(end_f))),
			m_ID(ID)
		{}
		template <typename F2>
		reactive_processor_impl_c(F2&& process_f, int ID) :
			m_process(ArggLib_impl::to_reactive_fun_t(std::forward<F2>(process_f))),
			m_ID(ID)
		{}
		template <typename F>
		reactive_processor_impl_c& set_begin(F&& begin_f) {
			m_begin = ArggLib_impl::to_reactive_fun_t(std::forward<F>(begin_f));
			return *this;
		}
		template <typename F>
		reactive_processor_impl_c& set_process(F&& process_f) {
			m_process= ArggLib_impl::to_reactive_fun_t(std::forward<F>(process_f));
			return *this;
		}
		template <typename F>
		reactive_processor_impl_c& set_end(reactive_fun_t end_f) {
			m_end = ArggLib_impl::to_reactive_fun_t(std::forward<F>(end_f));
			return *this;
		}
		react_proc_return begin() {
			return m_begin();
		}
		react_proc_return process() {
			return m_process();
		}
		react_proc_return end() {
			return m_end();
		}
		int ID() const {
			return m_ID;
		}
	};

	inline bool operator==(const reactive_processor_impl_c& lhs, const reactive_processor_impl_c& rhs) {
		return lhs.ID() == rhs.ID();
	}

	template <typename T>
	reactive_processor_impl_c reactive_processor_1(T&& process_f) {
		return reactive_processor_impl_c(
			std::forward<T>(process_f),
			get_unique_id()
		);
	}
	


	template <typename T>
	reactive_processor_impl_c reactive_processor_2(T&& process_f,int ID) {
		return reactive_processor_impl_c(
			std::forward<T>(process_f),
			ID);
	}
	template <typename T>
	reactive_processor_impl_c reactive_processor_on_begin(T&& begin_f, int ID) {
		return reactive_processor_impl_c(
			std::forward<T>(begin_f), 
			ArggLib_impl::reactive_fun_noop(),
			ArggLib_impl::reactive_fun_noop(),
			ID);
	}
	template <typename T>
	reactive_processor_impl_c reactive_processor_on_begin(T&& begin_f) {
		return reactive_processor_impl_c(
			std::forward<T>(begin_f),
			ArggLib_impl::reactive_fun_noop(),
			ArggLib_impl::reactive_fun_noop(),
			get_unique_id()
		);
	}

	template <typename Func_t, typename T>
	void reactive_processor_impl(int ID,const Func_t& f, T& t) {

		t.register_processor(reactive_processor_2( f,ID ) );

	}
	template <typename Func_t, typename T, typename... ARGS_t>
	void reactive_processor_impl(int ID, const Func_t& f, T& t, ARGS_t&... args) {
		
		t.register_processor(reactive_processor_2(f,ID));
		reactive_processor_impl(ID, f, args...);
	 }

	template <typename Func_t,  typename... ARGS_t>
	void reactive_processor(const Func_t& f, ARGS_t&... args) {

		
		reactive_processor_impl( get_unique_id(), f, args...);
	}
}
#endif //reactive_processor_h__
