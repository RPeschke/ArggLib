#ifndef write_host_h__
#define write_host_h__
#include "ArggLib/func.hh"
#include <iostream>
#include "object_modifier.hh"

namespace ArggLib {
	namespace ArggLib_impl {
		class write_out_impl :public fun_impl_b0 {
			add_parameter(header, std::string);
			add_parameter(footer, std::string);
			std::ostream& m_out;

		public:


			write_out_impl(std::ostream& out) :m_out(out) {
				footer("\n");
			}

			template <typename T>
			auto operator()(const T& t) {
				m_out << header();
				m_out << t;
				m_out << footer();
				return t;
			}

		};

	}
	inline auto write_host() {
		return  ArggLib_impl::write_out_impl{ std::cout };
	}

	inline auto write_out(std::ostream& out) {
		return  ArggLib_impl::write_out_impl{ out };
	}
}

#endif // write_host_h__
