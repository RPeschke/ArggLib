#ifndef object_modifier_h__
#define object_modifier_h__




namespace ArggLib {

	template<typename Fun_t>
	class object_modifier_impl {

		Fun_t m_fun;
	public:
		object_modifier_impl(Fun_t v) :m_fun(v) {}

		template <typename T>
		auto& modify(T& t) const{
			
			m_fun(t);
			return t;
		}

	};


	template <typename F>
	auto object_modifier(F f) {
		return object_modifier_impl<F>(std::move(f));
	}







	template <typename T1, typename T2>
	auto& operator-(T1& t1, const object_modifier_impl<T2>& t2) {
		t2.modify(t1);
		return t1;
	}

}


#endif // object_modifier_h__