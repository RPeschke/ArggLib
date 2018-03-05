

#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/vector_helpers.hh"
#include "ArggLib/proc_tools.hh"
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/func.hh"

#include <map>
#include <string>


using namespace ArggLib;


class tsda;
class c2 {
public:
  c2(const tsda&) {
    std::cout << "dsaasd\n";
  }
  int i;
};

void print_test(const c2& c) {
  std::cout << c.i << std::endl;
}
class tsda {
public:

  int get_number() {
    return 1;
  }
//   operator c2() {
//     return c2{*this};
//   }

} _impl_test;

#define  asd _impl_test

int main() {

  _impl_test.get_number();
  tsda t;
  auto adsad = t;
  print_test(t);
  ArggLib::run_tests();
  return 0;
}