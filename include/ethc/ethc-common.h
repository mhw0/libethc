#ifndef ETHC_COMMON_H
#define ETHC_COMMON_H

#ifdef _WIN32
#define ETHC_EXPORT __declspec(dllexport)
#elif __GNUC__
#define ETHC_EXPORT __attribute__((visibility("default")))
#else
#define ETHC_EXPORT
#endif

#define ETHC_SUCCESS 1
#define ETHC_FAIL 0

#define ETHC_RETURN_IF_FALSE(expr, val)                                        \
  if (!(expr))                                                                 \
  return val

#endif
