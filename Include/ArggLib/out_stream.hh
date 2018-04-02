#ifndef ArggLibout_stream_h___
#define ArggLibout_stream_h___

#include <map>
#include <string>
#include <vector>
#include "ArggLib/string_helpers.hh"
#include "ArggLib/smart_ptr_helpers.hh"
#include "ArggLib/cautor.hh"
#include "ArggLib/proc.hh"


namespace ArggLib {


  namespace ArggLib_impl {

    class return_input {
    public: 
      template<typename T>
      auto operator()(T&& t)  -> decltype (std::forward<T>(t)){
        return std::forward<T>(t);
      }
    };
    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2> & t) {
      out << "first: {" << t.first << "} second: {" << t.second << "}";
      return out;
    }
    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& out, std::pair<T1, T2> & t) {
      out << "first: {" << t.first << "} second: {" << t.second << "}";
      return out;
    }

    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& out, std::pair<T1, T2> && t) {
      out << "first: {" << t.first << "} second: {" << t.second << "}";
      return out;
    }

    template<typename T>
    void print__(std::ostream& out, const std::string& delimer, T&& t) {
      out << t << '\n';
    }
    template<typename T, typename... ARGS>
    void print__(std::ostream& out, const std::string& delimer, T&& t, ARGS&&... args) {
      out << t << delimer;
      print__(out, delimer, std::forward<ARGS>(args)...);
    }

    template<typename T, typename accessor_F >
    void print__(std::ostream& out, const std::string& delimer, const std::vector<T>& t, accessor_F&& accessor) {
      bool first = true;
      for (auto& e : t)
      {
        if (first) {
          out << accessor(e);
          first = false;
        }
        else {
          out << delimer << accessor(e);
        }
      }
      out << '\n';
    }

    template<typename T, typename accessor_F >
    void print__(std::ostream& out, const std::string& delimer, std::vector<T>& t, accessor_F&& accessor) {

      print__(out, delimer, const_cast<const  std::vector<T>&>(t), accessor);
    }
    template<typename T  >
    void print__(std::ostream& out, const std::string& delimer, const std::vector<T>& t) {
      print__(out, delimer, t, ArggLib_impl::return_input{});
    }

    template<typename T  >
    void print__(std::ostream& out, const std::string& delimer, std::vector<T>& t) {
      print__(out, delimer, const_cast<const  std::vector<T>&>(t));
    }

  }

	template <typename... T>
	void _Fill(std::ostream& out, const std::string& delimer, T&&... i) {
		ArggLib::ArggLib_impl::print__(out, delimer, std::forward<T>(i)...);
	}

	namespace ArggLib_impl {
		inline	void fill_header(std::ostream& out, cstringr delimer, size_t numberOfCollums) {

			out << "x0";
			for (size_t i = 1; i < numberOfCollums; ++i) {
				out << delimer << "x" << i;
			}
			out << '\n';
		}

		template <typename First_t, typename Second_t, typename OUT_STREAM_t>
		void out_stream_hash_tables(const std::map<First_t, Second_t>& hash_table, OUT_STREAM_t& out, bool& first, std::vector<First_t>& headers, cstringr delimiter) {
			if (first) {

				for (cautor e : hash_table) {
					headers.push_back(e.first);
				}

				_Fill(out, delimiter, headers);
				first = false;
			}

			
			print__(out, delimiter, headers, [&hash_table](const First_t& e) { return hash_table.at(e); });
		}
	}

	template<typename T>
	class out_stream_impl {
	public:
		T& m_out_stream;
		std::vector<std::string> m_headers; // used for hash tables
		bool m_first; // used for hash tables
		const std::string m_delimiter;

		auto delimiter(cstringr delimiter_) -> ArggLib::procImple<out_stream_impl<T>> {
			return out_stream(m_out_stream, delimiter_);
		}
		out_stream_impl(T& out_stream, const std::string& delimiter) :m_out_stream(out_stream) , m_delimiter(delimiter) {

		}

		
		procReturn Start() {
			m_headers.clear();
			m_first = true;
			return procReturn::success;
		}



		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

			_Fill(m_out_stream, m_delimiter, args...);
			return next(std::forward<ARGS>(args)...);
		}

		template <typename NEXT_T, typename HAST_TABLE_t>
		procReturn operator()(NEXT_T&& next, std::map<std::string, HAST_TABLE_t>&& hash_table) {
			ArggLib_impl::out_stream_hash_tables(hash_table, m_out_stream, m_first, m_headers, m_delimiter);
			
			return next(std::move(hash_table));
		}
		template <typename NEXT_T, typename HAST_TABLE_t>
		procReturn operator()(NEXT_T&& next, std::map<std::string, HAST_TABLE_t>& hash_table) {
			ArggLib_impl::out_stream_hash_tables(hash_table, m_out_stream, m_first, m_headers, m_delimiter);
			
			return next(hash_table);
		}

		template <typename NEXT_T, typename HAST_TABLE_t>
		procReturn operator()(NEXT_T&& next, const std::map<std::string, HAST_TABLE_t>& hash_table) {
			ArggLib_impl::out_stream_hash_tables(hash_table, m_out_stream, m_first, m_headers, m_delimiter);
			
			return next(hash_table);
		}

	};

	template <typename T>
	class out_stream_impl0 : public out_stream_impl<T> {
	public:
		std::shared_ptr<T> m_out_owned;

		auto delimiter(cstringr delimiter_) -> ArggLib::procImple<out_stream_impl0<T>>  {
			return proc() >> out_stream_impl0<T>(m_out_owned, delimiter_);
		}

		out_stream_impl0(std::shared_ptr<T> out_sp , cstringr delimiter ) : out_stream_impl<T>(*out_sp, delimiter), m_out_owned(out_sp) {}
		
		template<typename... ARGs>
    T&& End(ARGs&&... )   {
			
			auto ret = m_out_owned;
			m_out_owned.reset();
			return std::move(*ret);
		}

	};

	template <typename T>
	auto out_stream(T& out_stream, cstringr delimiter = "  ") -> decltype(proc() >> out_stream_impl<T>(out_stream, delimiter)) {
		return proc()>> out_stream_impl<T>(out_stream, delimiter);
	}


	inline auto out_stream( cstringr delimiter = "  " ) -> decltype(proc() >> out_stream_impl0<std::stringstream>(Snew std::stringstream(), delimiter)) {
		return proc() >> out_stream_impl0<std::stringstream>( Snew std::stringstream(),delimiter );
	}

	inline auto display() -> decltype(ArggLib::out_stream(std::cout)) {
		return	ArggLib::out_stream(std::cout);
	}
}

#endif // ArggLibout_stream_h___
