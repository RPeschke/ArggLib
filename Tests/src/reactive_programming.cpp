#include "ArggLib/complete_lib.hh"

#include "ArggLib/reactive_programming/reactive_backend.hh"
#include "ArggLib/reactive_programming/reactive_variable.hh"
#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/reactive_programming/reactive_signals.hh"

using namespace  ArggLib;
using namespace std;
ARGGLIB__DEFINE_TEST(reactive_programming_1) {

	std::stringstream out;
	reactive_backend r;
	auto p1 = reactive_processor_1([&out]() { out << "hallo welt\n";  });
	r.push_back(p1);

	r.append({
		reactive_processor_1 ([&out]() { out << "hallo welt1\n";  }),
		reactive_processor_1 ([&out]() { out << "hallo welt2\n";  }),
		reactive_processor_1 ([&out]() { out << "hallo welt3\n";  })
		});


	r.stop();
	___ARGGLIB_TEST("stream out", out.str(), "hallo welt\nhallo welt1\nhallo welt2\nhallo welt3\n");

}

ARGGLIB__DEFINE_TEST(reactive_programming_2_recursion) {

	std::stringstream out;
	reactive_backend r;
	auto p2 = reactive_processor_1([&r, &out]() { out << "<level_1>\n";
	auto p1 = reactive_processor_1([&out]() { out << "<level_2/>\n";  });
	r.push_back(p1);
	out << "</level_1>\n";
	});

	r.push_back(p2);




	r.stop();
	___ARGGLIB_TEST("reactive_programming_2_recursion", out.str(), "<level_1>\n</level_1>\n<level_2/>\n");
}

ARGGLIB__DEFINE_TEST(reactive_programming_3) {
	std::stringstream out;
	reactive_backend r; 
	reactive_variable<int> x(1,&r);
	reactive_variable<int> y(1, &r);
	reactive_variable<int> z(1, &r);
	reactive_processor([&out,&x,&y]() {
		out << "<reactive_processor>\n" << x << "\n</reactive_processor>\n" ;
		y = x.value() * 2;
	}, x);


	reactive_processor([&out,&x, &y]() {
		out << "<reactive_processor2>\n" << x << "\n" << y << "\n</reactive_processor2>\n";
		if (y < 17) {
			x = y.value() * 2;
		}
	}, y);

	x = 2;
	r.stop();
	___ARGGLIB_TEST("reactive_programming_2_recursion", out.str(), 
		"<reactive_processor>\n2\n</reactive_processor>\n<reactive_processor2>\n2\n4\n</reactive_processor2>\n<reactive_processor>\n8\n</reactive_processor>\n<reactive_processor2>\n8\n16\n</reactive_processor2>\n<reactive_processor>\n32\n</reactive_processor>\n<reactive_processor2>\n32\n64\n</reactive_processor2>\n"
	);

}

ARGGLIB__DEFINE_TEST(reactive_programming_4_signals) {
	std::stringstream out;
	reactive_backend r;

	reactive_signals<int> x(1,&r);
	reactive_signals<int> y(1, &r);

	reactive_processor([&out, &x, &y]() {
		out << "<x>\n" << x << "\n</x>\n";
		y = x.value() * 2;
		out << "<y>\n" << y << "\n</y>\n";

	}, x);

	reactive_processor([&out, &x, &y]() {
		if (y > 17) {
			return;
		}
		out << "<y>\n" << y << "\n</y>\n";
		x = y.value() * 2;
		out << "<x>\n" << x << "\n</x>\n";

	}, y);
	x = 2;
	//std::cout << y << std::endl;
	r.stop();
	___ARGGLIB_TEST("reactive_programming_2_recursion", out.str(),
		"<x>\n2\n</x>\n<y>\n1\n</y>\n<y>\n4\n</y>\n<x>\n2\n</x>\n<x>\n8\n</x>\n<y>\n4\n</y>\n<y>\n16\n</y>\n<x>\n8\n</x>\n<x>\n32\n</x>\n<y>\n16\n</y>\n"
	);
}