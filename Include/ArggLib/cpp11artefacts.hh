#ifndef cpp11artefacts_h__
#define cpp11artefacts_h__

#ifdef __CLING__

#define  CPP11ARTEFACT(x) x
#define  __constexpr__
#else
#define  CPP11ARTEFACT(x) 
#define  __constexpr__ constexpr
#endif // __CLING__


#endif // cpp11artefacts_h__
