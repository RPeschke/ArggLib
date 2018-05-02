#include "ArggLib/complete_lib.hh"
#include "TRandom.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TImage.h"

using namespace ArggLib;

ARGGLIB__DEFINE_TEST(root_processors) {


	auto c = new TCanvas();
	auto m = Modify([](int x) { return gRandom->Gaus(30, 10); });

	auto h1 = TH1D ("h1", "h1", 100, 0, 100);

	10000 | for_loop() >> m >> h1 | Draw();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("canvas.png");
	//delete img;

}