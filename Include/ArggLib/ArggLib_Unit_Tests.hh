#ifndef ArggLib_Unit_Tests_h__
#define ArggLib_Unit_Tests_h__




#include <functional>
#include <vector>
#include "ArggLib/PRECOMPILER_HELPER.hh"
#include "ArggLib/reverse.hh"
#include <iostream>

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

		}

		inline	static std::vector<std::function<void()>>& get_fun() {
			static std::vector<std::function<void()>> 	g_tests;
			return g_tests;
		}
	};

	inline	void run_tests() {
		__add_to_tests<>::run_tests();
	}

	template <typename T1, typename T2>
	void __Test_imple(int LineNR, std::string functionName, std::string FileName, std::string TestName, T1&& t1, T2&& t2) {

    std::cout << "<<<<<<<<<<<<<<<<<<<<<<\n";
    std::cout << "filename: " << FileName<<
      "\nline: " << LineNR << 
      "\nfun: " << functionName << 
      "\ntest '" << TestName << "'\n";
		if (t1 != t2) {
      std::cout << "status = failed\n";
			std::cout << "Variable: \n" << t1 << "\n Unequal \n " << t2 << "\n";
		}
		else {
      std::cout << "status = successful\n";
		}
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>\n\n";
	}
}

#endif // ArggLib_Unit_Tests_h__