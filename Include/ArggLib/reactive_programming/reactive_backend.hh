#ifndef reactive_backend_h__
#define reactive_backend_h__

#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/reactive_programming/reactive_backend_base.hh"
#include "ArggLib/contains.hh"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>


namespace ArggLib {


	class reactive_backend :public reactive_backend_base {
	public:
		reactive_backend() :reactive_backend_base([this]() mutable {return this->run(); }) {

		}
		void run() {
			wait_for_init();

			while (check_status()) {

	
				for (auto& e : m_notify_local) {
					handle_return_value(e->begin());
				}
				for (auto& e : m_notify_local) {
					handle_return_value(e->process());

				}
				for (auto& e : m_notify_local) {
					handle_return_value(e->end());
				}
			}

			m_running = stopped;
		}
	
};

inline reactive_backend& get_reactive_backend() {
	static reactive_backend g_reactive_backend;
	return g_reactive_backend;
}

}

#endif // reactive_backend_h__
