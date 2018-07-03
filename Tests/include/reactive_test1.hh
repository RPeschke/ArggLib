#ifndef reactive_test1_h__
#define reactive_test1_h__
#include <sstream>

#include "ArggLib/reactive_programming/reactive_backend.hh"
#include "ArggLib/reactive_programming/reactive_signals.hh"
#include "ArggLib/reactive_programming/reactive_entity_base.hh"
#include "ArggLib/reactive_programming/reactive_ports.hh"
namespace ArggLib {
	class myEntety2 :public reactive_entity_base {
	public:
		myEntety2(std::ostream& out, reactive_backend_base* backend) :reactive_entity_base([this]() { this->operator()(); return react_proc_return::success; }, backend), m_out(out) {}
		active_in_port<reactive_signals<int>> m_in;
		out_port<reactive_signals<int>> m_out_signal = 0;
		std::ostream& m_out;
		void operator()() {
			m_out << "<active_in_port>" << m_in.value() << "</active_in_port>\n";
			m_out_signal <<= m_in.value() * 2;
		}
	};


	inline std::string test_entity2(reactive_backend_base& r) {

		std::stringstream out;
		

		reactive_signals<int> x(1, &r);
		reactive_signals<int> y(1, &r);


		myEntety2 e1(out, &r);
		myEntety2 e2(out, &r);


		x >> e1.m_in;
		e1.m_out_signal >> e2.m_in;



		x = 2;

		r.stop();
		return out.str();
	}
}
#endif // reactive_test1_h__
