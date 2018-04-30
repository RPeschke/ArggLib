#ifndef ArggLib_Draw_root_objects
#define ArggLib_Draw_root_objects
#ifdef USE_ROOT

#include "ArggLib/type_trates.hh"
#include "TGraph.h"

#define __ROOT_DRAW_OPTION(name,key)     auto name() ->decltype(*this) { m_options += key ; return *this; }
namespace ArggLib {

  namespace ArggLib_impl {
    inline bool& Draw_on_same() {
      static bool __same;
      return __same;
    }

    
    class Draw_root_object_TGraph {
      TGraph* m_graph = nullptr;
      bool do_draw = true;
      std::string m_options;
      using this_t = Draw_root_object_TGraph;
    public:

      void Draw_impl() {
        if (m_graph && do_draw) {
          m_graph->Draw(m_options.c_str());
          do_draw = false;
        }
      }
      __ROOT_DRAW_OPTION(axis, " A")
      __ROOT_DRAW_OPTION(bar, " B")
        __ROOT_DRAW_OPTION(bar_bottom, " B1")
      Draw_root_object_TGraph(TGraph* gr) :m_graph(gr) {}
      ~Draw_root_object_TGraph(){

        Draw_impl();
      }

    };

    class Draw_root_objects0 {
    public:

      std::string m_options;
      Draw_root_objects0(std::string opt):m_options(opt) {

      }

      __ROOT_DRAW_OPTION(axis, " A")
        __ROOT_DRAW_OPTION(invisible_axis, " I")
        __ROOT_DRAW_OPTION(polyline, " L")
        __ROOT_DRAW_OPTION(fill_area, " F")
        __ROOT_DRAW_OPTION(smooth_Curve, " C")
        __ROOT_DRAW_OPTION(star, " *")
        __ROOT_DRAW_OPTION(current_marker, " P")
        __ROOT_DRAW_OPTION(bar, " B")
        __ROOT_DRAW_OPTION(bar_bottom, " B1")
        __ROOT_DRAW_OPTION(axis_X_top, " X+")
        __ROOT_DRAW_OPTION(axis_Y_top, " Y+")
        __ROOT_DRAW_OPTION(palette_Fill_Color, " PFC")
        __ROOT_DRAW_OPTION(palette_Line_Color, " PLC")
        __ROOT_DRAW_OPTION(palette_Marker_Color, " PMC")
        __ROOT_DRAW_OPTION(axis_X_reverse, " RX")
        __ROOT_DRAW_OPTION(axis_Y_reverse, " RY")

        template <typename ARGS>
      auto End(ARGS&& args) ->decltype(args->Draw(m_options.c_str())) {

        return   args->Draw(m_options.c_str());
      }

      template <typename NEXT_T, typename... ARGS>
      procReturn operator()(NEXT_T&& next, ARGS&&... args) {


        return next(std::forward<ARGS>(args)...);
      }


      template <typename T, ARGGLIB__REQUIRES((std::is_base_of<TObject, T>::value)) >
      auto operator()(T* obj) const ->decltype(obj->Draw(m_options.c_str())) {

        auto options = m_options;// +Draw_on_same();

        return obj->Draw(options.c_str());
      }
      template <typename T, ARGGLIB__REQUIRES((std::is_base_of<TObject, T>::value)) >
      auto operator()(T& obj) const ->decltype(obj.Draw(m_options.c_str())) {
        auto options = m_options;// +Draw_on_same();
        return obj.Draw(options.c_str());
      }
    };
  }
  inline void hold_on() {
    auto &_same = ArggLib_impl::Draw_on_same();
      _same = 1;
  }
  inline void hold_off() {
    auto &_same = ArggLib_impl::Draw_on_same();
    _same = 0;
  }
  template <typename T>
  auto operator|(T&& t, const  ArggLib_impl::Draw_root_objects0& dr) -> decltype(dr(std::forward<T>(t))) {
    return dr(std::forward<T>(t));
  }

  auto Draw(std::string opt = "") ->decltype(ArggLib_impl::Draw_root_objects0(opt)) {
    return  ArggLib_impl::Draw_root_objects0(opt);
  }
  auto Bar(std::string opt = "") ->decltype(ArggLib_impl::Draw_root_objects0(opt)) {
    return  ArggLib_impl::Draw_root_objects0(opt).bar().axis();
  }

  auto Draw(TGraph& gr) ->decltype(ArggLib_impl::Draw_root_object_TGraph(&gr)) {
    return ArggLib_impl::Draw_root_object_TGraph(&gr);
  }
  auto Draw(TGraph* gr) ->decltype(ArggLib_impl::Draw_root_object_TGraph(gr)) {
    return ArggLib_impl::Draw_root_object_TGraph(gr);
  }
}


#endif
#endif