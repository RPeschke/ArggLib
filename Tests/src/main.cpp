

#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/vector_helpers.hh"
#include "ArggLib/proc_tools.hh"
#include <algorithm>
#include "ArggLib/cautor.hh"
#include "ArggLib/func.hh"

#include <map>
#include <string>


using namespace ArggLib;



int main() {

auto sort1 = [](auto hir , auto F){

  std::sort(hir.begin(), hir.end(), F);
  return hir;
};

std::map<std::string, double> dsasd;
dsasd["dsad0"] = 0;
dsasd["dsad1"] = 2;
dsasd["dsad3"] = 3;
dsasd["dsad4"] = -5454;

  

  auto to_vector = [](auto x){
    return  std::pair<std::string, double>(x.first,x.second);
  };

  auto to_vector_loop = [](auto container_v,auto to_vector_f) {
    std::vector<std::remove_reference_t<decltype(to_vector_f(*container_v.begin()))>> ret;
    for (auto e : container_v) {
      ret.push_back(to_vector_f(e));
    }

    return ret;
  };


  auto sasas =   to_vector_impl()(dsasd,to_vector);
  auto f1 = ArggLib::fun() <<[](auto s){ return  ArggLib::to_vector_impl()(s);} << _X;
  f1(dsasd);
  auto sasasad  = dsasd | ArggLib_to_vector<< to_vector;
  auto asdada = sasasad | ArggLib_sort_by(second);
  std::vector<std::remove_reference_t<decltype(to_vector(*dsasd.begin()))>> hir;
  for (auto e : dsasd){
    hir.push_back(to_vector(e));
  }
  
    hir.emplace_back("dsasd0", 1);
    hir.emplace_back("dsasd1", 1312321);
    hir.emplace_back("dsasd0", -5456);
    std::remove_reference_t<decltype(hir[0])> sd("dsasd0", 1);
  //  auto s=  sort1(hir , [](cautor e1, cautor e2) {return  e1.second < e2.second; });
    auto f = fun() << sort1<< _X << [](cautor e1, cautor e2) {return  e1.second < e2.second; };
   auto s1= hir| f;
  // auto ss=  ArggLib_sort_by(hir, second);
   auto s2 = hir | ArggLib_sort_by(second);
  std::sort(hir.begin(), hir.end(), [](cautor e1, cautor e2) {return  e1.second < e2.second; });

  
  auto x=  contains(make_vec({ 1,2,5,6 }),1);
	ArggLib::run_tests();
	return 0;
}