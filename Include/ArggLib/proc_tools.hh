#ifndef proc_tools_h__
#define proc_tools_h__
#include "proc.hh"
#include <vector>
#include <type_traits>


#include <fstream>
//#include "procMultiThreading.hh"
#include "has_member_helper.hh"
#include <iostream>

#include "ArggLib/type_trates.hh"

#include "ArggLib/deprecated/old_processor_definitions.hh"

CREATE_TEST_FOR_MEMBER(has_begin, begin());
namespace ArggLib {

	template <typename T>
	std::vector<T> make_vec(std::initializer_list<T> l) {
		return std::vector<T>(l);
	}

	template <typename T1, typename T2, typename DEFLAULT_T>
	struct ___IS_BOTH_INT {
		using type = typename std::conditional<std::is_same<typename ArggLib::remove_cvref_t<T1>, typename ArggLib::remove_cvref_t<T2>>::value, typename ArggLib::remove_cvref_t<T1>, DEFLAULT_T>::type;
	};

#define  _IS_BOTH_INT_OR_DEFAULT_DOUBLE(x,y) typename ___IS_BOTH_INT<typename std::remove_all_extents<x>::type, typename std::remove_all_extents<y>::type, double>::type


	template <typename T1, typename T2, typename T3, typename DEFLAULT_T>
	struct ___IS_ALL_INT {
		using type = typename ___IS_BOTH_INT<typename ArggLib::remove_cvref_t<T2>, typename ___IS_BOTH_INT<T2, T3, DEFLAULT_T>::type, DEFLAULT_T>::type;
	};


#define  _IS_ALL_INT_OR_DEFAULT_DOUBLE(x,y,z) typename ___IS_ALL_INT<typename ArggLib::remove_cvref_t<x>, typename ArggLib::remove_cvref_t<y>, typename ArggLib::remove_cvref_t<z>, double >::type











	DEFINE_PROC1(square, nextP, inPut) {


		return nextP(inPut, inPut*inPut);
	}

	DEFINE_PROC_V(while_true, nextP, input_) {

		while (true) {
			auto ret = nextP(input_...);
			if (ret != success) {
				return ret;
			}
		}

		return success;

	}





}
#endif // proc_tools_h__

