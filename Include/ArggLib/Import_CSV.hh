#ifndef ArggLibimport_CSV_h__
#define ArggLibimport_CSV_h__
#include <type_traits>
#include "ArggLib/constexpr_if.hh"

namespace ArggLib {

	template <typename T>
	auto __readIn(std::ifstream& in, cstringr delimiter, T&& next) {

	}

	namespace ArggLib_impl {
		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t< COND, procReturn> expand_buffer_line(NEXT_T&& next, size_t index1,const std::string& buffer, cstringr delimiter,  ARGS&&... args) {

			auto index2 = buffer.find(delimiter, index1);
			auto index3 = buffer.find_first_not_of(' ', index1);
			index1 = std::max(index1, index3);
			auto index4 = buffer.find_first_of(' ', index1);
			index4 = std::min(index2, index4);
	

			if (index2 != std::string::npos) {
				expand_buffer_line<sizeof...(args) < 10>(next, index2 + 1, buffer, delimiter, std::forward<ARGS>(args)..., buffer.substr(index1, index4));
			}
			else
			{
				next(std::forward<ARGS>(args)..., buffer.substr(index1, index4));
			}
			
			return success;
		}

		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t < !COND, procReturn > expand_buffer_line(NEXT_T&& next, size_t index1,  const std::string& buffer, cstringr delimiter, ARGS&&... args) {

			throw std::invalid_argument("trying to read in a csv file with to many columns");
		

			return success;
		}
	}

	template <typename T>
	class Import_CSV_impl {
	public:
		std::shared_ptr<T> m_in;
		std::string m_name;
		const std::string  m_delimiter;
		std::string m_buffer;
		Import_CSV_impl(cstringr name,  cstringr delimiter) :
			m_name(name),
			m_delimiter(delimiter)
		{}
		std::vector<std::string> m_headers;


		procReturn Start() {
			m_in = Snew T(m_name);
			std::getline(*m_in, m_buffer);
			size_t index1 = 0;
			size_t index2 = 0;
			do{
				 index2 = m_buffer.find(m_delimiter, index1);
				 auto index3 = m_buffer.find_first_not_of(' ', index1);
				 index1 = std::max(index1, index3);
				 auto index4 = m_buffer.find_first_of(' ', index1);
				 index4 = std::min(index2, index4);
				 m_headers.push_back(m_buffer.substr(index1, index4));
				 index1 = index2+1 ;
			} while (index2 != std::string::npos);
			return procReturn::success;
		}
		procReturn End() {
			m_in->close();
			m_in.reset();
			return procReturn::success;
		}


		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {


			while (std::getline(*m_in, m_buffer)) {

				auto ret =  ArggLib_impl::expand_buffer_line<1>(next, 0, m_buffer, m_delimiter, std::forward<ARGS>(args)...);
				if (ret != success){
					return ret;
				}
			}
			return success;

			
		}


	};


	template <typename T>
	class Import_CSV_as_HashTable_impl {
	public:
		std::shared_ptr<T> m_in;
		std::string m_name;
		const std::string  m_delimiter;
		std::string m_buffer;
		Import_CSV_as_HashTable_impl(cstringr name, cstringr delimiter) :
			m_name(name),
			m_delimiter(delimiter)
		{}
		std::vector<std::string> m_headers;


		procReturn Start() {
			m_in = Snew T(m_name);
			std::getline(*m_in, m_buffer);
			size_t index1 = 0;
			size_t index2 = 0;
			do {
				index2 = m_buffer.find(m_delimiter, index1);
				auto index3 = m_buffer.find_first_not_of(' ', index1);
				index1 = std::max(index1, index3);
				auto index4 = m_buffer.find_first_of(' ', index1);
				index4 = std::min(index2, index4);
				m_headers.push_back(m_buffer.substr(index1, index4));
				index1 = index2 + 1;
			} while (index2 != std::string::npos);
			return procReturn::success;
		}
		procReturn End() {
			m_in->close();
			m_in.reset();
			return procReturn::success;
		}
		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

			std::map<std::string, std::string> ret;
			while (std::getline(*m_in, m_buffer)) {
				size_t index1 = 0;
				size_t index2 = 0;
				int header_index = 0;
				do {
					index2 = m_buffer.find(m_delimiter, index1);
					auto index3 = m_buffer.find_first_not_of(' ', index1);
					index1 = std::max(index1, index3);
					auto index4 = m_buffer.find_first_of(' ', index1);
					index4 = std::min(index2, index4);


					ret[m_headers[header_index++]] = m_buffer.substr(index1, index4);


					index1 = index2 + 1;
				} while (index2 != std::string::npos);
				auto ret1 =  next(std::forward<ARGS>(args)..., ret);
				if (ret1 != success) {
					return ret1;
				}
			}
			return success;

		}


	};

	template <typename T = std::ifstream>
	auto Import_CSV_as_HashTable(cstringr name, cstringr delimiter) {
		return Import_CSV_as_HashTable_impl<T>(name, delimiter);
	}
	template <typename T = std::ifstream>
	auto Import_CSV(cstringr name, cstringr delimiter) {
		return Import_CSV_impl<T>(name, delimiter);
	}
}

#endif // ArggLibimport_CSV_h__
