#ifndef ArggLib_factoryDefinitionen_h__
#define ArggLib_factoryDefinitionen_h__

#include "ArggLib/factory.hh"
#include <iostream>
#include <string>


#ifndef  ArggLib_Class_factory_Utilities_THROW
#define ArggLib_Class_factory_Utilities_THROW(msg,inPutVar) std::cout << "[Factory<baseClassType>::Create" << ":" << __LINE__ << "]: \n " << msg << " <" <<  inPutVar <<">"<<std::endl;
#endif


#define ArggLib_registerBaseClass(BaseClass) namespace{\
                                         void dummy_register_function_##BaseClass(BaseClass::MainType mType, BaseClass::Parameter_ref pType){\
                                           ArggLib::Class_Factory<BaseClass>::Create(mType,pType );       \
                                           ArggLib::Class_Factory<BaseClass>::GetTypes();          \
                                           ArggLib::Class_Factory<BaseClass>::getInstance();\
                                           }}  \
                                     int Class_factory_VARIABLE_DO_NOT_USE##BaseClass=0

namespace ArggLib {
  template <typename baseClassType>
  typename Class_Factory<baseClassType>::MainType_V Class_Factory<baseClassType>::GetTypes()
  {
    std::vector<MainType> result;
    for (auto& e : getInstance()) {
      result.push_back(e.first);
    }
    return result;
  }

  template <typename baseClassType>
  typename Class_Factory<baseClassType>::u_pointer  Class_Factory<baseClassType>::Create(const MainType & name, Parameter_ref params /*= ""*/)
  {
    
    for (const auto& e : getInstance()) {
      if (e.first == name) {
        return u_pointer((e.second)(params));
      }
    
    }
    
    ArggLib_Class_factory_Utilities_THROW("unknown class" , name);
    return nullptr;

  }

  template <typename baseClassType>
  typename Class_Factory<baseClassType>::map_t& Class_Factory<baseClassType>::getInstance(){
    static map_t m;
    return m;
  }




}
#endif // ArggLib_factoryDefinitionen_h__
