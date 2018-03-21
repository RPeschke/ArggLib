// PipeLines.cpp : Defines the entry point for the console application.
//

#include "ArggLib/proc.hh"
#include "ArggLib/param.hh"
#include "ArggLib/proc_tools.hh"

#include <sstream>
#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/for_loop.hh"
#include "ArggLib/out_stream.hh"
#include "ArggLib/Evaluate.hh"
#include "ArggLib/Drop.hh"
#include "ArggLib/Modify.hh"
#include "ArggLib/Where.hh"
#include "ArggLib/has_member_helper.hh"
#include "ArggLib/OnEnd.hh"
#include "ArggLib/impl_do_begin_do_end.hh"
#include "ArggLib/OnStart.hh"
#include "ArggLib/constexpr_if.hh"
#include "ArggLib/Export_CSV.hh"
#include "ArggLib/convert2HashTable.hh"
#include "ArggLib/Import_CSV.hh"
#include "ArggLib/named_variable.hh"
#include <string>
#include <iostream>
#include "ArggLib/validated_variable.hh"





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

}