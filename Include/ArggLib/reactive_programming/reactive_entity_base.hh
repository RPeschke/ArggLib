#ifndef reactive_entity_base_h__
#define reactive_entity_base_h__
#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/reactive_programming/reactive_backend.hh"




namespace ArggLib {

	class reactive_entity_base;
	template<typename T = void>
	class reactive_entity_base_storage {
	public:
		inline	static reactive_entity_base* set_current_reactive_entity_base(reactive_entity_base* entity, bool set = true) {
			static reactive_entity_base* g_entity;
			if (set){
				g_entity = entity;
			}

			return g_entity;
		}

	};

	inline void set_current_reactive_entity_base(reactive_entity_base* entity) {
		reactive_entity_base_storage<>::set_current_reactive_entity_base(entity, true);
	}
	inline reactive_entity_base* get_current_reactive_entity_base() {
		return reactive_entity_base_storage<>::set_current_reactive_entity_base(nullptr, false);
	}
	class reactive_entity_base {
	public:
		reactive_entity_base() {
			set_current_reactive_entity_base(this);
		}
		reactive_entity_base(reactive_fun_t f) : m_process([]() {}, std::move(f), []() {}, get_unique_id()),m_backend(&get_reactive_backend()) {
			set_current_reactive_entity_base(this);
		}
		reactive_entity_base(reactive_fun_t f, reactive_backend_base * backend) : m_process([]() {}, std::move(f), []() {}, get_unique_id()), m_backend(backend) {
			set_current_reactive_entity_base(this);
		}
		reactive_backend_base& get_backend() {
			return *m_backend;
		}
		void stop_backend() {
			m_backend->set_force_stop();
		}
		reactive_processor_impl_c m_process;
		reactive_backend_base * m_backend;
	};


}



#endif // reactive_entity_base_h__
