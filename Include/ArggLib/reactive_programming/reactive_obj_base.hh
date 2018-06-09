#ifndef reactive_obj_base_h__
#define reactive_obj_base_h__
#include "ArggLib/contains.hh"
namespace ArggLib {

	class reactive_obj_base {

	public:
		reactive_obj_base() : m_backend(&get_reactive_backend()) {

		}
		reactive_obj_base( reactive_backend* backend): m_backend(backend) {

		}
		reactive_obj_base& operator=(const reactive_obj_base& value) = delete;
		bool register_processor(reactive_processor_impl_c f) {
			auto contains_ = contains(m_notify, f);
			if (contains_) {
				return false;
			}

			m_notify.push_back(std::move(f));
			return true;
		}

	
		void notify() {
			m_backend->append(m_notify);
		}

	private:
		std::vector<reactive_processor_impl_c> m_notify;
		reactive_backend *m_backend;
	};
}
#endif // reactive_obj_base_h__
