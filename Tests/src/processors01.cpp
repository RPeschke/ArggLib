#include "ArggLib/complete_lib.hh"
#include "ArggLib/processor.hh"


using namespace  ArggLib;
using namespace std;

class t2 {
public:
  t2(std::ostream& out) :m_out(out) {}
  react_proc_return next() {
    m_out<< i << std::endl;
    if (++i > 10){
      return react_proc_return::stop;
    }
    return react_proc_return::success;
  }
  std::ostream& m_out;
  int i = 0;
};
ARGGLIB__DEFINE_TEST(processors_1) {

	std::stringstream out;

  processHandler ph;

  auto x =  Pnew(ph) t2(out);
  


  ph.run();
  

	___ARGGLIB_TEST("processors_1", out.str(), "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");

}

ARGGLIB__DEFINE_TEST(processors_2) {

  std::stringstream out;


  auto x = gPnew t2(out);
  gP_run();


  ___ARGGLIB_TEST("processors_2", out.str(), "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");

}


ARGGLIB__DEFINE_TEST(processors_3) {

  std::stringstream out;


  auto x = giPnew(1) t2(out);
  giP_run(1);
  
  


  ___ARGGLIB_TEST("processors_3", out.str(), "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");


  auto x0 = &gProcessHandler<0>();
  auto x1 = &gProcessHandler<1>();
  auto x3 = x0 == x1;
  ___ARGGLIB_TEST("processors_3b", x3, false);

}