// functional_programming.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//


#include <vector>
#include <algorithm>
#include <numeric>
#include <ostream>
#include <iostream>
#include  <functional>
#include <string>


#include "ArggLib/proc.hh"
#include "ArggLib/func.hh"
#include "ArggLib/proc.hh"
#include "ArggLib/param.hh"
#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/has_member_helper.hh"
#include <sstream>

using namespace std;
using namespace ArggLib;

#define  EXAMPLE 1




ARGGLIB__DEFINE_TEST(func_test1) {
	std::stringstream out;
  auto disp= [&out](auto&& e) {

		out << e << std::endl;
  };

  auto square_ = [](auto&& e) {
    return e*e;
  };

//   auto ttt = (fun() << square_ << 7 <<= _run());
//    auto p=param() << 1 << 1;
   auto f =  fun() << square_ << 7;
  auto xx=  param()|  f;
  auto xxx = param() | fun() << square_ << 8;
  auto xxx1 = param() | fun() << square_ << 8 >  fun() << get_ints3 << 1 <<_X <<1;

   auto f1 = fun() << square_;

   auto in_ =  param();
 //  decltype(_____try_if_arg_has_return_type(__is_not_param(in_)), f1(in_)) x2;
  auto i =  7 | f1;

  // __is_not_param(1);

   auto p = param();
   decltype(__is_not_param(param()),1) x1;
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










