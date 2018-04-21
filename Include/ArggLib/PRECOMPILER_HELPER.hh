#ifndef PRECOMPILER_HELPER_h__
#define PRECOMPILER_HELPER_h__

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define __MAKE_UNIQUE_PRECOMPILER_NAME__(x) CONCATENATE(x, __LINE__)

#define ARGGLIB_STRINGIFY(x) #x
#define ARGGLIB_TOSTRING(x) ARGGLIB_STRINGIFY(x)

#endif // PRECOMPILER_HELPER_h__
