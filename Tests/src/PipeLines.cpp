// PipeLines.cpp : Defines the entry point for the console application.
//

#include "ArggLib/proc.hh"
#include "ArggLib/param.hh"
#include "ArggLib/proc_tools.hh"

#include <sstream>
#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/for_loop.hh"
#include "ArggLib/Fill.hh"
#include "ArggLib/Evaluate.hh"
#include "ArggLib/Drop.hh"
#include "ArggLib/Modify.hh"
#include "ArggLib/Where.hh"
#include "ArggLib/has_member_helper.hh"
#include "ArggLib/OnEnd.hh"
#include "ArggLib/impl_do_begin_do_end.hh"
#include "ArggLib/OnStart.hh"
#include "ArggLib/constexpr_if.hh"






using namespace std;

using namespace ArggLib;








ARGGLIB__DEFINE_TEST( processor_test1)
{
	





//	param() << 0 << 10 << 2 | proc() >> for_loop();// >> disp;
	
	cout << "display the numbers from 0 to 10\n";
	param() << 0 << 10 | proc() >> ArggLib::for_loop() >> ArggLib::display();
	cout << "==================\n\n";

	cout << "display the numbers from 0 to 10 step 2 \n";
	param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::display();
	cout << "==================\n\n";
	cout << "display the numbers from 0 to 10 step 2 \n";
	param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::Fill(std::cout);
	cout << "==================\n\n";


	cout << "Streaming the numbers from 0 to 10 step 2 to out stream \n";

	std::stringstream out;
	param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::Fill(out);

	std::string dummy(out.str());
	__Test("stream out", dummy, "0\n2\n4\n6\n8\n");

	cout << "==================\n\n";


	    
}


ARGGLIB__DEFINE_TEST(processor_test2) {

	cout << "Streaming the numbers 1,2,5,6 to out stream \n";
	std::stringstream out;
	make_vec({ 1,2,5,6 }) | proc() >> ArggLib::for_loop() >> ArggLib::Fill(out);
	__Test("stream out", out.str(), "1\n2\n5\n6\n");
	cout << "==================\n\n";

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


void _Fill(std::vector<t1>& vec, double x, double y) {
	vec.push_back(t1{ x, y });
}
ARGGLIB__DEFINE_TEST(processor_test3) {
	std::vector<t1> vec;

	cout << "Streaming the numbers 10 x 10  to out stream \n";
	param() | proc() >> for_loop(10) >> for_loop(10) >> ArggLib::Fill(vec);

	vec | proc() >> ArggLib::for_loop() >> square()>>  ArggLib::drop<0>()>> ArggLib::display();
	cout << "==================\n\n";
}

double test_times_two(double x) {
	return x * 2;
}

ARGGLIB__DEFINE_TEST(processor_test4) {
	std::vector<t1> vec;

	int i = 0;
	auto f = [&i](auto&e) { i++; return e*e; };
	auto f2 = [&i](auto&e,auto& e1) { i++; return e*e1; };

	cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify(f2) >> ArggLib::Fill(cout);\n";
	param() | proc() >> for_loop(10) >> for_loop(10) >> Modify(f2) >> ArggLib::Fill(cout);
	cout << "==================\n\n";


	cout << "==================\n" << "param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2; }) >> Evaluate(f) >> ArggLib::Fill(cout);\n";
	param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2==0; }) >> Evaluate(f) >> ArggLib::Fill(cout);
	cout << "==================\n\n";


	cout << "==================\n" << "param() | proc() >> for_loop(10) >> Evaluate(&test_times_two) >> ArggLib::Fill(vec);\n";
	param() | proc() >> for_loop(10) >> Evaluate(&test_times_two) >> ArggLib::Fill(vec);
	cout << "==================\n\n";
	
	cout << "==================\n" << "vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::display();\n";
	vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::display();
	cout << "==================\n\n";
}




ARGGLIB__DEFINE_TEST(processor_test5) {
	cout << "==================\n" << "	auto p = proc() >> OnEnd([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnEnd([] { cout << \"3\\n\"; }) >> OnEnd([] { cout << \"4\\n\"; });\n\n";
	auto p = proc() >> OnEnd([] { cout << "1\n"; }) >> OnEnd([] { cout << "2\n"; }) >> OnEnd([] { cout << "3\n"; }) >> OnEnd([] { cout << "4\n"; });
	ArggLib_impl::unfold_end(p.m_pro);
	cout << "==================\n\n";
}


ARGGLIB__DEFINE_TEST(processor_test6) {
	cout << "==================\n" << "	auto p = proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnStart([] { cout << \"2\\n\"; }) >> OnStart([] { cout << \"3\\n\"; }) >> OnStart([] { cout << \"4\\n\"; });\n\n";
	auto p = proc() >> OnStart([] { cout << "1\n"; }) >> OnStart([] { cout << "2\n"; }) >> OnStart([] { cout << "3\n"; }) >> OnStart([] { cout << "4\n"; });
	ArggLib_impl::unfold_Start(p.m_pro);
	cout << "==================\n\n";
}

int test() {
	return 1;
}

void test2() {

}
ARGGLIB__DEFINE_TEST(processor_test7) {
	cout << "==================\n" << "param() | proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnStart([] {cout << \"3\\n\"; }) >> OnEnd([] {cout << \"4\\n\"; }) >> Evaluate([] { cout << \"eval\\n\"; return 1; });\n\n";
	param() | proc() >> OnStart([] { cout << "1\n"; }) >> OnEnd([] { cout << "2\n"; }) >> OnStart([] {cout << "3\n"; }) >> OnEnd([] {cout << "4\n"; }) >> Evaluate([] { cout << "eval\n"; return 1; });
	
	param() | proc()
		>> for_loop(10)
		>> OnStart([] { cout << "1\n"; })
		>> OnEnd([] { cout << "2\n"; })
		>> OnStart([] {cout << "3\n"; })
		>> OnEnd([] {cout << "4\n"; })
		//		>>drop<0>()
		>> Evaluate([](auto i) { cout << "eval\n";  return i*i; })
		>> display();
	

	cout << "==================\n\n";

}




