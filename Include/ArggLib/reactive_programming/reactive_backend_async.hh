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
			wait_for_init();
			while (check_status()) {


				m_futures.clear();
				for (auto& e : m_notify_local) {
					m_futures.emplace_back(std::async([&e]() { return e->begin();  }));
				}
				handle_return_futures();
				for (auto& e : m_notify_local) {
					m_futures.emplace_back(std::async([&e]() { return e->process();  }));
				}
				handle_return_futures();

				for (auto& e : m_notify_local) {
					m_futures.emplace_back(std::async([&e]() {return e->end(); }));
				}
				handle_return_futures();
			}
			
			set_stopped();
		}

		std::vector<std::future<react_proc_return>> m_futures;
	private:
		void handle_return_futures() {
			for (auto&& e : m_futures) {
				handle_return_value(e.get());
			}
			m_futures.clear();
		}
};

inline reactive_backend_async& get_reactive_backend_async() {
	static reactive_backend_async g_reactive_backend;
	return g_reactive_backend;
}

}


#endif // reactive_backend_async_h__