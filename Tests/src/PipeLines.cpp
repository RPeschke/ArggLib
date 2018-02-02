// PipeLines.cpp : Defines the entry point for the console application.
//

#include "ArggLib/proc.hh"
#include "ArggLib/param.hh"
#include "ArggLib/proc_tools.hh"

#include <sstream>
#include "ArggLib/ArggLib_Unit_Tests.hh"








using namespace std;

using namespace ArggLib;






template <typename T1,typename T2>
void __Test(std::string TestName, T1&& t1, T2&& t2) {

	if (t1 != t2) {
		cout << "test '" << TestName <<"' failed\n";
		cout << "Variable: \n" << t1 << "\n Unequal \n " << t2 << endl;
	}
	else {
		cout << "test '"<< TestName<< "' successful\n";
	}

}

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