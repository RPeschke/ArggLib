#include "ArggLib/complete_lib.hh"


#include <sstream>
#include <string>
#include <iostream>






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



ARGGLIB__DEFINE_TEST(where_p_test1) {


  auto x = 10 | for_loop() >> _where_p(_x > 5) >> out_stream();
  *x| to_string_f() | write_host();
	//std::cout << out.str();
	int i = 0;
}


template<typename T>
class OnEnd_impl2 {
public:
  T m_fun;

  OnEnd_impl2(T fun) :m_fun(std::move(fun)) {

  }

  auto End() {
  
    return   m_fun();
  }

  template <typename NEXT_T, typename... ARGS>
  procReturn operator()(NEXT_T&& next, ARGS&&... args) {


    return next(std::forward<ARGS>(args)...);
  }


};


template <typename T>
auto OnEnd2(T fun) ->decltype(proc() >> OnEnd_impl2<T>(std::move(fun))) {
  return proc() >> OnEnd_impl2<T>(std::move(fun));
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




template <typename T, typename... N>
auto return_first(T&& t, N&&... n) {
  return t;
}

template <typename... N>
auto try_call_fnction2(N&&... n) {
  return return_first(n...);
}




template <typename default_T, typename Fun_t, typename ARG>
auto  try_call_fnction2(default_T&& def, Fun_t&& fun, ARG && arg) -> decltype(fun.End()) {

  return fun.End();

}


template <typename... N>
auto try_call_fnction3(N&&... n) {
  return try_call_fnction2(n...);
}




template <typename default_T, typename Fun_t, typename ARG>
auto  try_call_fnction3(default_T&& def, Fun_t&& fun , ARG && arg) -> decltype(fun.End(arg)) {

  return fun.End(def);

}

template <typename default_T, typename Fun_t, typename ARG>
struct types_of_f1 {
  default_T def;
  Fun_t fun;
  ARG arg;


  auto operator()() {
    return try_call_fnction3( def,  fun,  arg);
  }
};


template <typename default_T, typename Fun_t, typename ARG>
auto make_types_of_f(default_T&& def, Fun_t&& fun, ARG && arg) {
  return types_of_f1<ArggLib::remove_cvref_t< default_T>, ArggLib::remove_cvref_t<  Fun_t>, ArggLib::remove_cvref_t< ARG > >{def, fun, arg };
}

template <typename T, typename std::enable_if_t<std::is_same_v< std::result_of_t<T()>, void>, int> = 0 >
auto try_call_fnction4(T&& t) {
   t();
   return 123;
}

template <typename T, typename std::enable_if_t<!std::is_same_v< std::result_of_t<T()>, void>, int> = 0 >
auto try_call_fnction4(T&& t) {
  return t();
}


ARGGLIB__DEFINE_TEST(where_p_test21) {


	"test.csv" | Import_CSV() >>  Where([i = int(0)](auto...) mutable { return i++ < 10;  }) >> display()->delimiter(" del ");
	"fileNames.txt" | Import_CSV()>> display();
	auto s = "fileNames.txt" | Import_CSV() >> Import_CSV() >> display()->delimiter(" del ");

  param() | Import_CSV()->fileName("fileNames.txt") >> display();
//	std::cout << s.str() << std::endl;
	//std::cout << out.str();
	int i = 0;
  auto x = OnEnd2([] { std::cout << "SDAA\n";  });
 // auto x12 = try_call_fnction3(123, x.m_pro,321);
  auto x13 = make_types_of_f(123, x.m_pro, 321);
  auto x14 =  try_call_fnction4(x13);
  auto r = [x]() mutable {return   x.m_pro.End(); };
  r();
  auto r2 = make_types_of_f(r);
  auto xxx = std::is_same_v< std::result_of_t<decltype(x13)()>,int> ;
  auto x2 = std::is_same_v< std::result_of_t<decltype(x13)()>, void>;
  ArggLib_impl::unfold_end(OnEnd2([] { std::cout << "SDAA\n"; }));
}
