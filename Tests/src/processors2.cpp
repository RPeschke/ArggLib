#include <time.h>
#include "TCanvas.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include "TH2D.h"
#include "TApplication.h"
#include "proc.hh"
#include <functional>
#include <memory>
#include "TFile.h"
#include "TTree.h"

#include "proc_tools.hh"
#include <fstream>

#include "l_array.hh"
#include "test.hh"
#include <type_traits>
#include <math.h>
#include <future>
#include <thread>
#include <windows.h>
#include "procN2M.hh"
#include "TMath.h"
#include "procMultiThreading.hh"



#define  multiThread 1

using namespace std;

#define Unew __unew_start << new 
enum _unew_ {
  __unew_start
};

template <typename T>
auto operator<<(_unew_, T* in_) {
  return std::unique_ptr<T>(in_);
}

TApplication* getApp() {
  return new TApplication("myapp", 0, 0);
}












class timestamp {
public:
  timestamp() :start(clock()) {}
  ~timestamp() {

    double interval = (clock() - start) / (double)CLOCKS_PER_SEC;



    printf("%f seconds elapsed \n", interval);
  }
  void print(std::ostream& out) const {

    double interval = (clock() - start) / (double)CLOCKS_PER_SEC;

    out << interval << "  seconds elapsed" << std::endl;


  }
  double get_timestamp() {
    return  (clock() - start) / (double)CLOCKS_PER_SEC;
  }
  clock_t start;
};

struct hit__ {

  double x, y, z;
  hit__(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {

  }
};











// class par_for_ {
// public:
//   par_for_(thread_handler* th):m_th(th) {}
// 
//   template<typename Next_t>
//   auto operator()(Next_t&& next_, int start_, int end_) {
//     auto th = m_th;
//  auto thr1 =   std::thread(
//    [next_, start_,  end_ ,th]() mutable {
//       for (auto i = start_ ; i < end_;i += 3) {
//         next_(i);
//         th->update();
//       }
//      });
//  m_th->push(thr1.get_id());
// 
// 
//  auto thr2 = std::thread([next_, start_,  end_,th]() mutable {
//    for (auto i = start_ + 1; i < end_;i += 3) {
//      next_(i);
//      th->update();
//    }
//  });
//  m_th->push(thr2.get_id());
// 
//  auto thr3 = std::thread([next_, start_,  end_ , th]() mutable {
//    for (auto i = start_ + 2; i < end_;i += 3) {
//      next_(i);
//      th->update();
//    }
//  });
//  m_th->push(thr3.get_id());
// 
//  thr1.join();
//  thr2.join();
//  thr3.join();
//     return success;
//   }
// private:
//   thread_handler* m_th;
// };











// int f1() {
// 
// 	timestamp t;
// 	thread_handler th;
// 	ofstream out("test_3_threads.txt");
// 
// 
// 
// 	auto p = proc()
// 		>> par_for(4, &th)
// 
// 		>> [](auto&& next, auto&& input__) mutable {
// 		std::vector<int> vec;
// 		vec.push_back(input__);
// 		for (int i = 2; i < input__; ++i) {
// 			if (input__%i == 0) {
// 				vec.push_back(i);
// 			}
// 		}
// 		return next(vec);
// 	}
// 
// 		>> de_randomize(&th)
// 		>> push(&out);
// 
// 	p(100000000, 100000500);
// 
// 	out << t.get_timestamp() << " seconds" << std::endl;
// 
// 	return 0;
// }

int i_max = 2000;

void f2_c(const int& i, const int& j, const int& k, int & sum) {
  sum += TMath::Sqrt(i + k + j);
}
void f2_b(const int& i, const int& j, int & sum) {
 
    for (int k = 0; k < i_max; ++k) {
      f2_c(i, j, k, sum);
      //std::cout << sum << std::endl;
    }

}
void f2_a(const int& i, int & sum){
  for (int k = 0; k < i_max; ++k) {
    f2_b(i,k,sum);
  }
}
int f2() {
	timestamp t;
 
  int sum = 0;
  for (int i =0 ;i< i_max ; ++i){
    f2_a(i, sum);
  }


	std::cout << t.get_timestamp() << " seconds "<< sum << std::endl;

  return sum;
}

template<typename T>
auto for_loop_(T start,T end_) {
  return [start,end_](auto& next, auto&&... args) {
    
    for (auto i = start; i < end_; ++i) {
      if (next(args..., i) == stop_) {
        return stop_;
      }
    }

    return success;
  };
}




class combine {
public:
  combine(unsigned& sum_) :sum(sum_) {}
  template<typename next_t, typename IN_t>
  auto operator()(next_t& next, IN_t& i) {
    s+=i;
    return next(i);
  }

  ~combine() {
    static std::mutex m;
    std::unique_lock<std::mutex> l(m);
    std::cout << sum << "  " << s;
    sum += s;
    std::cout << "  " << sum << std::endl;
  }

  unsigned s = 0;
  unsigned & sum ;
};



int f3() {
  timestamp t;
  int i_max = 20;


  auto for__= [i_max](auto& next, const auto&... args) {
    for (int i = 0; i < i_max; ++i) {
      next(i,args...);
    }
    return success;
  };
  thread_handler th;
  unsigned sum = 0;
  unsigned sum1 = 0;
  std::mutex m;
  auto p = Unew int(50);

  auto pp = p.release();
  auto a = proc() 
    >> par_for(8 ,&th, 0, i_max)
    >> for_loop(0, i_max)
    >> for_loop(0, i_max)
    >> [](auto& next, const auto& i, auto j, auto k) {
    Sleep(1);
      return next( TMath::Sqrt(i+ j) );
  }
  >> combine(sum1)
    // >> [&sum, &m](auto& next, unsigned i) { std::unique_lock<std::mutex> l(m); sum += i; return next(); }
  //>> [&sum](auto& next, unsigned i) {sum += i; return next(); }
  ;
  a();
  std::cout << t.get_timestamp() << " seconds " << sum << std::endl;
  return 0;
}
#include "TObject.h"
#include "RQ_OBJECT.h"
#include "TGButton.h"
#define RQ_SLOT(className,fname) auto slot_##fname {return Signal_slots2<className> (this,#fname,#className);}\
void fname 


template <typename T>
class Signal_slots2 {
public:
  Signal_slots2(T* obj, const std::string& str, const std::string& class_name = "") :
    m_object(obj),
    m_name(str),
    m_className(class_name) {}

  T* m_object;
  std::string m_name, m_className;
};


class A {
  RQ_OBJECT("A")

public:

  void   test() const { std::cout << "test" << std::endl; }

  RQ_SLOT(A, test1());
};
void A::test1() {
  std::cout << "test1" << std::endl;
}


class Signal_slots {
public:
  Signal_slots(TQObject* obj, const std::string& str, const std::string& class_name = "") :
    m_object(obj),
    m_name(str),
    m_className(class_name) {}

  TQObject * m_object;
  std::string m_name, m_className;
};



template <typename T1 , typename T2>
void operator >> (const Signal_slots2<T1>& signal_, const Signal_slots2<T2>& slot_) {

  signal_.m_object->Connect(signal_.m_name.c_str(), slot_.m_className.c_str(), slot_.m_object, slot_.m_name.c_str());
}

class TGButton_signals {
public:
  TGButton_signals(TGButton* obj) : m_object(obj) {}
  Signal_slots2<TGButton> Clicked() {
    TString cname = "TGButton";
    return Signal_slots2<TGButton>(m_object, "Clicked()", cname.Data());
  }

  TGButton* m_object;
};

TGButton_signals _signals(TGButton* obj) {

  return TGButton_signals(obj);
}
void t0() {
  TGTextButton b;
  A a;
  
  _signals(&b).Clicked() >> a.slot_test1();

  b.Clicked();
}
int main() {
  TApplication *myapp = new TApplication("myapp", 0, 0);

  t0();

  myapp->Run();
//  f2();
  f3();
//   std::vector<int> i{ 1,2,3,4 };
//   auto a=  proc() >> for_loop(1,10)>> for_loop(1, 10) >> display();
// 
//   auto l1 = proc()>> for_loop(1, 10)>> for_loop(1.0, 10.0) >>display() ;
// auto iii =l1();
 // a();
  return 0;
}