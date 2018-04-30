

#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/vector_helpers.hh"
#include "ArggLib/proc_tools.hh"
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/func.hh"
#include "ArggLib/Evaluate.hh"
#include "ArggLib/complete_lib.hh"
#include <map>
#include <string>
#include "ArggLib/root/Fill_root_objects.hh"
#include "ArggLib/root/Draw_root_objects.hh"
#include "TRandom.h"
#include "TApplication.h"
#include "TGraph.h"
using namespace ArggLib;




int main() {
	TApplication app{"test",0,0};
	TGraph g;

	auto m=	Modify([](auto x) { return gRandom->Gaus(30,10) ; });
	auto e = Evaluate([](auto x) {return x * x; });
  TH1D h1("h1", "h1", 100, 0, 100);

  auto x = 10000 | for_loop() >> Evaluate([](auto x) {return x * x; }) >> g ;
  //>> Draw();
  //hold_on();
  //*x | Bar();

  Draw(g).bar().axis();
  ArggLib::run_tests();

  auto d = Draw("COLZ");
  app.Run();
  return 0;
}