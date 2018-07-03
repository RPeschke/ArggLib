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

			while ((m_running == running || !m_notify.empty()) && m_running != force_stopping) {

				std::unique_lock<std::mutex> lock(m);
				if (m_notify.empty()) cond_var.wait(lock);
				auto l_notify = m_notify;
				m_notify.clear();
				lock.unlock();
				for (auto& e : l_notify) {
					e->begin();
				}
				for (auto& e : l_notify) {
					e->process();
				}
				for (auto& e : l_notify) {
					e->end();
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
