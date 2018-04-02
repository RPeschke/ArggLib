#ifndef ArggLibconvert2HashTable_h__
#define ArggLibconvert2HashTable_h__
#include <map>
#include <type_traits>
#include <string>
#include <assert.h>
#include "ArggLib/procReturn.hh"
#include "ArggLib/proc.hh"


namespace ArggLib {

	namespace ArggLib_impl {

		inline	void fill_header(std::vector<std::string> & out,  size_t numberOfCollums) {

			out.push_back("x0");
			for (size_t i = 1; i < numberOfCollums; ++i) {
				out.push_back("x" + std::to_string(i)); 
			}
			
		}
		template<typename INPUT_TYPE, typename OUTPUT_TYPE>
		OUTPUT_TYPE __convert_to(INPUT_TYPE&& t, OUTPUT_TYPE&& default_arg) {
			return t;
		}

		template<typename INPUT_TYPE>
		std::string __convert_to(INPUT_TYPE&& t, std::string&& default_arg) {
			return std::to_string(t);
		}
		template<typename INPUT_TYPE>
		std::string __convert_to(INPUT_TYPE&& t, std::string& default_arg) {
			return std::to_string(t);
		}
		template<typename INPUT_TYPE>
		std::string __convert_to(INPUT_TYPE&& t,const  std::string& default_arg) {
			return std::to_string(t);
		}
		template<typename INPUT_TYPE, typename OUTPUT_TYPE>
		std::enable_if<1, std::string >::type __convert_to(INPUT_TYPE && t) {
			return  std::to_string(std::forward<INPUT_TYPE>(t));
		}
		template<typename HAST_TABLE_t, typename TAG_NAME_ITT, typename T2>
		void fill_hash_table(std::map<std::string, HAST_TABLE_t>& out, TAG_NAME_ITT TagName, T2&& TagValue) {
			out[*TagName] =
				__convert_to(std::forward<T2>(TagValue), HAST_TABLE_t());
		}
		template<typename HAST_TABLE_t, typename TAG_NAME_ITT, typename T2, typename... ARGS>
		void fill_hash_table(std::map<std::string, HAST_TABLE_t>& out, TAG_NAME_ITT TagName, T2&& TagValue, ARGS&&... args) {
			out[*TagName] = 				__convert_to(std::forward<T2>(TagValue), HAST_TABLE_t());
			++TagName;
			fill_hash_table(out, TagName,  std::forward<ARGS>(args)...);
		}

	}

	template <typename T>
	class convert2HashTable_impl_no_header {
#ifdef _DEBUG
		size_t NumberOfColums = 0;
#endif // _DEBUG
	public:
		std::vector<std::string> m_headers; 
		bool m_first = true;

		procReturn Start() {
			
			m_first = true;
			return procReturn::success;
		}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {
			if (m_first) {
				static_assert(sizeof...(args) > 0, "");
				ArggLib_impl::fill_header(m_headers, sizeof...(args));
				m_first = false;
#ifdef _DEBUG
				NumberOfColums = sizeof...(args);
#endif // _DEBUG

			}

#ifdef _DEBUG
			assert(NumberOfColums == sizeof...(args));
#endif // _DEBUG
			std::map<std::string,T> ret;
			ArggLib_impl::fill_hash_table(ret, m_headers.begin(), std::forward<ARGS>(args)...);
			//_Fill(*out, m_delimiter, args...);
			return next(ret);
		}

	};


	template <typename T>
	class convert2HashTable_impl {
		std::vector<std::string> m_headers;
	public:
		convert2HashTable_impl(std::vector<std::string> headers) : m_headers(std::move(headers)) {}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

#ifdef _DEBUG
			assert(m_headers.size() == sizeof...(args));
#endif // _DEBUG
			std::map<std::string, T> ret;
			ArggLib_impl::fill_hash_table(ret, m_headers.begin(), std::forward<ARGS>(args)...);
			//_Fill(*out, m_delimiter, args...);
			return next(ret);
		}

	};

	template<typename T = std::string>
	auto convert2HashTable() ->decltype (proc() >> convert2HashTable_impl_no_header<T>()) {
		return proc() >> convert2HashTable_impl_no_header<T>();
	}

	template<typename T = std::string>
	auto convert2HashTable(std::vector<std::string> headers) ->decltype(proc() >> convert2HashTable_impl<T>(std::move(headers))) {
		return proc () >>  convert2HashTable_impl<T>( std::move(headers));
	}
}

#endif // ArggLibconvert2HashTable_h__
