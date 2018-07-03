#ifndef reactive_backend_async_h__
#define reactive_backend_async_h__




#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/reactive_programming/reactive_backend_base.hh"
#include "ArggLib/contains.hh"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>


namespace ArggLib {


	class reactive_backend_async :public reactive_backend_base {
	public:
		reactive_backend_async() :reactive_backend_base([this]() mutable {return this->run(); }) {

		}
		void run() {

			while ((m_running == running || !m_notify.empty()) && m_running != force_stopping) {

				std::unique_lock<std::mutex> lock(m);
				if (m_notify.empty()) cond_var.wait(lock);
				auto l_notify = m_notify;
				m_notify.clear();
				lock.unlock();
				m_futures.clear();
				for (auto& e : l_notify) {
					m_futures.emplace_back(std::async([&e]() { e->begin();  }));
				}
				m_futures.clear();
				for (auto& e : l_notify) {
					m_futures.emplace_back(std::async([&e]() { e->process();  }));
				}

				m_futures.clear();

				for (auto& e : l_notify) {
					m_futures.emplace_back(std::async([&e]() { e->end(); }));
				}
				m_futures.clear();
			}
			
			m_running = stopped;
		}

		std::vector<std::future<void>> m_futures;
	
};

inline reactive_backend_async& get_reactive_backend_async() {
	static reactive_backend_async g_reactive_backend;
	return g_reactive_backend;
}

}


#endif // reactive_backend_async_h__