#ifndef ArggLib_Unit_Tests_h__
#define ArggLib_Unit_Tests_h__




#include <functional>
#include <vector>
#include "ArggLib/PRECOMPILER_HELPER.hh"
#include "reverse.hh"

#define  ARGGLIB__DEFINE_TEST(x) void x();  __add_to_tests<> __MAKE_UNIQUE_PRECOMPILER_NAME__(dummy){ []() { x(); } }; void x() 


namespace ArggLib {


	template<typename T = void>
	class __add_to_tests {
	public:
		__add_to_tests(std::function<void()> f) {
			get_fun().push_back(std::move(f));
		}
		static void run_tests() {
			for (auto& e : ArggLib::reverse(get_fun())) {
				e();
			}

		}

		static std::vector<std::function<void()>>& get_fun() {
			static std::vector<std::function<void()>> 	g_tests;
			return g_tests;
		}
	};

	inline	void run_tests() {
		__add_to_tests<>::run_tests();
	}

	template <typename T1, typename T2>
	void __Test(std::string TestName, T1&& t1, T2&& t2) {

		if (t1 != t2) {
			cout << "test '" << TestName << "' failed\n";
			cout << "Variable: \n" << t1 << "\n Unequal \n " << t2 << endl;
		}
		else {
			cout << "test '" << TestName << "' successful\n";
		}

	}
}

#endif // ArggLib_Unit_Tests_h__