#ifndef add_parameter_h__
#define add_parameter_h__


#define add_parameter(name,type) \
public: \
	auto name() const { \
		return m_##name; \
	} \
	auto name(const type& del) { \
		m_##name = del; \
		return *this; \
	} \
private: \
	type m_##name



#endif // add_parameter_h__