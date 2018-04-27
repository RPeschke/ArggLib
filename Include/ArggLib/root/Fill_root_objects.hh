#ifndef ArggLib_Fill_root_objects
#define ArggLib_Fill_root_objects

#include "TH1D.h"
#include "TGraph.h"
#include "ArggLib/type_trates.hh"
#include <type_traits>
#include <tuple>
namespace ArggLib {
  template <typename T>
  class Fill_root_histogram_impl {
  public:
    T * m_h = nullptr;
    bool do_reset = false;
    Fill_root_histogram_impl& reset_on_start() {
      do_reset = true;
      return *this;
    };
    Fill_root_histogram_impl(T* h) :m_h(h) {}
    template <typename NEXT_T, typename... ARGS>
    auto operator()(NEXT_T&& next, ARGS&&... x) ->decltype(next(x...)) {
      m_h->Fill(x...);
      return next(x...);
    }
    
    procReturn Start() {
      if (do_reset) {
        m_h->Reset();
      }
      return procReturn::success;
    }
    auto End() ->decltype(m_h) {

      return   m_h;
    }

  };
  template <typename T>
  auto Fill_histogramm(T* t) -> decltype(Fill_root_histogram_impl<T>(t)){
    return  Fill_root_histogram_impl<T>(t);
  }



  template < typename T, ARGGLIB__REQUIRES(( std::is_base_of<TH1, T>::value)) >
 // template < typename T, typename std::enable_if < std::is_base_of<TH1, T>::value , int > ::type = 0 >
//  template < typename T, typename  std::enable_if <  ___REQ( std::is_base_of<TH1, T>::value) ,int   > ::type = 0 >
  auto __helper_to_proc(T* t) -> decltype(Fill_histogramm(t)) {
    return Fill_histogramm(t);
  }
  template < typename T, ARGGLIB__REQUIRES(( std::is_base_of<TH1, T>::value))>
  inline auto __helper_to_proc(T& t) -> decltype(Fill_histogramm(&t)) {
    return Fill_histogramm(&t);
  }

  template <typename T>
  class Fill_root_graph_impl{
  public:
	  Fill_root_graph_impl(T* graph_) :m_graph(graph_) {}
	  T* m_graph;
	  template <typename NEXT_T, typename... ARGS>
	  auto operator()(NEXT_T&& next, ARGS&&... x) ->decltype(next(std::forward<ARGS>(x)...)) {
		  
		  m_graph->SetPoint(m_graph->GetN(), x...);
	
		  return next(std::forward<ARGS>( x)...);
	  }


	  auto End() ->decltype(m_graph) {

		  return   m_graph;
	  }

  };

  template <typename T>
  auto Fill_graph(T* t) -> decltype(Fill_root_graph_impl<T>(t)) {
	  return  Fill_root_graph_impl<T>(t);
  }

  template < typename T, ARGGLIB__REQUIRES((std::is_base_of<TGraph, T>::value)) >
  auto __helper_to_proc(T* t) -> decltype(Fill_graph(t)) {
	  return Fill_graph(t);
  }
  template < typename T, ARGGLIB__REQUIRES((std::is_base_of<TGraph, T>::value)) >
  inline auto __helper_to_proc(T& t) -> decltype(Fill_graph(&t)) {
	  return Fill_graph(&t);
  }

  class Draw_root_objects0 {
  public:
    

    Draw_root_objects0()  {

    }

    template <typename ARGS>
    auto End(ARGS&& args) ->decltype(args->Draw()) {

      return   args->Draw();
    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {


      return next(std::forward<ARGS>(args)...);
    }


  };


  auto Draw() {
	  return Draw_root_objects0();
  }

  template <typename ARGS>
  class Draw_root_objects1 {
  public:
	  ARGS m_args;

	  Draw_root_objects1(const ARGS& arg):m_args(arg) {

	  }
	  Draw_root_objects1( ARGS&& arg) :m_args(std::move(arg)) {

	  }
	  template <typename ARGS>
	  auto End(ARGS&& args) ->decltype(args->Draw()) {

		  return   args->Draw();
	  }

	  template <typename NEXT_T, typename... ARGS>
	  procReturn operator()(NEXT_T&& next, ARGS&&... args) {


		  return next(std::forward<ARGS>(args)...);
	  }


  };
  template <typename... Args>
  auto Draw(Args&&... args) {
	  auto t =std::make_tuple(std::forward<Args>(args)...);
	  return  Draw_root_objects1<decltype(t)>(std::move(t));
  }
}
#endif