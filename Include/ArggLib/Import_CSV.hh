#ifndef ArggLibimport_CSV_h__
#define ArggLibimport_CSV_h__
#include <type_traits>
#include "ArggLib/constexpr_if.hh"
#include "named_variable.hh"
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

      auto index2 = buffer.find(delimiter, index1);
      auto index3 = buffer.find_first_not_of(' ', index1);
      index1 = std::max(index1, index3);
      auto index4 = buffer.find_last_not_of(' ', index2) + 1;
      index4 = std::min(index2, index4);


      if (index2 != std::string::npos) {
        expand_buffer_line < sizeof...(args) < 10 > (next, index2 + 1, buffer, delimiter, std::forward<ARGS>(args)..., buffer.substr(index1, index4));
      }
      else
      {
        next(std::forward<ARGS>(args)..., buffer.substr(index1, index4));
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




    template <typename T>
    class Import_CSV_impl_base {
    public:
      std::shared_ptr<T> m_in;
      std::string m_name;
      const char  m_delimiter;
      std::string m_buffer;

	  T& get_instream() {
        return *m_in;
      }
      Import_CSV_impl_base(cstringr name, const char delimiter) :
        m_name(name),
        m_delimiter(delimiter)
      {}
      std::vector<std::string> m_headers;


      procReturn Start() {
        m_in = Snew T(m_name);
        std::getline(*m_in, m_buffer);
        m_headers.clear();
        m_headers = split_string2vector(m_buffer, m_delimiter);
        return procReturn::success;
      }
      procReturn End() {
        // m_in->close();
        m_in.reset();
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

          auto ret = ArggLib_impl::expand_buffer_line<1>(next, 0, this->m_buffer, this->m_delimiter, std::forward<ARGS>(args)...);
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

          split_string(this->m_buffer, this->m_delimiter, [&ret, header_index = int(0), this](cautor s) mutable {
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

          auto ret = ArggLib_impl::expand_buffer_line_named_variables<1>(
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
  auto Import_CSV_as_HashTable(cstringr name, const char delimiter) {
    return proc() >> ArggLib_impl::Import_CSV_as_HashTable_impl<T>(name, delimiter);
  }
  template <typename T = std::ifstream>
  auto Import_CSV(cstringr name, const char delimiter) {
    return proc() >> ArggLib_impl::Import_CSV_impl<T>(name, delimiter);
  }
  template <typename T = std::ifstream>
  auto Import_CSV_as_named_variables(cstringr name, const char delimiter) {
    return proc() >> ArggLib_impl::Import_CSV_as_named_variables_impl<T>(name, delimiter);
  }
}

#endif // ArggLibimport_CSV_h__
