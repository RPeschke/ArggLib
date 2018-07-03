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
	}

	class reactive_processor_impl_c {
		reactive_fun_t m_begin = ArggLib_impl::reactive_fun_noop();
		reactive_fun_t m_process = ArggLib_impl::reactive_fun_noop();
		reactive_fun_t m_end = ArggLib_impl::reactive_fun_noop();
		int m_ID;
	public:
		reactive_processor_impl_c() {};
		reactive_processor_impl_c(reactive_fun_t begin_f, reactive_fun_t process_f, reactive_fun_t end_f,int ID) : 
			m_begin(std::move(begin_f)), 
			m_process(std::move(process_f)), 
			m_end(std::move(end_f)) ,
			m_ID(ID)
		{}
		reactive_processor_impl_c(reactive_fun_t process_f, int ID) :
			m_process(std::move(process_f)),
			m_ID(ID)
		{}
		reactive_processor_impl_c& set_begin(reactive_fun_t begin_f) {
			m_begin = std::move(begin_f);
		}
		reactive_processor_impl_c& set_end(reactive_fun_t end_f) {
			m_end = std::move(end_f);
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
	inline reactive_processor_impl_c reactive_processor_1(reactive_fun_t process_f) {
		return reactive_processor_impl_c(
			ArggLib_impl::reactive_fun_noop(), 
			std::move(process_f), 
			ArggLib_impl::reactive_fun_noop() , 
			get_unique_id()
		);
	}
	
	inline reactive_processor_impl_c reactive_processor_1(reactive_fun_t_void process_f) {
		return reactive_processor_impl_c(
			ArggLib_impl::reactive_fun_noop(),
			[process_f]() {process_f(); return react_proc_return::success; },
			ArggLib_impl::reactive_fun_noop(),
			get_unique_id()
		);
	}

	inline reactive_processor_impl_c reactive_processor_2(reactive_fun_t process_f,int ID) {
		return reactive_processor_impl_c(
			ArggLib_impl::reactive_fun_noop(),
			std::move(process_f),
			ArggLib_impl::reactive_fun_noop(),
			ID);
	}
	inline reactive_processor_impl_c reactive_processor_2(reactive_fun_t_void process_f, int ID) {
		return reactive_processor_impl_c(
			ArggLib_impl::reactive_fun_noop(),
			[process_f]() { process_f(); return react_proc_return::success; },
			ArggLib_impl::reactive_fun_noop(),
			ID);
	}
	inline reactive_processor_impl_c reactive_processor_on_begin(reactive_fun_t begin_f, int ID) {
		return reactive_processor_impl_c(
			std::move(begin_f), 
			ArggLib_impl::reactive_fun_noop(),
			ArggLib_impl::reactive_fun_noop(),
			ID);
	}
	inline reactive_processor_impl_c reactive_processor_on_begin(reactive_fun_t begin_f) {
		return reactive_processor_impl_c(
			std::move(begin_f),
			ArggLib_impl::reactive_fun_noop(),
			ArggLib_impl::reactive_fun_noop(),
			get_unique_id()
		);
	}
	inline reactive_processor_impl_c reactive_processor_on_begin(reactive_fun_t_void begin_f) {
		return reactive_processor_impl_c(
			[begin_f]() {begin_f(); return react_proc_return::success; },
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
