#ifndef reactive_processor_h__
#define reactive_processor_h__
#include <functional>

namespace ArggLib {
	inline int get_unique_id() {
		static int i = 0;
		return i++;
	}
	using reactive_fun_t = std::function<void()>;
	
	class reactive_processor_impl_c {
		reactive_fun_t m_begin = []() {} ;
		reactive_fun_t m_process = []() {};
		reactive_fun_t m_end = []() {};
		int m_ID;
	public:
		reactive_processor_impl_c() {};
		reactive_processor_impl_c(reactive_fun_t begin_f, reactive_fun_t process_f, reactive_fun_t end_f,int ID) : 
			m_begin(std::move(begin_f)), 
			m_process(std::move(process_f)), 
			m_end(std::move(end_f)) ,
			m_ID(ID)
		{}
		reactive_processor_impl_c& set_begin(reactive_fun_t begin_f) {
			m_begin = std::move(begin_f);
		}
		reactive_processor_impl_c& set_end(reactive_fun_t end_f) {
			m_end = std::move(end_f);
		}
		void begin() {
			m_begin();
		}
		void process() {
			m_process();
		}
		void end() {
			m_end();
		}
		int ID() const {
			return m_ID;
		}
	};

	inline bool operator==(const reactive_processor_impl_c& lhs, const reactive_processor_impl_c& rhs) {
		return lhs.ID() == rhs.ID();
	}
	inline reactive_processor_impl_c reactive_processor_1(reactive_fun_t process_f) {
		return reactive_processor_impl_c([]() {}, std::move(process_f), []() {}, get_unique_id());
	}

	inline reactive_processor_impl_c reactive_processor_2(reactive_fun_t process_f,int ID) {
		return reactive_processor_impl_c([]() {}, std::move(process_f), []() {}, ID);
	}

	inline reactive_processor_impl_c reactive_processor_on_begin(reactive_fun_t begin_f, int ID) {
		return reactive_processor_impl_c(std::move(begin_f), []() {} , []() {}, ID);
	}
	inline reactive_processor_impl_c reactive_processor_on_begin(reactive_fun_t begin_f) {
		return reactive_processor_impl_c(std::move(begin_f), []() {}, []() {}, get_unique_id());
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
