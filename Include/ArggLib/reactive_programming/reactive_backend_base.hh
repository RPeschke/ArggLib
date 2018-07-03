#ifndef reactive_backend_base_h__
#define reactive_backend_base_h__



#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/contains.hh"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>


namespace ArggLib {


	class reactive_backend_base {
	public:
		reactive_backend_base(std::function<void()> worker ) :m_worker(std::move(worker)) {

		}

		void push_back(reactive_processor_impl_c* f) {
			{

				std::unique_lock<std::mutex> lock(m);
				if (!contains(f))
				{
					m_notify.push_back(f);
				}
			}
			cond_var.notify_one();
		}
		void append( std::vector<reactive_processor_impl_c> &  app) {
			{

				std::unique_lock<std::mutex> lock(m);
				for(int i =0; i< app.size(); ++i){
					if (!contains( &app[i]))
					{
						m_notify.push_back(&app[i]);
					}
				}
			}
			cond_var.notify_one();

		}

		void append_ptr(const std::vector<reactive_processor_impl_c*> & app) {
			{
			std::unique_lock<std::mutex> lock(m);
			for (auto f : app) {
				
				if (!contains(f))
				{
					m_notify.push_back(f);
				}
			}
			}
		cond_var.notify_one();

	}
	void stop() {
		m_running = stopping;

		m_worker.join();

	}
	void join() {
		m_worker.join();
	}
	void set_force_stop() {
		m_running = force_stopping;
		cond_var.notify_one();
	}

	void force_stop() {
		m_running = force_stopping;
		cond_var.notify_one();
		m_worker.join();

	}
protected:
	enum state {
		running,
		stopping,
		force_stopping,
		stopped
	};
#ifndef  WIN32 
	state m_running = running;
#else
	std::atomic<state> m_running = running;
#endif
	std::condition_variable cond_var;
	std::mutex m;
	std::thread m_worker;
	std::vector<reactive_processor_impl_c*> m_notify;
	private:
		bool contains(reactive_processor_impl_c* f) {
			return std::find_if(m_notify.begin(), m_notify.end(), [&](reactive_processor_impl_c* e) { return e->ID() == f->ID(); }) != m_notify.end();
		}
};


}

#endif // reactive_backend_base_h__