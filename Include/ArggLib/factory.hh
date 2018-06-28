#ifndef ArggLib_factory_h__
#define ArggLib_factory_h__

#include <map>
#include <vector>
#include <memory>
#ifdef WIN32
  #define CLASSFACTORY_DLLEXPORT  __declspec( dllexport ) 
  #include <stdint.h>
#else
  #define CLASSFACTORY_DLLEXPORT  
#endif

#define ArggLib_registerClass(baseClass,derivedClass,identifier)    namespace {static ArggLib::ArggLib_impl::Registerbase<baseClass,derivedClass> reg##derivedClass(identifier); } int Class_factory_DUMMY_VARIABLE_DO_NOT_USE##derivedClass=0 



namespace ArggLib {

  template <typename baseClassType>
  class CLASSFACTORY_DLLEXPORT  Class_Factory {
  public:

    using MainType = typename baseClassType::MainType;
    using MainType_V = std::vector < MainType > ;
    using Parameter_t = typename baseClassType::Parameter_t;
    using Parameter_ref = typename baseClassType::Parameter_ref;
    using u_pointer = std::unique_ptr < baseClassType > ;

    typedef baseClassType* (*factoryfunc_raw)(Parameter_ref);
#if defined( WIN32) && !defined(__CLING__)
    using map_t = std::map <typename MainType,typename  Class_Factory<baseClassType>::factoryfunc_raw > ;
#else
    using map_t = std::map < MainType, Class_Factory<baseClassType>::factoryfunc_raw >;
#endif


    static u_pointer Create(const MainType & name, Parameter_ref params);
    template <typename T>
    static void Register(const MainType &  name) {
      do_register(name, basefactory<T>);
    }
    static MainType_V GetTypes();

    template <typename T>
    static baseClassType* basefactory(Parameter_ref params) {
      return new T(params);
    }


    static void do_register(const MainType & name, factoryfunc_raw func) {
      getInstance()[name] = func;
    }

    static map_t& getInstance(); 

  };


  namespace ArggLib_impl {
	  template <typename baseClass, typename DerivedClass>
	  class CLASSFACTORY_DLLEXPORT Registerbase {
	  public:
		  using MainType = typename baseClass::MainType;

		  Registerbase(const MainType & name) {
			  Class_Factory<baseClass>::template Register<DerivedClass>(name);
		  }
	  };

  }
}
#endif // ArggLib_factory_h__
