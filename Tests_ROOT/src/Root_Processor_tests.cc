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

ARGGLIB__DEFINE_TEST(root_processors1) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	TGraph g;
	10000 | for_loop() >> e >> g;
	auto x = Draw(g).axis().bar()();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph01.png");
}


ARGGLIB__DEFINE_TEST(root_processors2) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	TGraph g;
	10000 | for_loop() >> e >> g;
	Draw(g).axis().bar();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph02.png");
}

ARGGLIB__DEFINE_TEST(root_processors3) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	TGraph g;
	10 | for_loop() >> e >> g;
	Draw(g).axis().full_circle();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph03.png");
}

ARGGLIB__DEFINE_TEST(root_processors4) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	TGraph g;
	10 | for_loop() >> e >> g;
	Draw(g).full_circle();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph04.png");
}

ARGGLIB__DEFINE_TEST(root_processors5) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	auto e2 = Evaluate([](auto x) {return 2*x; });
	TGraph g;
	10 | for_loop() >> e >> g;
	Draw(g).full_circle();
	TGraph g2;
	10 | for_loop() >> e2 >> g2;
	Draw(g2).full_circle();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph05.png");
}


ARGGLIB__DEFINE_TEST(root_processors6) {
	auto c = new TCanvas();
	auto e = Evaluate([](auto x) {return x * x; });
	auto e2 = Evaluate([](auto x) {return 2 * x; });
	TGraph g;
	10 | for_loop() >> e >> g;
	Draw(g).full_circle();
	TGraph g2;
	10 | for_loop() >> e2 >> g2;
	hold_on();
	Draw(g2).full_circle();
	gSystem->ProcessEvents();
	TImage *img = TImage::Create();
	img->FromPad(c);
	img->WriteImage("TGraph06.png");
}