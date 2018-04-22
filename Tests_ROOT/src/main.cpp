

#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/vector_helpers.hh"
#include "ArggLib/proc_tools.hh"
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/func.hh"

#include "ArggLib/complete_lib.hh"
#include <map>
#include <string>
#include "ArggLib/root/Fill_root_objects.hh"

using namespace ArggLib;




int main() {

  TH1D h1("h1", "h1", 100, 0, 100);
  100 | for_loop() >> &h1 >> Draw_root_objects{};
  ArggLib::run_tests();
  return 0;
}