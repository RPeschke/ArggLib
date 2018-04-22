#ifndef ArggLib_old_processor_definitions
#define ArggLib_old_processor_definitions

namespace ArggLib {


#define  DEFINE_PROC0(name, nextProcessorName)     class name ## imple { public: \
template < typename NEXT_T>\
   procReturn operator()( NEXT_T&& nextProcessorName) const ;}; \
inline auto name() ->decltype (proc() >> name ## imple()){return proc() >> name ## imple();}\
template < typename NEXT_T>\
   procReturn  name ## imple ::operator()( NEXT_T&& nextProcessorName) const 


#define  DEFINE_PROC1(name, nextProcessorName,inputName)     class name ## imple { public: \
template < typename NEXT_T,typename BLOCKS_T>\
   procReturn operator()( NEXT_T&& nextProcessorName,BLOCKS_T&& inputName) const ;}; \
inline auto name() ->decltype (proc() >> name ## imple()){return proc() >> name ## imple();}\
template < typename NEXT_T,typename BLOCKS_T>\
   procReturn name ## imple::operator()( NEXT_T&& nextProcessorName,BLOCKS_T&& inputName) const 

#define  DEFINE_PROC2(name, nextProcessorName,inputName1,inputName2)     class  name ## imple  { public: \
template < typename NEXT_T,typename BLOCKS_T1,typename BLOCKS_T2>\
   procReturn operator()( NEXT_T&& nextProcessorName,BLOCKS_T1&& inputName1,BLOCKS_T2&& inputName2) const ;}; \
inline auto name()  ->decltype (proc() >> name ## imple()) {return proc() >> name ## imple();}\
template < typename NEXT_T,typename BLOCKS_T1,typename BLOCKS_T2>\
   procReturn  name ## imple ::operator()( NEXT_T&& nextProcessorName,BLOCKS_T1&& inputName1,BLOCKS_T2&& inputName2) const 


#define  DEFINE_PROC3(name, nextProcessorName,inputName1,inputName2,inputName3)     class  name ## imple  { public: \
template < typename NEXT_T,typename BLOCKS_T1,typename BLOCKS_T2,typename BLOCKS_T3>\
   procReturn operator()( NEXT_T&& nextProcessorName,BLOCKS_T1&& inputName1,BLOCKS_T2&& inputName2,BLOCKS_T3&& inputName3) const ;}; \
auto name()  ->decltype (proc() >> name ## imple()) {return proc() >> name ## imple();}\
template < typename NEXT_T,typename BLOCKS_T1,typename BLOCKS_T2,typename BLOCKS_T3>\
   procReturn  name ## imple ::operator()( NEXT_T&& nextProcessorName,BLOCKS_T1&& inputName1,BLOCKS_T2&& inputName2,BLOCKS_T3&& inputName3) const 

#define  DEFINE_PROC_V(name, nextProcessorName,inputNameV)     class name ## imple { public: \
template < typename NEXT_T,typename... BLOCKS_T>\
   procReturn operator()( NEXT_T&& nextProcessorName,BLOCKS_T&&... inputNameV) const ;}; \
inline auto name()  ->decltype (proc() >> name ## imple()) {return proc() >> name ## imple();}\
template < typename NEXT_T,typename... BLOCKS_T>\
   procReturn name ## imple::operator()( NEXT_T&& nextProcessorName,BLOCKS_T&&... inputNameV) const 



}

#endif //ArggLib_old_processor_definitions