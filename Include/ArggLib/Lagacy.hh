#ifndef ArggLibLagacy_h__
#define ArggLibLagacy_h__

#include <vector>
#include "ArggLib/procReturn.hh"

namespace ArggLib {



	template <typename T>
	void ___reset(std::vector<T>* vec) {
		vec->clear();
	}


	template <typename T, typename... ARGS>
	void ___Fill(std::vector<T>* vec, ARGS&&... args) {
		vec->emplace_back(std::forward<ARGS>(args)...);
	}





	template<typename T, typename... ARGS>
	void ___Fill(T* g, ARGS&&... args);

	template<typename T>
	void ___reset(T* h);

	void ___reset(std::ostream*);

	template<typename... ARGS> void ___Fill(std::ostream* out, ARGS&&... args);
	template<typename T>
	class push_impl {
		T* m_graph;
	public:

		push_impl(T* t) :m_graph(t) {
			___reset(m_graph);
		}



		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS... args) {
			___Fill(m_graph, args...);
			return next(args...);
		}
	};

	template<typename T>
	push_impl<T> push(T* f) {
		return push_impl<T>(f);
	}

	template<typename T>
	class push_impl_RV {
		T m_graph;
	public:

		push_impl_RV(T&& t) :m_graph(std::move(t)) {
			___reset(&m_graph);
		}



		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS... args) {
			___Fill(&m_graph, args...);
			return next(args...);
		}
	};

	template<typename T>
	push_impl_RV<typename std::remove_all_extents<T>::type> push(T&& f) {
		return push_impl_RV<typename std::remove_all_extents<T>::type >(std::forward<T>(f));
	}

	class push_first {
	public:
		push_first(std::vector<double>& vec_) :vec(vec_) {}
		template <typename NEXT_T, typename T, typename... args>
		procReturn operator()(NEXT_T&& next, T x, args&&... ar) const {
			vec.push_back(x);

			return next(ar...);
		}
	private:
		std::vector<double>& vec;
	};

	class get_element {
	public:
		get_element(const std::vector<double>& vec_) :vec(vec_) {}
		template <typename NEXT_T, typename T, typename... args>
		procReturn operator()(NEXT_T&& next, T i, args&&... ar) {

			return next(i, ar..., vec[i]);
		}
	private:
		const	std::vector<double>& vec;
	};


	inline void ___reset(std::ostream*) {}

	template<typename... ARGS> void ___Fill(std::ostream* out, ARGS&&... args) { print__(*out, args...); }

	inline void ___reset(std::shared_ptr<std::ofstream>*) {}

	template<typename... ARGS> void ___Fill(std::shared_ptr<std::ostream>* out, ARGS&&... args) { print__(*out->get(), args...); }
}


#endif // ArggLibLagacy_h__
