#ifndef ArggLibFill_h__
#define ArggLibFill_h__



namespace ArggLib {


	template<typename T>
	void print__(std::ostream& out, T&& t) {
		out << t << std::endl;
	}
	template<typename T, typename... ARGS>
	void print__(std::ostream& out, T&& t, ARGS&&... args) {
		out << t << "  ";
		print__(out, std::forward<ARGS>(args)...);
	}
	template<typename T>
	void print__(std::ostream& out, std::vector<T>& t) {
		for (auto& e : t)
		{
			out << e << "   ";
		}
		out << std::endl;
	}



	template <typename... T>
	void _Fill(std::ostream& out, T&&... i) {
		ArggLib::print__(out, std::forward<T>(i)...);
	}


	template<typename T>
	class Fill_impl {
	public:
		T& m_histo__;
		Fill_impl(T& vec) :m_histo__(vec) {

		}


		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

			_Fill(m_histo__, args...);
			return next(std::forward<ARGS>(args)...);
		}


	};

	template <typename T>
	Fill_impl<T> Fill(T& histo__) {
		return Fill_impl<T>(histo__);
	}



	auto display() {
		return	ArggLib::Fill(std::cout);
	}
}

#endif // ArggLibFill_h__
