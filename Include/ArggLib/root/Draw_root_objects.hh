#ifndef ArggLib_Draw_root_objects
#define ArggLib_Draw_root_objects
#ifdef USE_ROOT
#include "ArggLib/proc.hh"
#include "ArggLib/type_trates.hh"
#include "TGraph.h"

#define __ROOT_DRAW_OPTION(name,key)          auto name() ->decltype(*this) { m_options += key ; return *this; }
#define __ROOT_DRAW_OPTION_MARKER(name,key)   auto name() ->decltype(*this) { this->current_marker(); this->m_Root_object->SetMarkerStyle(key);  return *this; }
namespace ArggLib {

  namespace ArggLib_impl {
    inline bool& Draw_on_same() {
      static bool __same;
      return __same;
    }

	template <typename T>
	std::string draw_hold_off(T* t, std::string options) {
		return options;
	}
	template <typename T>
	std::string draw_hold_on(T* t, std::string options) {
		return options;
	}

	inline std::string draw_hold_off(TGraph* t, std::string options) {
		options += " A";
		return options;
	}

	inline std::string draw_hold_on(TGraph* t, std::string options) {
		std::replace(options.begin(), options.end(), 'A', ' ');
		return options;
	}
    template <typename T,typename derived_t>
    class Draw_root_object_base {
    protected:
	  enum { force_hold_on, force_holding_off, use_global } m_hold = use_global;
      T m_Root_object;
      bool do_draw = true;
      std::string m_options;

      void Draw_impl() {
        if (do_draw) {
			std::string local_options;
			if (m_hold == force_hold_on|| m_hold == use_global && Draw_on_same()) {
				local_options = draw_hold_on(m_Root_object, m_options);
			}
			else if (m_hold == force_holding_off || m_hold == use_global && !Draw_on_same()) {
				local_options = draw_hold_off(m_Root_object, m_options);
			}

          m_Root_object->Draw(local_options.c_str());
          do_draw = false;
        }
      }
    public:
      Draw_root_object_base(T obj) :m_Root_object(obj) {}
	  derived_t operator()() {
		do_draw = true;
        Draw_impl();
		return *(static_cast<derived_t*>(this));
      }
      
    };
    class Draw_root_object_TGraph :public Draw_root_object_base<TGraph*, Draw_root_object_TGraph> {
	
	public:
      Draw_root_object_TGraph & set_DrawOption(const std::string& opt) {
        this->m_options += " " + opt;
        return *this;
      }
      Draw_root_object_TGraph(TGraph* gr) :Draw_root_object_base(gr) {}


	  auto hold_on() ->decltype(*this) {
		  this->m_hold = force_hold_on;
		  return *this;
	  }
	  auto hold_off() ->decltype(*this) {
		  this->m_hold = force_holding_off;
		  return *this;
	  }
      __ROOT_DRAW_OPTION(axis, " A")
      __ROOT_DRAW_OPTION(bar, " B")
      __ROOT_DRAW_OPTION(bar_bottom, " B1")
	  __ROOT_DRAW_OPTION(current_marker, " P")

	  __ROOT_DRAW_OPTION_MARKER(dot,        kDot)
	  __ROOT_DRAW_OPTION_MARKER(plus,       kPlus)
      __ROOT_DRAW_OPTION_MARKER(star,       kStar)
      __ROOT_DRAW_OPTION_MARKER(Circle,     kCircle)
      __ROOT_DRAW_OPTION_MARKER(Multiply,   kMultiply)
      __ROOT_DRAW_OPTION_MARKER(small_dot,  kFullDotSmall)
      __ROOT_DRAW_OPTION_MARKER(medium_dot, kFullDotMedium)
      __ROOT_DRAW_OPTION_MARKER(large_scalable_dot, kFullDotLarge)
      __ROOT_DRAW_OPTION_MARKER(full_circle, kFullCircle)
      __ROOT_DRAW_OPTION_MARKER(full_square, kFullSquare)
      __ROOT_DRAW_OPTION_MARKER(Full_Triangle_Up, kFullTriangleUp)
      __ROOT_DRAW_OPTION_MARKER(Full_Triangle_Down, kFullTriangleDown)
      __ROOT_DRAW_OPTION_MARKER(open_circle, kOpenCircle)
      __ROOT_DRAW_OPTION_MARKER(open_square, kOpenSquare)
      __ROOT_DRAW_OPTION_MARKER(open_triangle_up, kOpenTriangleUp)
      __ROOT_DRAW_OPTION_MARKER(open_diamond, kOpenDiamond)
      __ROOT_DRAW_OPTION_MARKER(open_cross, kOpenCross)
      __ROOT_DRAW_OPTION_MARKER(open_star, kOpenStar)
      __ROOT_DRAW_OPTION_MARKER(full_star, kFullStar)
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
  inline auto operator|(T&& t, const  ArggLib_impl::Draw_root_objects0& dr) -> decltype(dr(std::forward<T>(t))) {
    return dr(std::forward<T>(t));
  }

  inline auto Draw(std::string opt = "") ->decltype( ArggLib_impl::Draw_root_objects0(opt)) {
    return  ArggLib_impl::Draw_root_objects0(opt);
  }
  inline auto Bar(std::string opt = "") ->decltype( ArggLib_impl::Draw_root_objects0(opt)) {
    return   ArggLib_impl::Draw_root_objects0(opt).bar().axis();
  }

  inline auto Draw(TGraph& gr) ->decltype(ArggLib_impl::Draw_root_object_TGraph(&gr)) {
    return ArggLib_impl::Draw_root_object_TGraph(&gr);
  }
  inline auto Draw(TGraph* gr) ->decltype(ArggLib_impl::Draw_root_object_TGraph(gr)) {
    return ArggLib_impl::Draw_root_object_TGraph(gr);
  }
}


#endif
#endif