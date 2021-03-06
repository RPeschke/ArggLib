#ifndef ArggLib_RQSignals_h__
#define ArggLib_RQSignals_h__


#include <string>
#include <memory>
#include "TString.h"


template <typename T>
auto _RQ_signals(std::shared_ptr<T>& obj) ->decltype(_RQ_signals(obj.get())) {
  return _RQ_signals(obj.get());
}
template <typename T>
auto _RQ_signals(std::unique_ptr<T>& obj) ->decltype(_RQ_signals(obj.get()) ){
  return _RQ_signals(obj.get());
}
template <typename T>
auto _RQ_slots(std::shared_ptr<T>& obj)  ->decltype(_RQ_slots(obj.get()) ){
  return _RQ_slots(obj.get());
}
template <typename T>
auto _RQ_slots(std::unique_ptr<T>& obj) -> decltype(_RQ_slots(obj.get()) ){
  return _RQ_slots(obj.get());
}

#define  __DEFINE_RQ_SIGNAL__(funName,SignalString) /*emits SignalString*/ \
RQ_SIGNAL_TEMPLATE<T>  funName() { \
return RQ_SIGNAL_TEMPLATE<T>(this->m_object, SignalString, this->cname.Data()); \
  }

#define  __DEFINE_RQ_SIGNAL__CONSTRUCTOR(derivedname,baseName) derivedname<T>(T* obj, const char* ClassName) :baseName<T>(obj, ClassName) 


#define  __DEFINE_RQ_SIGNAL__FACTORY(ClassName,ClassSignalName) class ClassName;\
 inline ClassSignalName<ClassName> _RQ_signals(ClassName* obj) {\
  return ClassSignalName<ClassName>(obj, #ClassName);\
}\
inline ClassSignalName<ClassName> _RQ_signals(ClassName& obj) {\
  return ClassSignalName<ClassName>(&obj, #ClassName);\
}


#define START__DEFINE_RQ_SIGNAL_BASE_CLASS(CLASSNAME) class CLASSNAME; \
template <typename T> \
class CLASSNAME##_signals{ \
public: \
TString cname; \
T* m_object; \
 CLASSNAME##_signals(T* obj, const char* ClassName) : m_object(obj), cname(ClassName) {}

#define START__DEFINE_RQ_SIGNAL_DERIVED_CLASS(CLASSNAME,BASE_CLASS) class CLASSNAME; \
template <typename T> \
class  CLASSNAME##_signals : public BASE_CLASS##_signals<T> {\
public: \
 CLASSNAME##_signals<T>(T* obj, const char* ClassName) : BASE_CLASS##_signals<T>(obj, ClassName){}

#define  END__DEFINE_RQ_SIGNAL_CLASS(CLASSNAME) };\
 inline CLASSNAME##_signals<CLASSNAME> _RQ_signals(CLASSNAME* obj) {\
  return CLASSNAME##_signals<CLASSNAME>(obj, #CLASSNAME);\
}\
inline CLASSNAME##_signals<CLASSNAME> _RQ_signals(CLASSNAME& obj) {\
  return CLASSNAME##_signals<CLASSNAME>(&obj, #CLASSNAME);\
}


#define START__DEFINE_RQ_SLOTS_BASE_CLASS(CLASSNAME) class CLASSNAME; \
template <typename T> \
class CLASSNAME##_slots{ \
public: \
TString cname; \
T* m_object; \
 CLASSNAME##_slots(T* obj, const char* ClassName) : m_object(obj), cname(ClassName) {}


#define START__DEFINE_RQ_SLOTS_DERIVED_CLASS(CLASSNAME,BASE_CLASS) class CLASSNAME; \
template <typename T> \
class  CLASSNAME##_slots : public BASE_CLASS##_slots<T> {\
public: \
 CLASSNAME##_slots<T>(T* obj, const char* ClassName) : BASE_CLASS##_slots<T>(obj, ClassName){}

#define  END__DEFINE_RQ_SLOTS_CLASS(CLASSNAME) };\
 inline CLASSNAME##_slots<CLASSNAME> _RQ_slots(CLASSNAME* obj) {\
  return CLASSNAME##_slots<CLASSNAME>(obj, #CLASSNAME);\
}\
 inline CLASSNAME##_slots<CLASSNAME> _RQ_slots(CLASSNAME& obj) {\
  return CLASSNAME##_slots<CLASSNAME>(&obj, #CLASSNAME);\
}

#define __DEFINE_RQ_SLOT__(funName,SignalString)  \
RQ_SLOT_TEMPLATE<T>  funName() { \
return RQ_SLOT_TEMPLATE<T>(m_object, SignalString, cname.Data()); \
  }

#define  __DEFINE_RQ_SLOT__CONSTRUCTOR(derivedname,baseName) derivedname<T>(T* obj, const char* ClassName) :baseName<T>(obj, ClassName) 

#define  __DEFINE_RQ_SLOT__FACTORY(ClassName,ClassSignalName) inline ClassSignalName<ClassName> _RQ_slots(ClassName* obj) {\
  return ClassSignalName<ClassName>(obj, #ClassName);\
}\
inline ClassSignalName<ClassName> _RQ_slots(ClassName& obj) {\
  return ClassSignalName<ClassName>(&obj, #ClassName);\
}

template <typename T>
class RQ_SLOT_TEMPLATE {
public:
  RQ_SLOT_TEMPLATE(T* obj, const std::string& str, const std::string& class_name = "") :
    m_object(obj),
    m_name(str),
    m_className(class_name) {}

  T* m_object;
  std::string m_name, m_className;
};

template <typename T>
class RQ_SIGNAL_TEMPLATE :public RQ_SLOT_TEMPLATE<T> {
public:
  RQ_SIGNAL_TEMPLATE(T* obj, const std::string& str, const std::string& class_name = "") :RQ_SLOT_TEMPLATE<T>(obj, str, class_name) {}
};

template <typename T>
RQ_SIGNAL_TEMPLATE<T> __make_RQsignal(T* obj, const std::string& member, const std::string& class_name) {
  return RQ_SIGNAL_TEMPLATE<T>(obj, member, class_name);
}
template <typename T>
RQ_SIGNAL_TEMPLATE<T> __make_RQsignal(T* obj, const std::string& member) {
  return __make_RQsignal(obj, member, obj->Class_Name());
}

template <typename T>
RQ_SLOT_TEMPLATE<T> __make_RQslot(T* obj, const std::string& member) {
  return __make_RQsignal(obj, member);
}
template <typename T>
RQ_SLOT_TEMPLATE<T> __make_RQslot(T* obj, const std::string& member, const std::string& class_name) {
  return __make_RQsignal(obj, member, class_name);
}
template <typename T1, typename T2>
void operator >> (const RQ_SIGNAL_TEMPLATE<T1>& signal_, const RQ_SLOT_TEMPLATE<T2>& slot_) {

  signal_.m_object->Connect(signal_.m_name.c_str(), slot_.m_className.c_str(), slot_.m_object, slot_.m_name.c_str());
}



#endif // ArggLib_RQSignals_h__
