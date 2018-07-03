#include "ArggLib/complete_lib.hh"

#include "ArggLib/reactive_programming/reactive_backend.hh"
#include "ArggLib/reactive_programming/reactive_variable.hh"
#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/reactive_programming/reactive_signals.hh"
#include "ArggLib/reactive_programming/reactive_entity_base.hh"
#include "ArggLib/reactive_programming/reactive_ports.hh"
#include "ArggLib/reactive_programming/reactive_backend_async.hh"
#include "../include/reactive_test1.hh"


using namespace  ArggLib;
using namespace std;
ARGGLIB__DEFINE_TEST(b_reactive_programming_1) {

	std::stringstream out;
	reactive_backend r;
	r.init();
	reactive_signals<int> x(1, &r);
	reactive_signals<int> y(1, &r);
	std::atomic<int> i =0;
	reactive_processor([&out, &x, &y,&i]() {
		out << "<reactive_processor>\n";
		out << "<x>" << x << "</x>\n";
		out << "<y>\n" << y << "\n</y>\n";
		out << "</reactive_processor>\n";
		i = 1;
	}, x, y);

	x = 2;
	while (i==0){ }
	y = 3;

	r.stop();
	___ARGGLIB_TEST("b_reactive_programming_2", out.str(), "<reactive_processor>\n<x>2</x>\n<y>\n1\n</y>\n</reactive_processor>\n<reactive_processor>\n<x>2</x>\n<y>\n3\n</y>\n</reactive_processor>\n");

}


ARGGLIB__DEFINE_TEST(b_reactive_programming_2) {

	std::stringstream out;
	reactive_backend r;
	r.init();


	r.force_stop(); // just checking if this statement doesn't produce deadlock 
	___ARGGLIB_TEST("b_reactive_programming_2", out.str(), "");

}


std::string compare_single_threaded_with_multi_threaded(reactive_backend_base& r) {
	std::stringstream out;

	std::stringstream out1;
	std::stringstream out2;
	reactive_signals<int> x(1, &r);
	reactive_signals<int> y(1, &r);
	reactive_signals<int> z(1, &r);

	std::atomic<int> i = 0;
	reactive_processor([&out, &out1, &x, &y, &i]() {
		
		out << "<process1>\n";
		for (int i = 1; i < y; ++i)
		{
			out1 << i << ", ";
		}
		out1 << "\n";
		auto str = out1.str();
		out1.str("");
		out1.clear();
		out1 << str.substr(str.size() - 10);
		out << "</process1>\n";
	}, y);

	reactive_processor([&out, &out2, &x, &y, &i]() {
		
		out << "<process2>\n";
		for (int i = 1; i < x; ++i)
		{
			out2 << i << ", ";
		}
		out2 << "\n";
		auto str = out2.str();
		out2.str("");
		out2.clear();
		out2 << str.substr(str.size() - 10);
		out << "</process2>\n";
	}, x);


	reactive_processor([&out, &x, &y, &z]() {
		out << "<process3>\n";
		x = z.value();
		y = z.value() / 2;
		out << "</process3>\n";
	}, z);

	z = 100000;

	r.stop();

	___ARGGLIB_TEST("stream out1", out1.str(), ", 49999, \n");
	___ARGGLIB_TEST("stream out2", out2.str(), ", 99999, \n");
	return out.str();


}
ARGGLIB__DEFINE_TEST(b_reactive_programming_3) {
	reactive_backend_async r;
	r.init();
	auto str = compare_single_threaded_with_multi_threaded(r);
	___ARGGLIB_TEST("b_reactive_programming_3", str, "<process3>\n</process3>\n<process2>\n<process1>\n</process1>\n</process2>\n");

}

ARGGLIB__DEFINE_TEST(b_reactive_programming_4) {
	reactive_backend  r;
	r.init();
	auto str = compare_single_threaded_with_multi_threaded(r);
	___ARGGLIB_TEST("b_reactive_programming_4", str, "<process3>\n</process3>\n<process2>\n</process2>\n<process1>\n</process1>\n");

}

ARGGLIB__DEFINE_TEST(b_reactive_programming_4_entities) {
	reactive_backend_async r;
	r.init();
	auto out = test_entity2(r);
	___ARGGLIB_TEST("b_reactive_programming_4_entities", out,
		"<active_in_port>2</active_in_port>\n<active_in_port>4</active_in_port>\n"
	);
}