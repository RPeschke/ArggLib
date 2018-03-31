#include "ArggLib/complete_lib.hh"

#include <vector>
#include <algorithm>
#include <numeric>
#include <ostream>
#include <iostream>
#include <functional>
#include <string>



#include <sstream>
#include <map>


using namespace std;
using namespace ArggLib;

#define  EXAMPLE 1






ARGGLIB__DEFINE_TEST(func_test1) {
  std::stringstream out;
  auto disp = [&out](auto&& e) {

    out << e << std::endl;
  };

  auto square_ = [](auto&& e) {
    return e*e;
  };

  //   auto ttt = (fun() << square_ << 7 <<= _run());
  //    auto p=param() << 1 << 1;
  auto f = fun() << square_ << 7;
  auto xx = param() | f;
  auto xxx = param() | fun() << square_ << 8;
  auto xxx1 = param() | fun() << square_ << 8 > fun() << get_ints3 << 1 << _X << 1;

  auto f1 = fun() << square_;

  auto in_ = param();
  //  decltype(_____try_if_arg_has_return_type(__is_not_param(in_)), f1(in_)) x2;
  auto i = 7 | f1;

  // __is_not_param(1);

  auto p = param();
  decltype(__is_not_param(param()), 1) x1;
  //    auto f = fun() << make_vec() << 1 << (fun() << square_ << 7)() <<1
  //               >  sum_all() 
  //               > disp;
  //    f();
  //    
  //   fun() << get_ints3 << 1 << 49 << 1
  //               > fun() << sum_all() << _X() << 10> disp <<= _run();

   // auto rr = r <<= _run();

   // auto ff1 = ff<< 0 << 1;
    //ff1();
  //   auto f1 = f << 0 << 1;
     //f1();
  //   param() << 0 << 10 | fun() > get_ints3 > sum_all() > disp;+++++








}


ARGGLIB__DEFINE_TEST(func_test_pipe_test) {


  std::map<std::string, double> map_in;
  map_in["dsad0"] = 0;
  map_in["dsad1"] = 2;
  map_in["dsad3"] = 3;
  map_in["dsad4"] = -5454;


  auto vector_out = map_in | to_vector() | ArggLib_sort_by(second);
  std::stringstream out;
  //map_in | ArggLib::out_stream(out);
  vector_out | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("Function pipe operator",
    out.str(),
    "first: {dsad4} second: {-5454}\nfirst: {dsad0} second: {0}\nfirst: {dsad1} second: {2}\nfirst: {dsad3} second: {3}\n"
  );


  auto x_true = make_vec({ 1,2,5,6 }) | ArggLib::contains(1);

  ___ARGGLIB_TEST("Function contains true case",
    x_true,
    true
  );
  auto x_false = make_vec({ 1,2,5,6 }) | ArggLib::contains(100);

  ___ARGGLIB_TEST("Function contains false case",
    x_false,
    false
  );

}



ARGGLIB__DEFINE_TEST(func_test_pipe_test2121) {
  std::stringstream out;
  auto x = ArggLib::_to_vector << 1.0 << 10;

  x | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("auto x = _to_vector << 1.0 << 10;",
    out.str(),
    "1\n2\n3\n4\n5\n6\n7\n8\n9\n"
  );

  out.str("");
  out.clear();


  auto x2 = ArggLib::_to_vector << 1.0 < -2 < -11;
  x2 | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("auto x2 = ArggLib::_to_vector << 1.0 < -2 < -11;",
    out.str(),
    "1\n-1\n-3\n-5\n-7\n-9\n"
  );


  out.str("");
  out.clear();

  auto x3 = ArggLib::_to_vector << 1.0 < -2 <= -11;
  x3 | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("auto x3 = ArggLib::_to_vector << 1.0 < -2 <= -11;",
    out.str(),
    "1\n-1\n-3\n-5\n-7\n-9\n-11\n"
  );
  out.str("");
  out.clear();
}








ARGGLIB__DEFINE_TEST(func_test_pipe_test21211) {

	std::vector<double> vec = { 1,2,4,5 };

	auto x =  vec | contains(1);
	auto x1 = vec | contains_any({ 10,20 });
	auto x2 = vec | contains_all({ 1,20 });
	auto x3 = vec | contains_if([](cautor x) { return x > 3; });

	auto x4 = vec | contains_if([](cautor x) { return x > 30; });

}


ARGGLIB__DEFINE_TEST(func_test_pipe_test12) {


	std::map<std::string, double> map_in;
	map_in["dsad0"] = 0;
	map_in["dsad1"] = 2;
	map_in["dsad3"] = 3;
	map_in["dsad4"] = -5454;


	auto vector_out = map_in | to_vector() | ArggLib_sort_by(second);
	std::stringstream out;
	//map_in | ArggLib::out_stream(out);
	vector_out | ArggLib::for_loop() >> ArggLib::out_stream(out);
	___ARGGLIB_TEST("Function pipe operator",
		out.str(),
		"first: {dsad4} second: {-5454}\nfirst: {dsad0} second: {0}\nfirst: {dsad1} second: {2}\nfirst: {dsad3} second: {3}\n"
	);


	auto x_true = make_vec({ 1,2,5,6 }) | ArggLib::contains(1);

	___ARGGLIB_TEST("Function contains true case",
		x_true,
		true
	);
	auto x_false = make_vec({ 1,2,5,6 }) | ArggLib::contains(100);

	___ARGGLIB_TEST("Function contains false case",
		x_false,
		false
	);

}





ARGGLIB__DEFINE_TEST(func_test_pipe_test122) {


	std::map<std::string, double> map_in;
	map_in["dsad0"] = 0;
	map_in["dsad1"] = 2;
	map_in["dsad3"] = 3;
	map_in["dsad4"] = -5454;

	std::stringstream out;
	map_in | to_string_f().delimiter("\n") | write_out(out);

	___ARGGLIB_TEST("map_in | to_string().delimiter(\"\\n\") | write_out(out);",
		out.str(),
		"dsad0  0.000000\ndsad1  2.000000\ndsad3  3.000000\ndsad4  -5454.000000\n"
	);
	out.str("");
	out.clear();


	auto vector_out = map_in | to_vector() | ArggLib_sort_by(second);

	auto vec = _to_vector << 1 << 10;
	auto s1 = vector_out | to_string_f()  | write_out(out);
	___ARGGLIB_TEST("auto s1 = vector_out | to_string()  | write_out(out);",
		out.str(),
		"dsad4  -5454.000000dsad0  0.000000dsad1  2.000000dsad3  3.000000\n"
	);

	___ARGGLIB_TEST("auto s1 = vector_out | to_string()  | write_out(out);",
		s1,
		"dsad4  -5454.000000dsad0  0.000000dsad1  2.000000dsad3  3.000000"
	);
	out.str("");
	out.clear();


	vector_out | to_string_f().delimiter("\n").header("<header>\n").footer("\n</header>") | write_out(out);


	___ARGGLIB_TEST("vector_out | to_string().delimiter(\"\\n\").header(\"<header>\\n\").footer(\"\\n< / header>\") | write_out(out);",
		out.str(),
		"<header>\ndsad4  -5454.000000\ndsad0  0.000000\ndsad1  2.000000\ndsad3  3.000000\n</header>\n"
	);

}




ARGGLIB__DEFINE_TEST(func_test_pipe_test12212) {


	std::map<std::string, double> map_in;
	map_in["dsad0"] = 0;
	map_in["dsad1"] = 2;
	map_in["dsad3"] = 3;
	map_in["dsad4"] = -5454;

	std::stringstream out;



	auto vector_out = map_in | to_vector() | sort().by_second();

	int i = 2;
	auto s1 = vector_out | _where_f( _x.second > i; ) |   to_string_f() | write_out(out);
	___ARGGLIB_TEST("auto s1 = vector_out | to_string()  | write_out(out);",
		out.str(),
		"dsad4  -5454.000000dsad0  0.000000dsad1  2.000000\n"
	);


	auto vec = _to_vector << 1 <= -10;

	_to_vector << 10 <= -10  | sort() | display();


	

	_to_vector << -10 << 10  | sort().descending().absulute() | display();
}