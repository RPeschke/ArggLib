#ifndef reactive_backend_base_h__
#define reactive_backend_base_h__



#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/contains.hh"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>


namespace ArggLib {


	class reactive_backend_base {
	public:
		reactive_backend_base(std::function<void()> worker ) :m_worker(std::move(worker)) {
			

		}
		void init() {
			m_running = starting1;
			while (m_running==starting1)
			{
			}
		}
		void wait_for_init() {
			while (m_running == starting0){

			}
			m_running = running;
			
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
	bool check_status() {


		if (m_running == force_stopping1) {
			return false;
		}
		if (m_running == force_stopping){
			m_running = force_stopping1;
			return false;
		}

		if ((m_running == running || !m_notify.empty() )) {
			m_notify_local.clear();
			std::unique_lock<std::mutex> lock(m);

			if (m_notify.empty()) cond_var.wait(lock);
			m_notify_local = m_notify;
			m_notify.clear();
			lock.unlock();
			return true;
		}


		return false;
	}
	void stop() {
		m_running = stopping;
		cond_var.notify_one();
		m_worker.join();

	}
	void join() {
		cond_var.notify_one();
		m_worker.join();
	}
	bool set_force_stop() {
		if (has_finished) {
			return true;
		}
		if (m_running == stopped) {
			return true;
		}

		if (m_running != force_stopping1 || m_running != force_stopping ) {
			m_running = force_stopping;
		}
		cond_var.notify_one();
		return m_running == force_stopping1;
	}

	void force_stop() {
		while (!set_force_stop()) {

			
		}

		m_worker.join();

	}
	void handle_return_value(react_proc_return ret) {
		if (ret == react_proc_return::stop) {
			m_running = force_stopping1;
		}
	}
protected:
	enum state {
		running,
		stopping,
		force_stopping,
		force_stopping1,
		stopped,
		starting0,
		starting1,
		starting2
	};

#ifndef  WIN32 
	state m_running = running;
#else
	std::atomic<state> m_running = starting0;
#endif
	void set_stopped() {
		has_finished = true;
		m_running = stopped;
	}
	bool has_finished = false;
	std::condition_variable cond_var;
	std::mutex m;
	std::mutex m_init_lock;
	std::condition_variable cond_init;
	std::thread m_worker;
	std::vector<reactive_processor_impl_c*> m_notify;

	std::vector<reactive_processor_impl_c*> m_notify_local;
	private:
		bool contains(reactive_processor_impl_c* f) {
			return std::find_if(m_notify.begin(), m_notify.end(), [&](reactive_processor_impl_c* e) { return e->ID() == f->ID(); }) != m_notify.end();
		}
};


}

#endif // reactive_backend_base_h__