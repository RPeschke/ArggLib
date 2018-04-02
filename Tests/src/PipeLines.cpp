// PipeLines.cpp : Defines the entry point for the console application.
//



#include <sstream>
#include <string>
#include <iostream>


#include "ArggLib/complete_lib.hh"




using namespace std;

using namespace ArggLib;








ARGGLIB__DEFINE_TEST(processor_test1)
{

  std::stringstream out;






  //	param() << 0 << 10 << 2 | proc() >> for_loop();// >> disp;


  param() << 0 << 10 | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("display the numbers from 0 to 10", out.str(), "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n");
  out.str("");
  out.clear();






  param() << 0 << 10 << 2 | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("display the numbers from 0 to 10 step 2", out.str(), "0\n2\n4\n6\n8\n");
  out.str("");
  out.clear();



  param() << 0 << 10 << 2 | ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("display the numbers from 0 to 10 step 2 ", out.str(), "0\n2\n4\n6\n8\n");
  out.str("");
  out.clear();




  param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::out_stream(out);
  std::string dummy(out.str());
  ___ARGGLIB_TEST("Streaming the numbers from 0 to 10 step 2 to out stream", dummy, "0\n2\n4\n6\n8\n");
  out.str("");
  out.clear();



}



ARGGLIB__DEFINE_TEST(processor_test2) {


  std::stringstream out;
  make_vec({ 1,2,5,6 }) | proc() >> ArggLib::for_loop() >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("Streaming the numbers 1,2,5,6 to out stream ",
    out.str(),
    "1\n2\n5\n6\n");


}


struct t1
{
  double x, y;

};

std::ostream& operator<<(std::ostream& out, t1& t) {
  out << "x: " << t.x << " y: " << t.y << std::endl;
  return out;
}

double operator*(t1& x, t1& y) {
  return x.x*y.x + x.y*y.y;
}


void _Fill(std::vector<t1>& vec, const std::string& delimer, double x, double y) {
  vec.push_back(t1{ x, y });
}
ARGGLIB__DEFINE_TEST(processor_test3) {
  std::vector<t1> vec;
  std::stringstream out;

  param() | proc() >> for_loop(10) >> for_loop(10) >> ArggLib::out_stream(vec);

  int i = 0;
  vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> _where{ return i++ < 10; } >> out_stream(out);
  ___ARGGLIB_TEST("stream out",
    out.str(),
    "0\n1\n4\n9\n16\n25\n36\n49\n64\n81\n");

}

double test_times_two(double x) {
  return x * 2;
}

ARGGLIB__DEFINE_TEST(processor_test4) {
  std::vector<t1> vec;
  std::stringstream out;
  int i = 0;
  auto f = [&i](auto&e) { i++; return e * e; };
  auto f2 = [&i](auto&e, auto& e1) { i++; return e * e1; };


  param() | proc() >> for_loop(5) >> for_loop(5) >> Modify(f2) >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("param() | proc() >> for_loop(10) >> for_loop(10) >> Modify(f2) >> ArggLib::Fill(cout);",
    out.str(),
    "0\n0\n0\n0\n0\n0\n1\n2\n3\n4\n0\n2\n4\n6\n8\n0\n3\n6\n9\n12\n0\n4\n8\n12\n16\n");
  out.str("");
  out.clear();





  param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2 == 0; }) >> Evaluate(f) >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2; }) >> Evaluate(f) >> ArggLib::Fill(cout);",
    out.str(),
    "0  0\n2  4\n4  16\n6  36\n8  64\n");

  out.str("");
  out.clear();




  param() | proc() >> for_loop(10) >> Evaluate(&test_times_two) >> ArggLib::out_stream(vec);



  vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::out_stream(out);

  ___ARGGLIB_TEST("vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::display();",
    out.str(),
    "0\n5\n20\n45\n80\n125\n180\n245\n320\n405\n");

  out.str("");
  out.clear();

}







ARGGLIB__DEFINE_TEST(processor_test5) {
  std::stringstream out;

  auto p = proc() >> OnEnd([&] { out << "1\n"; }) >> OnEnd([&] { out << "2\n"; }) >> OnEnd([&] { out << "3\n"; }) >> OnEnd([&] { out << "4\n"; });
  ArggLib_impl::unfold_end(p.m_pro);

  ___ARGGLIB_TEST("auto p = proc() >> OnEnd([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnEnd([] { cout << \"3\\n\"; }) >> OnEnd([] { cout << \"4\\n\"; });",
    out.str(),
    "1\n2\n3\n4\n");

  out.str("");
  out.clear();

}


ARGGLIB__DEFINE_TEST(processor_test6) {
  std::stringstream out;

  auto p = proc() >> OnStart([&] { out << "1\n"; }) >> OnStart([&] { out << "2\n"; }) >> OnStart([&] { out << "3\n"; }) >> OnStart([&] { out << "4\n"; });
  ArggLib_impl::unfold_Start(p.m_pro);

  ___ARGGLIB_TEST("auto p = proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnStart([] { cout << \"2\\n\"; }) >> OnStart([] { cout << \"3\\n\"; }) >> OnStart([] { cout << \"4\\n\"; })", out.str(), "4\n3\n2\n1\n");
  out.str("");
  out.clear();

}


ARGGLIB__DEFINE_TEST(processor_test7) {
  // cout << "==================\n" << "param() | proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnStart([] {cout << \"3\\n\"; }) >> OnEnd([] {cout << \"4\\n\"; }) >> Evaluate([] { cout << \"eval\\n\"; return 1; });\n\n";
  std::stringstream out;

  param() | proc() 
	  >> OnStart([&]  { out << "1\n"; }) 
	  >> OnEnd([&] ( ) { out << "2\n"; }) 
	  >> OnStart([&] {out << "3\n"; }) 
	  >> OnEnd([&]() {out << "4\n"; })
	  >> Evaluate([&] { out << "eval\n"; return 1; });

  ___ARGGLIB_TEST("param() | proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnStart([] {cout << \"3\\n\"; }) >> OnEnd([] {cout << \"4\\n\"; }) >> Evaluate([] { cout << \"eval\\n\"; return 1; });", out.str(), "3\n1\neval\n2\n4\n");

  out.str("");
  out.clear();


  param() | proc()
    >> for_loop(10)
    >> OnStart([&] { out << "1\n"; })
    >> OnEnd([&] { out << "2\n"; })
    >> OnStart([&] {out << "3\n"; })
    >> OnEnd([&] {out << "4\n"; })
    //		>>drop<0>()
    >> Evaluate([&](auto i) { out << "eval\n";  return i * i; })
    >> drop<-2>()
    >> ArggLib::out_stream(out);


  ___ARGGLIB_TEST("param() | proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnStart([] {cout << \"3\\n\"; }) >> OnEnd([] {cout << \"4\\n\"; }) >> Evaluate([] { cout << \"eval\\n\"; return 1; });", out.str(), "3\n1\neval\n0\neval\n1\neval\n4\neval\n9\neval\n16\neval\n25\neval\n36\neval\n49\neval\n64\neval\n81\n2\n4\n");



}




ARGGLIB__DEFINE_TEST(processor_test8) {

  //cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> for_loop(10) >> Export_CSV("test.csv");

  //cout << "==================\n\n";

  //cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test2.csv\", { \"x\",\"y\",\"z\" }););\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> for_loop(10) >> Export_CSV("test2.csv", { "x","y","z" }, ":  ");

  //cout << "==================\n\n";

  //cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test2.csv\", { \"x\",\"y\",\"z\" }););\n\n";
  param() | proc() >> for_loop(10) >> Export_CSV("test3.csv", ":  ", { "x" });

  //cout << "==================\n\n";
}


ARGGLIB__DEFINE_TEST(processor_test9) {

  //cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret["x"] = x; ret["y"] = y;  return ret; }) >> Export_CSV("test4.csv");


  //cout << "==================\n\n";


}


ARGGLIB__DEFINE_TEST(processor_test10) {

  //cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> convert2HashTable<double>() >> Export_CSV("test4.csv");


  //cout << "==================\n\n";


}

ARGGLIB__DEFINE_TEST(processor_test11) {


  std::stringstream out;
  10 | proc() >> for_loop() >> Evaluate([](auto i) {return i * i; }) >> convert2HashTable({ "x","y" }) >> Export_CSV("test5.csv") >> out_stream(out);

  ___ARGGLIB_TEST("  10 | proc() >> for_loop() >> Evaluate([](auto i) {return i * i; }) >> convert2HashTable({ \"x\",\"y\" }) >> Export_CSV(\"test5.csv\") >> out_stream(out);",
    out.str(),
    "x  y\n0  0\n1  1\n2  4\n3  9\n4  16\n5  25\n6  36\n7  49\n8  64\n9  81\n");




}


ARGGLIB__DEFINE_TEST(processor_test12) {

  std::stringstream out;
  int i = 0;
  param() | Import_CSV("test4.csv", ';') >> Where([&i](auto...) { return i++ < 10;  }) >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("param() | Import_CSV(\"test4.csv\", ';') >> Where([&i](auto...) { return i++ < 10;  }) >> ArggLib::out_stream(out) ;",
    out.str(),
    "0  0\n0  1\n0  2\n0  3\n0  4\n0  5\n0  6\n0  7\n0  8\n0  9\n");

  out.str("");
  out.clear();
  i = 0;
  param() | Import_CSV_as_HashTable("test4.csv", ';') >> Where([&i](auto...) { return i++ < 10;  }) >> ArggLib::out_stream(out);
  ___ARGGLIB_TEST("param() | Import_CSV_as_HashTable(\"test4.csv\", ';') >> Where([&i](auto...) { return i++ < 10;  }) >> ArggLib::out_stream(out) ;",
    out.str(),
    "x  y\n0  0\n0  1\n0  2\n0  3\n0  4\n0  5\n0  6\n0  7\n0  8\n0  9\n");



}


template <typename FunBase_t>
class  fun_class1 {
public:
  FunBase_t m_funB;

  fun_class1(FunBase_t&& funB) : m_funB(std::move(funB)) {}

};

template <typename T>
auto make_fun_class(T&& t) {
  return fun_class1<T>(std::move(t));
}

template <typename FunBase_t, typename Fun_t>
class  fun_class {
public:
  FunBase_t m_funB;
  Fun_t m_fun;
  fun_class(FunBase_t funB, Fun_t fun) : m_funB(std::move(funB)), m_fun(std::move(fun)) {}

};

template <typename T, typename T1>
auto make_fun_class(T t, T1 t1) {
  return fun_class<T, T1>(std::move(t), std::move(t1));
}


template <typename T, typename F>
auto operator<<(fun_class1<T> t, F f) {
  return make_fun_class(t.m_funB, [t, f](auto i) { return t.m_funB(f, i); });
}


template <typename T, typename T1, typename F>
auto operator<<(fun_class<T, T1> t, F f) {
  return make_fun_class(t.m_funB, [f, t](auto i1) {return t.m_funB(f, t.m_fun(i1)); });
}

template <typename F, typename T, typename T1>
auto operator<<=(const F& f, fun_class<T, T1> t) {

  return t.m_fun(f);
}

template <typename F, typename T, typename T1>
auto operator<<=(F& f, fun_class<T, T1> t) {
  f = t.m_fun(f);
  return f;
}

auto _plus() {
  return  make_fun_class([](auto i1, auto i2) { return i1 + i2; });
}

auto _max() {
  return  make_fun_class([](auto i1, auto i2) { return  std::max(i1, i2); });
}


void test() {
  auto i = make_fun_class([](auto i1, auto i2) { return i1 + i2; });
  int j = 0;
  auto x = j <<= _max() << 1 << 2 << 3;
  auto x1 = 4 <<= _max() << 1 << 2 << 3;
}
ARGGLIB__DEFINE_TEST(processor_test13) {
  std::stringstream out;

  int iii = 0;


  param() | Import_CSV_as_named_variables("test4.csv", ';') >> Where([&](auto...) { return ++iii < 10; }) >> out_stream(out);
  ___ARGGLIB_TEST("Import_CSV_as_named_variables(\"test4.csv\", ';') >> Where([&](auto...) { return ++iii < 10; }) >> out_stream(out);",
    out.str(),
    "x: {0}  y: {0}\nx: {0}  y: {1}\nx: {0}  y: {2}\nx: {0}  y: {3}\nx: {0}  y: {4}\nx: {0}  y: {5}\nx: {0}  y: {6}\nx: {0}  y: {7}\nx: {0}  y: {8}\n");
  out.str("");
  out.clear();




  iii = 0;
  param() | Import_CSV_as_HashTable("test4.csv", ';') >> Where([&](auto...) { return ++iii < 10; }) >> out_stream(out);
  ___ARGGLIB_TEST("param() | Import_CSV_as_HashTable(\"test4.csv\", ';') >> out_stream(out);",
    out.str(),
    "x  y\n0  0\n0  1\n0  2\n0  3\n0  4\n0  5\n0  6\n0  7\n0  8\n");
  out.str("");
  out.clear();

  auto x = Import_CSV(';')->delimiter(':');

}

