#ifndef reactive_ports_h__
#define reactive_ports_h__


#include "ArggLib/reactive_programming/reactive_signals.hh"
#include "ArggLib/reactive_programming/reactive_entity_base.hh"
#include "ArggLib/reactive_programming/reactive_variable.hh"

namespace ArggLib {
	template<typename reactive_signals_T>
	class passive_in_port {
	public:
		reactive_entity_base * m_base = nullptr;
		const reactive_signals_T* m_input = nullptr;

		passive_in_port() :m_base(get_current_reactive_entity_base()) {

		}
		virtual void set_input(reactive_signals_T* input) {

			m_input = input;

		}
		auto value() const ->decltype(m_input->value()){
			return this->m_input->value();
		}

	};

	template<typename reactive_signals_T>
	class active_in_port : public passive_in_port<reactive_signals_T> {
	public:

		virtual void set_input(reactive_signals_T* input) override {
			passive_in_port<reactive_signals_T>::set_input(input);
			input->register_processor_ptr(&this->m_base->m_process);
		}

	};


	template<typename reactive_signals_T>
	class out_port {
	public:
		reactive_entity_base * m_base = nullptr;
		reactive_signals_T m_out;

		template <typename T>
		out_port(T&& t) :m_base(get_current_reactive_entity_base()), m_out(std::forward<T>(t),get_current_reactive_entity_base()->m_backend) {

		}

		auto value() const ->decltype(m_out.value()) {
			return m_out.value();
		}
		template <typename T>
		void set(T&& t) {
			m_out.set(std::forward<T>(t));
		}
		template <typename T>
		void operator<<=(T&& t) {
			m_out.set(std::forward<T>(t));
		}

	};

	template <typename T1, typename T2>
	out_port< T1> & operator>>(out_port< T1> & out, passive_in_port<T2>& in) {
		in.set_input(&out.m_out);
		return out;
	}

	template <typename T1, typename T2>
	reactive_variable<T1>& operator>>(reactive_variable<T1> & out, passive_in_port<T2>& in) {
		in.set_input(&out);
		return out;
	}
	template <typename T1, typename T2>
	reactive_signals<T1>& operator>>(reactive_signals<T1> & out, passive_in_port<T2>& in) {
		in.set_input(&out);
		return out;
	}


}


#endif // reactive_ports_h__
