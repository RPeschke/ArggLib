#include "ArggLib/complete_lib.hh"


#include <sstream>
#include <string>
#include <iostream>

#include "ArggLib/try_run_default.hh"
#include "ArggLib/onEnd2.hh"

#include "ArggLib/proc_fill_obj.hh"
#include "ArggLib/async_for_loop.hh"
#include "ArggLib/impl_do_begin_do_end.hh"

using namespace std;

using namespace ArggLib;





DEFINE_PROC1(loop, nextP, inPut) {

  for (double i = 0; i < inPut; ++i) {

    nextP(i);
  }
  return success;
}

DEFINE_PROC1(disp_, nextP, inPut) {

  std::cout << inPut;
  return nextP(inPut);
}


DEFINE_PROC_V(wSpace, nextP, inPut) {

  std::cout << "  ";
  return nextP(inPut...);
}

DEFINE_PROC1(square2, nextP, inPut) {


  return nextP(inPut*inPut);
}
auto make_loop() {
  return proc() >> loop();
}
class wSpace1 {
public:
  wSpace1(int w = 5, std::ostream& out = std::cout) :m_w(w), m_out(out) {}
  template < typename NEXT_T, typename... BLOCKS_T>
  procReturn operator()(NEXT_T&& nextP, BLOCKS_T&&... inPut) const {
    m_out << std::string(m_w, ' ');
    return nextP(inPut...);
  }
  const int m_w;
  std::ostream& m_out;
};


ARGGLIB__DEFINE_TEST(processor_test14) {
  std::stringstream out;
  int i = 2;
  auto disp2 = [&](auto& next_, auto& input) {
    i += input;
    out << i << std::endl;
    return next_(input);
  };

  auto newLine = [&](auto& next_, auto& input) {

    out << "\n";
    return next_(input);
  };
  auto t = make_loop();

  auto t1 = t >> (stop()) + (disp_());

  auto tt = loop() >> stop()
    + (out_stream(out) >> wSpace1(10, out) >> loop() >> out_stream(out) >> stop())
    + newLine >> stop()
    + (out_stream(out) >> wSpace1(5, out) >> square2() >> out_stream(out) >> newLine)
    >> disp2 >> newLine;

  5 | tt;


  ___ARGGLIB_TEST("stream out", out.str(), "0\n          \n0\n     0\n\n2\n\n1\n          0\n\n1\n     1\n\n3\n\n2\n          0\n1\n\n2\n     4\n\n7\n\n3\n          0\n1\n2\n\n3\n     9\n\n16\n\n4\n          0\n1\n2\n3\n\n4\n     16\n\n32\n\n");
  out.str("");
  out.clear();
  tt.m_pro(stop(), 5);
  ___ARGGLIB_TEST("stream out", out.str(), "0\n          \n0\n     0\n\n32\n\n1\n          0\n\n1\n     1\n\n33\n\n2\n          0\n1\n\n2\n     4\n\n37\n\n3\n          0\n1\n2\n\n3\n     9\n\n46\n\n4\n          0\n1\n2\n3\n\n4\n     16\n\n62\n\n");
}


ARGGLIB__DEFINE_TEST(processor_test15) {

  std::stringstream out;
  // cout << "  param() | for_loop(2,20,3) >> \n   (_where{ return _x % 2 == 0; } >> wSpace1(10,out) >> out_stream(out))\n  + (_where{ return _x % 2 == 1; } >> wSpace1(5,out) >> out_stream(out)) ;\n";
  param() | for_loop(2, 20, 3) >>
    (_where{ return _x % 2 == 0; } >> wSpace1(10, out) >> out_stream(out))
    + (_where{ return _x % 2 == 1; } >> wSpace1(5, out) >> out_stream(out));

  ___ARGGLIB_TEST("stream out", out.str(), "          2\n     5\n          8\n     11\n          14\n     17\n");
  //cout << "==================\n\n";
}



ARGGLIB__DEFINE_TEST(validated_variable_test) {



  auto x = make_validated_variable(100, [](auto) { return true; });
  auto x1 = make_validated_variable(100);

  try
  {
    auto x2 = make_validated_variable(100, [](auto x) {return x > 200; });

  }
  catch (const std::exception& e)
  {
    std::string errormsg = e.what();
    ___ARGGLIB_TEST("validated_variable_1", errormsg, "Argument: '100' is not valid as an Argument for validated_variable.");
  }

  x1 = 200;


  auto x2 = make_validated_variable(300, [](auto x) {return x > 200; });
  try
  {
    x2 = 100;

  }
  catch (const std::exception& e)
  {
    std::string errormsg =  e.what();
    ___ARGGLIB_TEST("validated_variable_2", errormsg, "Argument: '100' is not valid as an Argument for validated_variable.");
  }

  int i = x1;
  ArggLib::double_positive x4 = 100;
//  ArggLib::double_positive x5 = -100;

}



ARGGLIB__DEFINE_TEST(try_run_or_default_End) {
	{
		std::stringstream out;
		auto x = OnEnd2([&out] { out << "Called end sucessfully\n"; });

		auto x13 = try_run_or_default_End(123, x.m_pro, 321);
		___ARGGLIB_TEST("try_run_or_default_End", out.str(), "Called end sucessfully\n");
		___ARGGLIB_TEST("try_run_or_default_End", x13, 123);
	}

	{
		std::stringstream out;
		auto x = OnEnd2([&out] { out << "Called end sucessfully\n"; return 1; });

		auto x13 = try_run_or_default_End(123, x.m_pro, 321);
		___ARGGLIB_TEST("try_run_or_default_End", out.str(), "Called end sucessfully\n");
		___ARGGLIB_TEST("try_run_or_default_End", x13, 1);
	}


	{
		std::stringstream out;
		auto x = OnEnd3([&out] (auto i) { out << "Called end sucessfully\n"<< i << "\n"; });

		auto x13 = try_run_or_default_End(123, x.m_pro, 321);
		___ARGGLIB_TEST("try_run_or_default_End", out.str(), "Called end sucessfully\n321\n");
		___ARGGLIB_TEST("try_run_or_default_End", x13, 123);
	}

	{
		std::stringstream out;
		auto x = OnEnd3([&out](auto i) { out << "Called end sucessfully\n" << i << "\n"; return 1; });

		auto x13 = try_run_or_default_End(123, x.m_pro, 321);
		___ARGGLIB_TEST("try_run_or_default_End", out.str(), "Called end sucessfully\n321\n");
		___ARGGLIB_TEST("try_run_or_default_End", x13, 1);
	}

	{
		std::stringstream out;
		auto x = OnEnd3([&out](auto i) { out << "Called end sucessfully\n" << i << "\n"; return 1; });

		auto x13 = try_run_or_default_End(123, x, 321);
		___ARGGLIB_TEST("try_run_or_default_End", out.str(), "");
		___ARGGLIB_TEST("try_run_or_default_End", x13, 123);
	}
}




ARGGLIB__DEFINE_TEST(try_run_or_default_functor) {
	{
		std::stringstream out;
		auto x = [&out] { out << "Called end sucessfully\n"; };

		auto x13 = try_run_or_default_functor(123, x, 321);
		___ARGGLIB_TEST("try_run_or_default_functor", out.str(), "Called end sucessfully\n");
		___ARGGLIB_TEST("try_run_or_default_functor", x13, 123);
	}

	{
		std::stringstream out;
		auto x = [&out] { out << "Called end sucessfully\n"; return 1; };

		auto x13 = try_run_or_default_functor(123, x, 321);
		___ARGGLIB_TEST("try_run_or_default_functor", out.str(), "Called end sucessfully\n");
		___ARGGLIB_TEST("try_run_or_default_functor", x13, 1);
	}


	{
		std::stringstream out;
		auto x = [&out](auto i) { out << "Called end sucessfully\n" << i << "\n"; };

		auto x13 = try_run_or_default_functor(123, x, 321);
		___ARGGLIB_TEST("try_run_or_default_functor", out.str(), "Called end sucessfully\n321\n");
		___ARGGLIB_TEST("try_run_or_default_functor", x13, 123);
	}

	{
		std::stringstream out;
		auto x = [&out](auto i) { out << "Called end sucessfully\n" << i << "\n"; return 1; };

		auto x13 = try_run_or_default_functor(123, x, 321);
		___ARGGLIB_TEST("try_run_or_default_functor", out.str(), "Called end sucessfully\n321\n");
		___ARGGLIB_TEST("try_run_or_default_functor", x13, 1);
	}

	{
		std::stringstream out;
		auto x = [&out](auto i) { out << "Called end sucessfully\n" << i << "\n"; return 1; };

		auto x13 = try_run_or_default_functor(123, "no Functor", 321);
		___ARGGLIB_TEST("try_run_or_default_functor", out.str(), "");
		___ARGGLIB_TEST("try_run_or_default_functor", x13, 123);
	}
}

ARGGLIB__DEFINE_TEST(test_end_unrole) {
	


	auto i = ArggLib_impl::do_end3(12, std::async([] {return 1; }));
	___ARGGLIB_TEST("test_end_unrole", i.get(), 1);

	std::stringstream out;
	auto x = OnEnd2([&out] { out << "Called end sucessfully\n"; });
	auto i2 = ArggLib_impl::do_end3(x,123);
	
	std::cout << i2 << std::endl;

}

ARGGLIB__DEFINE_TEST(where_p_test1) {


  auto x = 10 | for_loop() >> _where_p(_x > 5) >> out_stream();
  *x| to_string_f() | write_host();
	//std::cout << out.str();
	int i = 0;
}



template <typename T1>
struct types_of_f {

  T1 m_fun;

  auto operator()() {
    return m_fun();
  }
};

template <typename T1>
auto make_types_of_f(T1 f ) {
  return types_of_f<T1>{f};
}




#define MAKE_TRY_RUN_FUNCION 



ARGGLIB__DEFINE_TEST(where_p_test21) {


	"test.csv" | Import_CSV() >>  Where([i = int(0)](auto...) mutable { return i++ < 10;  }) >> display()->delimiter(" del ");
	"fileNames.txt" | Import_CSV()>> display();
	auto s = "fileNames.txt" | Import_CSV() >> Import_CSV() >> display()->delimiter(" del ");

  param() | Import_CSV()->fileName("fileNames.txt") >> display();
//	std::cout << s.str() << std::endl;
	//std::cout << out.str();
	int i = 0;
  auto x =  OnEnd2([] { std::cout << "SDAA\n";  });
 // auto x12 = try_call_fnction3(123, x.m_pro,321);

  auto x12 = try_run_or_default_End(111, x.m_pro, 1.2);
  auto x121 = try_run_or_default_End(111, x, 1.2);
  auto x13 = ArggLib_impl::try_run_or_default_End_make_types_try(123, x.m_pro, 321);
  auto x14 =  try_call_fnction4(x13);
  auto r = [x]() mutable {return   x.m_pro.End(); };
  r();
  auto r2 = make_types_of_f(r);
  auto xxx = std::is_same_v< std::result_of_t<decltype(x13)()>,int> ;
  auto x2 = std::is_same_v< std::result_of_t<decltype(x13)()>, void>;
  ArggLib_impl::unfold_end(OnEnd2([] { std::cout << "SDAA\n"; }));

  auto x6 = OnEnd3([](double x) { 
    return x; 
  });
  std::cout << try_run_or_default_End(111, x6.m_pro, 1.2) << std::endl;

  std::cout << try_run_or_default_functor(123, [](auto e) {
    std::cout << e<< "\nasdasd\n";
    return 3324;
  }, 123) << std::endl;



}


ARGGLIB__DEFINE_TEST(fill_test) {
	auto x1 = 10 | for_loop() >> out_stream();
	std::cout << "<>\n ";
	std::future<int> i;
	//auto i1 = is_future(i);
	ArggLib_impl::do_end3(1, std::move(i));

	int i3 = ArggLib::is_future_type<int>::value;
	int i4 = ArggLib::is_future_type<std::future<int>>::value;
	auto i2 = std::enable_if_t< decltype(is_future(declval<	std::future<int> >() ))::value , int>(0) ;
    auto x =  10 | for_loop() >> "adadas" >> 1.2;
	//auto x1 = 10 | proc_async()>>  for_loop() >> display();
	std::cout << x <<  "\n</>\n ";
}


