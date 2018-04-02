#ifndef Export_CSV_h__
#define Export_CSV_h__
#include <fstream>
#include <assert.h> 
#include "ArggLib/out_stream.hh"
#include "ArggLib/smart_ptr_helpers.hh"
#include "ArggLib/cautor.hh"
#include "ArggLib/string_helpers.hh"
#include <map>
#include <memory>


namespace ArggLib {


	template<typename T>
	class Export_CSV_impl_no_header {
	public:
		std::shared_ptr<T> out;
		std::string m_name;
		const std::string m_delimiter;
		std::vector<std::string> m_headers; // used for hash tables
		bool m_first = true;
		Export_CSV_impl_no_header(cstringr name, cstringr delimiter) :m_name(name), m_delimiter(delimiter) {}
#ifdef _DEBUG
		size_t NumberOfColums = 0;
#endif // _DEBUG


		procReturn Start() {
			out = Snew T(m_name);
			m_first = true; 
			m_headers.clear();
			return procReturn::success;
		}
		procReturn End() {
			out->close();
			out.reset();
			return procReturn::success;
		}
		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {
			if (m_first) {
				static_assert(sizeof...(args) > 0, "");
				ArggLib_impl::fill_header(*out, m_delimiter, sizeof...(args));
				m_first = false;
#ifdef _DEBUG
				NumberOfColums = sizeof...(args);
#endif // _DEBUG

			}

#ifdef _DEBUG
			assert(NumberOfColums == sizeof...(args));
#endif // _DEBUG

			_Fill(*out, m_delimiter, args...);
			return next(std::forward<ARGS>(args)...);
		}


		template <typename NEXT_T, typename First_t, typename Second_t>
		procReturn operator()(NEXT_T&& next, std::map<First_t, Second_t>& hash_table) {
			ArggLib_impl::out_stream_hash_tables(hash_table, *out, m_first, m_headers, m_delimiter);
			return 	 next(hash_table);
		}
		template <typename NEXT_T, typename First_t, typename Second_t>
		procReturn operator()(NEXT_T&& next,const std::map<First_t, Second_t>& hash_table) {
			ArggLib_impl::out_stream_hash_tables(hash_table, *out, m_first, m_headers, m_delimiter);
			return 	 next(hash_table);
		}

	};

	template <typename T = std::ofstream>
	auto Export_CSV(cstringr name, cstringr delimiter = ";  ") -> decltype(proc() >> Export_CSV_impl_no_header<T>(name, delimiter)) {
		return proc() >> Export_CSV_impl_no_header<T>(name, delimiter);

	}




	template<typename T>
	class Export_CSV_impl {
	public:
		std::shared_ptr<T> out;
		std::string m_name;
		const std::string  m_delimiter;
		Export_CSV_impl(cstringr name, std::vector<std::string> headers, cstringr delimiter) :
			m_name(name),
			m_headers(std::move(headers)),
			m_delimiter(delimiter)
		{}
		std::vector<std::string> m_headers;


		procReturn Start() {
			out = Snew T(m_name);

			ArggLib_impl::print__(*out, m_delimiter, m_headers);
			return procReturn::success;
		}
		procReturn End() {
			out->close();
			out.reset();
			return procReturn::success;
		}
		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {


#ifdef _DEBUG
			assert(m_headers.size() == sizeof...(args));
#endif // _DEBUG

			_Fill(*out, m_delimiter, args...);
			return next(std::forward<ARGS>(args)...);
		}



	};

	template <typename T = std::ofstream>
	auto Export_CSV(const std::string& name, std::vector<std::string> headers, const std::string& delimiter = ";  ") ->decltype (proc() >> Export_CSV_impl<T>(name, std::move(headers), delimiter)) {
		return proc() >> Export_CSV_impl<T>(name, std::move(headers), delimiter);

	}

	template <typename T = std::ofstream>
	auto Export_CSV(const std::string& name, const std::string& delimiter, std::vector<std::string> headers) ->decltype(proc() >> Export_CSV_impl<T>(name, std::move(headers), delimiter)) {
		return proc() >> Export_CSV_impl<T>(name, std::move(headers), delimiter);

	}
}

#endif // Export_CSV_h__
