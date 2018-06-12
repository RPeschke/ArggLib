#ifndef reactive_variable_h__
#define reactive_variable_h__
#include "ArggLib/reactive_programming/reactive_backend.hh"
#include "ArggLib/contains.hh"

#include "ArggLib/reactive_programming/reactive_processor.hh"

#include "ArggLib/reactive_programming/reactive_obj_base.hh"

namespace ArggLib {
	template <typename T>
	class reactive_variable :public reactive_obj_base{
	public:
		using type = T;
		reactive_variable(T value) : m_value(std::move(value)){

		}
		reactive_variable(T value,reactive_backend* backend) : reactive_obj_base( backend),m_value(std::move(value)) {

		}

		operator T() {
			return m_value;
		}
		T value() const {
			return m_value;
		}

		void set(const T& val) {
			m_value = val;
			notify();
		}

		template <typename T1>
		void operator<<=(T1&& t) {
			m_value = std::forward<T>(t);
			notify();
			return *this;
		}

		reactive_variable<T>& operator=(T value) {

			m_value = std::move(value);
			notify();
			return *this;
		}
	private:
		T m_value;
	};

}
#endif // reactive_variable_h__
