#ifndef ArggLibimport_CSV_h__
#define ArggLibimport_CSV_h__
#include <type_traits>

#include "ArggLib/named_variable.hh"
#include "ArggLib/ArggLib_impl/import_CSV_impl.hh"
#include "ArggLib/type_trates.hh"
#include "ArggLib/smart_ptr_helpers.hh"
#include "ArggLib/string_helpers.hh"
#include "ArggLib/proc.hh"


namespace ArggLib {

  namespace ArggLib_impl {



    template <typename T>
    class Import_CSV_impl_base0 {
    public:
      Import_CSV_impl_base0(const char delimiter) : m_delimiter(delimiter) {}
      std::shared_ptr<T> m_in;
      std::vector<std::string> m_headers;
      const char  m_delimiter;
      std::string m_buffer;
      T& get_instream() {
        return *m_in;
      }
      procReturn End() {
        m_in.reset();
        return procReturn::success;
      }
    };

    template <typename T>
    class Import_CSV_impl_base :public Import_CSV_impl_base0<T> {
    public:

      std::string m_name;

      Import_CSV_impl_base(cstringr name, const char delimiter) : Import_CSV_impl_base0<T>(delimiter),
        m_name(name)
      {}




      procReturn Start() {
        this->m_in = Snew T(m_name);
        std::getline(*this->m_in, this->m_buffer);
        this->m_headers.clear();
        this->m_headers = split_string2vector(this->m_buffer, this->m_delimiter);
        return procReturn::success;
      }

    };


    template <typename T>
    class Import_CSV_impl : public   Import_CSV_impl_base<T> {
    public:

      using Import_CSV_impl_base<T>::Import_CSV_impl_base;



      template <typename NEXT_T, typename... ARGS>
      procReturn operator()(NEXT_T&& next, ARGS&&... args) {


        while (std::getline(this->get_instream(), this->m_buffer)) {

          auto ret = expand_buffer_line<1>(next, 0, this->m_buffer, this->m_delimiter, std::forward<ARGS>(args)...);
          if (ret != success) {
            return ret;
          }
        }
        return success;


      }


    };


    template <typename T>
    class Import_CSV_as_HashTable_impl : public Import_CSV_impl_base<T> {
    public:
      using Import_CSV_impl_base<T>::Import_CSV_impl_base;
      template <typename NEXT_T, typename... ARGS>
      procReturn operator()(NEXT_T&& next, ARGS&&... args) {


        std::map<std::string, std::string> ret;
        while (std::getline(this->get_instream(), this->m_buffer)) {
          auto header_index = int(0);
          split_string(this->m_buffer, this->m_delimiter, [&ret, header_index, this](cstringr s) mutable {
            ret[this->m_headers[header_index++]] = s;

          });


          auto ret1 = next(std::forward<ARGS>(args)..., ret);
          if (ret1 != success) {
            return ret1;
          }
        }
        return success;

      }


    };



    template <typename T>
    class Import_CSV_as_named_variables_impl :public Import_CSV_impl_base<T> {
    public:


      using Import_CSV_impl_base<T>::Import_CSV_impl_base;


      template <typename NEXT_T, typename... ARGS>
      procReturn operator()(NEXT_T&& next, ARGS&&... args) {


        while (std::getline(this->get_instream(), this->m_buffer)) {

          auto ret = expand_buffer_line_named_variables<1>(
            next,
            0, this->m_buffer,
            0, this->m_headers,
            this->m_delimiter,
            std::forward<ARGS>(args)...
            );
          if (ret != success) {
            return ret;
          }
        }
        return success;


      }


    };

  }
    template <typename T = std::ifstream>
    auto Import_CSV(cstringr name, const char delimiter) ->decltype(proc() >> ArggLib_impl::Import_CSV_impl<T>(name, delimiter)) {
      return proc() >> ArggLib_impl::Import_CSV_impl<T>(name, delimiter);
    }

    namespace ArggLib_impl{
      template <typename T>
      class Import_CSV_impl_stream_in;

		template <typename T>
		class Import_CSV_impl_stream_in :public Import_CSV_impl_base0 <T>{

    public:

      using Import_CSV_impl_base0<T>::Import_CSV_impl_base0;


      auto delimiter(const char del) -> ArggLib::procImple<Import_CSV_impl_stream_in<T>> {
        return proc() >> Import_CSV_impl_stream_in<T>(del);
      }
			auto fileName(cstringr fileName_) ->decltype(Import_CSV(fileName_,this-> m_delimiter) ){
				return Import_CSV(fileName_, this->m_delimiter);
			}


			template <typename NEXT_T, typename T1, typename... ARGS>
			procReturn operator()(NEXT_T&& next, T1&& t1, ARGS&&... ) {

        this->m_in = Snew T(t1);
				std::getline(*this->m_in, this->m_buffer);
        this->m_headers.clear();
				this->m_headers = split_string2vector(this->m_buffer, this->m_delimiter);

				while (std::getline(this->get_instream(), this->m_buffer)) {
					//next(this->m_buffer);
					auto ret =  expand_buffer_line<1>(next, 0, this->m_buffer, this->m_delimiter);
					if (ret != success) {
						return ret;
					}
				}
				return success;


			}

		};

	}


	template <typename T = std::ifstream>
	auto Import_CSV_as_HashTable(cstringr name, const char delimiter) -> decltype(proc() >> ArggLib_impl::Import_CSV_as_HashTable_impl<T>(name, delimiter)) {
		return proc() >> ArggLib_impl::Import_CSV_as_HashTable_impl<T>(name, delimiter);
	}

	template <typename T = std::ifstream>
	auto Import_CSV(const char delimiter = ';') -> decltype(proc() >> ArggLib_impl::Import_CSV_impl_stream_in<T>(delimiter) ){
		return proc() >> ArggLib_impl::Import_CSV_impl_stream_in<T>(delimiter);
	}
	template <typename T = std::ifstream>
	auto Import_CSV_as_named_variables(cstringr name, const char delimiter) -> decltype(proc() >> ArggLib_impl::Import_CSV_as_named_variables_impl<T>(name, delimiter) ){
		return proc() >> ArggLib_impl::Import_CSV_as_named_variables_impl<T>(name, delimiter);
	}
}

#endif // ArggLibimport_CSV_h__
