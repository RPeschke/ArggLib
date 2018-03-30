#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/factoryDef.hh"
#include <string>
#include <iostream>


using namespace std;


class base_class_test {
public:
	using MainType = std::string;
	using Parameter_t = std::string;
	using Parameter_ref = const Parameter_t&;
	virtual void print(std::ostream& out) = 0;
};

ArggLib_registerBaseClass(base_class_test);

class d_class1 :public base_class_test {
public:
	d_class1(Parameter_ref p) :value(p) {

	}

	virtual void print(std::ostream& out) {
		out << "1: " << value << std::endl;
	}
	Parameter_t value;
};
ArggLib_registerClass(base_class_test, d_class1, "1");


class d_class2 :public base_class_test {
public:
	d_class2(Parameter_ref p) :value(p) {

	}
	virtual void print(std::ostream& out) {
		out << "2: " << value << std::endl;
	}
	Parameter_t value;
};
ArggLib_registerClass(base_class_test, d_class2, "2");

ARGGLIB__DEFINE_TEST(factory_test_1) {

	auto x1 = ArggLib::Class_Factory<base_class_test>::Create("1", "asdad");
	auto x2 = ArggLib::Class_Factory<base_class_test>::Create("2", "sadasd");

	std::stringstream out;
	x1->print(out);
	___ARGGLIB_TEST("factory_test_1 x1 ", out.str(), "1: asdad\n");
	out.str("");
	out.clear();


	x2->print(out);
	___ARGGLIB_TEST("factory_test_1 x2 ", out.str(),  "2: sadasd\n");
	out.str("");
	out.clear();
}