#ifndef import_CSV_impl_h__
#define import_CSV_impl_h__

#define ArggLib_Import_CSV_MAX_NUMBER_OF_COLUMNS 10
namespace ArggLib {
	namespace ArggLib_impl {

		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t < !COND, procReturn > expand_buffer_line(NEXT_T&& next, size_t index1, cstringr buffer, const char delimiter, ARGS&&... args) {

			throw std::invalid_argument("trying to read in a csv file with to many columns");


			return success;
		}


		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t< COND, procReturn> expand_buffer_line(NEXT_T&& next, size_t index1, cstringr buffer, const char delimiter, ARGS&&... args) {

			//std::cout << buffer << std::endl;
			auto index2 = buffer.find(delimiter, index1);
			auto index3 = buffer.find_first_not_of(' ', index1);
			index1 = std::max(index1, index3);
			auto index4 = buffer.find_first_of(' ', index3) ;
			index4 = std::min(index2, index4);


			if (index2 != std::string::npos) {
				expand_buffer_line < sizeof...(args) < 10 > (next, index2 + 1, buffer, delimiter, std::forward<ARGS>(args)..., buffer.substr(index1, index4-index1));
			}
			else
			{
				next(std::forward<ARGS>(args)..., buffer.substr(index1));
			}

			return success;
		}


		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t < !COND, procReturn > expand_buffer_line_named_variables(
			NEXT_T&& next,
			size_t index1, cstringr buffer,
			size_t index_header, cstringsr headers,
			const char delimiter,
			ARGS&&... args) {

			throw std::invalid_argument("trying to read in a csv file with to many columns");


			return success;
		}

		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t< COND, procReturn> expand_buffer_line_named_variables(
			NEXT_T&& next,
			size_t index1, cstringr buffer,
			size_t index_header, cstringsr headers,
			const char delimiter,
			ARGS&&... args) {

			auto index2 = buffer.find(delimiter, index1);
			auto index3 = buffer.find_first_not_of(' ', index1);
			index1 = std::max(index1, index3);
			auto index4 = buffer.find_last_not_of(' ', index2) + 1;
			index4 = std::min(index2, index4);



			if (index2 != std::string::npos) {
				expand_buffer_line_named_variables < sizeof...(args) < ArggLib_Import_CSV_MAX_NUMBER_OF_COLUMNS > (
					next,
					index2 + 1, buffer,
					index_header + 1, headers,
					delimiter,
					std::forward<ARGS>(args)...,
					make_named_variable(headers[index_header],
						buffer.substr(index1, index4)));
			}
			else
			{
				next(std::forward<ARGS>(args)..., make_named_variable(headers[index_header], buffer.substr(index1, index4)));
			}

			return success;
		}
	}
}
#endif // import_CSV_impl_h__
