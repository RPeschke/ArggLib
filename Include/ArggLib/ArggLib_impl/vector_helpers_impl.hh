#ifndef vector_helpers_impl_h__
#define vector_helpers_impl_h__


#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/object_modifier.hh"
#include "ArggLib/func.hh"



namespace ArggLib {
	namespace ArggLib_impl {

		template<typename T, typename T1>
		class make_vec_impl_2 {
		public:

			make_vec_impl_2(T t_, T1 t1_) :t(std::move(t_)), t1(std::move(t1_)) {}
			T t;
			T1 t1;
			template <typename T2>
			auto operator<(T2 t2) const {
				std::vector<T> ret;
				if (t1 > 0) {
					if (t > t1) {
						throw std::invalid_argument("Second Argument smaller then the First on");
					}
					for (auto i = t; i < t2; i = i + t1) {
						ret.push_back(i);

					}
				}
				else if (t1 < 0) {
					if (t < t1) {
						throw std::invalid_argument("Second Argument bigger then the First on");
					}
					for (auto i = t; i > t2; i = i + t1) {
						ret.push_back(i);

					}
				}
				else {
					throw std::invalid_argument("Second Argument musst not be zero");
				}

				return ret;
			}
			template <typename T2>
			auto operator<=(T2 t2) const {
				std::vector<T> ret;
				if (t1 > 0) {
					if (t > t1) {
						throw std::invalid_argument("Second Argument smaller then the First on");
					}
					for (auto i = t; i <= t2; i = i + t1) {
						ret.push_back(i);

					}
				}
				else if (t1 < 0) {
					if (t < t1) {
						throw std::invalid_argument("Second Argument bigger then the First on");
					}
					for (auto i = t; i >= t2; i = i + t1) {
						ret.push_back(i);

					}
				}
				else {
					throw std::invalid_argument("Second Argument musst not be zero");
				}

				return ret;
			}
		};

		template<typename T>
		class make_vec_impl_1 {
		public:

			make_vec_impl_1(T t_) :t(std::move(t_)) {}
			T t;
			template <typename T1>
			auto operator<<(T1 t1) const {
				T steps = 1;
				std::vector<T> ret;
				if (t > t1) {
					for (auto i = t; i > t1; --i ) {
						ret.push_back(i);

					}
				} else {

					for (auto i = t; i < t1; ++i ) {
						ret.push_back(i);

					}
				}




				return ret;
			}
			template <typename T1>
			auto operator<=(T1 t1) const {
				T steps = 1;
				std::vector<T> ret;
				if (t > t1) {
					for (auto i = t; i >= t1; --i) {
						ret.push_back(i);

					}
				}
				else {

					for (auto i = t; i <= t1; ++i) {
						ret.push_back(i);

					}
				}




				return ret;
			}
			template <typename T1>
			auto operator<(T1 t1) const {

				return make_vec_impl_2<T, T1>(t, std::move(t1));
			}

		};

		template <typename comp_t, typename GET_Prob_t, typename ABS_F>
		class sort_impl :public fun_impl_b0 {
			GET_Prob_t m_get_proberty;
			comp_t m_comp;
			ABS_F m_abs;
		public:

			template<typename T>
			auto by_property(T get_proberty) {
				return  sort(m_comp, get_proberty, m_abs);
			}
			auto absulute() const {
				return  sort(m_comp, m_get_proberty, [](cautor e) { return std::abs(e); });
			}
			auto by_first() const {
				return  sort(m_comp, [](cautor e) { return e.first; } , m_abs);
			}
			auto by_second() const {
				return  sort(m_comp, [](cautor e) { return e.second; } , m_abs);
			}
			auto ascending() const {
				return  sort([](cautor e1, cautor e2) {return  e1 < e2; }, m_get_proberty, m_abs);
			}
			auto descending() const {
				return  sort([](cautor e1, cautor e2) {return  e1 > e2; }, m_get_proberty, m_abs);
			}
			sort_impl(comp_t comp, GET_Prob_t get_proberty, ABS_F abs_f) :m_comp(std::move(comp)), m_get_proberty(std::move(get_proberty)),m_abs(std::move(abs_f)) {}
			template <typename Container_t>
			auto operator()(Container_t hir) {
				auto pred = [&](cautor e1, cautor e2) {  return m_comp(m_abs(m_get_proberty(e1)), m_abs(m_get_proberty(e2))); };
				std::sort(hir.begin(), hir.end(), pred);
				return hir;
			}
		};

		template <typename T>
		auto  to_vector_impl_convert_element(T&& t) {
			return t;

		};


		template <typename T1, typename T2>
		auto  to_vector_impl_convert_element(std::pair<T1, T2>& t) {

			return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

		};
		template <typename T1, typename T2>
		auto  to_vector_impl_convert_element(std::pair<T1, T2>&& t) {

			return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

		};

		template <typename T1, typename T2>
		auto  to_vector_impl_convert_element(const std::pair<T1, T2>& t) {

			return   std::pair<std::remove_const_t<T1>, std::remove_const_t<T2>>(t.first, t.second);;

		};


		template<typename T>
		class to_vector_impl :public fun_impl_b0 {
		public:
			to_vector_impl(T f1) : m_fun(std::move(f1)) {}
			T m_fun;


			template <typename T1 >
			auto set_converter_function(T1 f) {
				return to_vector(std::move(f));
			}

			template <typename T2>
			auto operator()(T2& container_v) {
				std::vector<
					ArggLib::remove_cvref_t<
					decltype(m_fun(*container_v.begin()))
					>
				> ret;
				for (auto e : container_v) {
					ret.push_back(m_fun(e));
				}

				return ret;
			}


		};


		template <typename T1>
		auto to_string(const T1 & p) ->decltype(std::to_string(p)) {
			return std::to_string(p);

		}

		inline const std::string & to_string(const std::string & p) {
			return p;

		}
		template <typename T1, typename T2>
		auto to_string(const std::pair<T1, T2>& p) {
			std::string buffer;
			buffer +=
				to_string(p.first) +
				"  " +
				to_string(p.second);
			return buffer;
		}

		template <typename to_string_f>
		class to_string_impl : public fun_impl_b0 {
			add_parameter(delimiter, std::string);
			add_parameter(header, std::string);
			add_parameter(footer, std::string);
			to_string_f m_to_string;
		public:
			to_string_impl(to_string_f f1) :m_to_string(std::move(f1)) {}

			template <typename Container_t>
			auto operator()(const Container_t& con) const  -> decltype(std::begin(con), std::string()) {

				std::string buffer = header();
				bool first = true;
				for (cautor e : con) {
					if (first) {
						first = false;
					}
					else {
						buffer += delimiter();
					}
					buffer += m_to_string(e);

				}
				buffer += footer();
				return buffer;
			}

			template <typename Container_t>
			auto operator()(const Container_t& con) const  -> decltype(con.str(), std::string()) {

				std::string buffer = header();
				buffer += con.str();
				buffer += footer();
				return buffer;
			}
		};

	}




}


#endif // vector_helpers_impl_h__
