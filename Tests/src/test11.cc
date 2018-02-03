  std::mutex m;
  timestamp t;

 // auto app = getApp();
#ifdef multiThread
  ofstream out("test_multi.txt");
#else
  ofstream out("test_single.txt");
#endif
  std::vector<std::future<procReturn>> future__;
auto p=proc() >> 
#ifdef multiThread
par_for()
#else
for_loop()
#endif
    >> [](auto&& next,auto&& input__ ) mutable {
   std::vector<int> vec;
    vec.push_back(input__);
    
    for (int i=2;i< input__;++i){

      if (input__%i==0) {
         vec.push_back(i);
      }
   }
         next(vec);
   
  
    return success;
 }
#ifdef multiThread
 >> [&m](auto&& next,auto && i ) { std::lock_guard<std::mutex> lock(m); return next(i); }
#endif

 >> push(&out);



 p(0,1000,1);

 
   std::lock_guard<std::mutex> lock(m);

   t.print(out);
 

//   vector<std::unique_ptr<ProcessorBase>> processors;
//   p_storage<std::shared_ptr<TFile>>  file(processors, std::make_shared<TFile>("run000850_fitter.root"));
//   auto collection_gbl = make_proc<get_collection>(file, "GBL_tracks");
//   auto collection_FEI4 = make_proc<get_collection>(file, "zsdata_apix");
//   auto pl1 = make_proc<getPlane>(collection_gbl, 8);
//   auto fei4 = make_proc<getPlane>(collection_FEI4, 20);
//   auto fei4_trans2 = make_proc<transform_hits>(fei4, [](const hit_xy& hit) { return hit_xy(0.241029*hit.x - 11.9750, -0.0477630*hit.y + 4.70778); });
//   auto closest = make_proc<find_closest>(pl1, fei4_trans2, 1);
//   auto collection_dut = make_proc<get_collection>(file, "zsdata_strip");
//   auto dut = make_proc<getPlane>(collection_dut, 8);
//   auto gbl_trans = make_proc<transform_hits>(closest, [](const hit_xy& hit) { return hit_xy(13.4111 *hit.x + 63.6906, hit.y); });
// 
//   auto hits_with_DUT_hit = make_proc<find_closest_strip>(gbl_trans, dut, 5, x_axis);
// 
// 
//   auto hist12 = make_proc<Fill_HIST<TH2D> >(hits_with_DUT_hit, "h11", "TrueHitsWithDUT", 120, 0, 120, 100, -10, 10);
//   auto hist13 = make_proc<Fill_HIST<TH2D> >(gbl_trans, "h11", "TureHits", 120, 0, 120, 100, -10, 10);
// 
// 
//   run(processors, 100000);
// 
// 
//   hist12->Draw("colz");
//   new TCanvas();
// 
// 
//   hist12->Divide(&hist13.m_t.m_t);
// 
// 
//   app->Run();