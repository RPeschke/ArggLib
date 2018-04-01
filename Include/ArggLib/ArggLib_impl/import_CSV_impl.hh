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
		struct  split_string_index_triplet{
			size_t start, size_trimed, end;
		};
		inline auto  split_string_to_index_pairs(cstringr buffer, const char delimiter, size_t offset) {
			auto index2 = buffer.find(delimiter, offset);
			auto index3 = buffer.find_first_not_of(' ', offset);
			offset = std::max(offset, index3);
			auto index4 = buffer.find_first_of(' ', index3);
			index4 = std::min(index2, index4);
			auto ret = split_string_index_triplet{ offset ,index4 - offset ,index2 };
			return ret;

		}
		template <bool COND, typename NEXT_T, typename... ARGS>
		std::enable_if_t< COND, procReturn> expand_buffer_line(NEXT_T&& next, size_t index1, cstringr buffer, const char delimiter, ARGS&&... args) {

			//std::cout << buffer << std::endl;

			auto in = split_string_to_index_pairs(buffer, delimiter, index1);

			if (in.end != std::string::npos) {
				expand_buffer_line < sizeof...(args) < 10 > (next, in.end + 1, 
					buffer, 
					delimiter, 
					std::forward<ARGS>(args)..., 
					buffer.substr(in.start, in.size_trimed));
			}
			else
			{
				next(std::forward<ARGS>(args)..., buffer.substr(in.start));
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

			auto in = split_string_to_index_pairs(buffer, delimiter, index1);



			if (in.end != std::string::npos) {
				expand_buffer_line_named_variables < sizeof...(args) < ArggLib_Import_CSV_MAX_NUMBER_OF_COLUMNS > (
					next,
					in.end + 1, buffer,
					index_header + 1, headers,
					delimiter,
					std::forward<ARGS>(args)...,
					make_named_variable(headers[index_header],
						buffer.substr(in.start, in.size_trimed)));
			}
			else
			{
				next(std::forward<ARGS>(args)..., make_named_variable(headers[index_header], buffer.substr(in.start)));
			}

			return success;
		}
	}
}
#endif // import_CSV_impl_h__
