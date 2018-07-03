#ifndef reactive_obj_base_h__
#define reactive_obj_base_h__
#include "ArggLib/contains.hh"
#include "ArggLib/reactive_programming/reactive_backend_base.hh"


namespace ArggLib {

	class reactive_obj_base {

	public:
		reactive_obj_base() : m_backend(&get_reactive_backend()) {

		}
		reactive_obj_base(reactive_backend_base* backend): m_backend(backend) {

		}
		reactive_obj_base& operator=(const reactive_obj_base& value) = delete;
		bool register_processor(reactive_processor_impl_c f) {

			if (contains(&f)) {
				return false;
			}

			m_notify.push_back(std::move(f));
			return true;
		}

		bool register_processor_ptr(reactive_processor_impl_c* f) {
			
			if (contains(f)) {
				return false;
			}

			m_notify_ptr.push_back(f);
			return true;
		}
	
		void notify() {
			m_backend->append(m_notify);
			m_backend->append_ptr(m_notify_ptr);
		}

	private:
		bool contains(reactive_processor_impl_c* f) {
			auto contains_1 = std::find_if(m_notify_ptr.begin(), m_notify_ptr.end(), [&](reactive_processor_impl_c* e) { return e->ID() == f->ID(); }) != m_notify_ptr.end();
			auto contains_2 = std::find_if(m_notify.begin(), m_notify.end(), [&](reactive_processor_impl_c& e) { return e.ID() == f->ID(); }) != m_notify.end();
			return contains_2 || contains_1;
		}
		std::vector<reactive_processor_impl_c> m_notify;
		std::vector<reactive_processor_impl_c*> m_notify_ptr;
		reactive_backend_base *m_backend;
	};
}
#endif // reactive_obj_base_h__
