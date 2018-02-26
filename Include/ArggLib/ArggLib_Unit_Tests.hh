#ifndef ArggLib_Unit_Tests_h__
#define ArggLib_Unit_Tests_h__




#include <functional>
#include <vector>
#include "ArggLib/PRECOMPILER_HELPER.hh"
#include "ArggLib/reverse.hh"
#include <iostream>
#include <sstream>

#define  ARGGLIB__DEFINE_TEST(x) void x();  __add_to_tests<> __MAKE_UNIQUE_PRECOMPILER_NAME__(dummy){ []() { x(); } }; void x() 
#define ___ARGGLIB_TEST(TestName,First,Second) __Test_imple( __LINE__ , __FUNCTION__ ,__FILE__ , TestName, First, Second)

namespace ArggLib {


  template<typename T = void>
  class __add_to_tests {
  public:
    inline	__add_to_tests() {}
    inline	__add_to_tests(std::function<void()> f) {
      get_fun().push_back(std::move(f));
    }
    inline	static void run_tests() {
      for (auto& e : ArggLib::reverse(get_fun())) {
        e();
      }

      std::cout << "<<<<<<<<<<   Test Finished >>>>>>>>>>>>>>\nFailed Test: " << get_error_list().size() << " of " << get_fun().size() << "\n\n" ;
      for (auto& e : get_error_list()) {
        e(std::cout);
      }
      std::cout << "\n<<<<<<<<<<   End  >>>>>>>>>>>>>>\n";
    }

    inline	static std::vector<std::function<void()>>& get_fun() {
      static std::vector<std::function<void()>> 	g_tests;
      return g_tests;
    }
    inline	static std::vector<std::function<void(std::ostream&)>>& get_error_list() {
      static std::vector<std::function<void(std::ostream&)>> 	g_errors;
      return g_errors;
    }
    inline static void push_error(std::function<void(std::ostream&)> error_f) {
      get_error_list().push_back(std::move(error_f));
    }
  };

  inline	void run_tests() {
    __add_to_tests<>::run_tests();
  }

  template <typename T1, typename T2>
  void __Test_imple(int LineNR, std::string functionName, std::string FileName, std::string TestName, T1&& t1, T2&& t2) {


    std::stringstream out;

    out << "<<<<<<<<<<<<<<<<<<<<<<\n";
    out << "filename: " << FileName <<
      "\nline: " << LineNR <<
      "\nfun: " << functionName <<
      "\ntest '" << TestName << "'\n";
    if (t1 != t2) {
      out << "status = failed\n";
      out << "Variable: \n" << t1 << "\n Unequal \n " << t2 << "\n";


    }
    else {
      out << "status = successful\n";
    }
    out << ">>>>>>>>>>>>>>>>>>>>>>\n\n";

    std::cout << out.str();
    if (t1 != t2) {
      __add_to_tests<>::push_error([str = out.str()](std::ostream& out){
        out << str;
      });
    }

  }

}

#endif // ArggLib_Unit_Tests_h__