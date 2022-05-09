#ifndef ETHC_COMMON_H
#define ETHC_COMMON_H

#ifdef _WIN32
#define ETHC_EXPORT __declspec(dllexport)
#elif __GNUC__
#define ETHC_EXPORT __attribute__((visibility("default")))
#else
#define ETHC_EXPORT
#endif

#define ETHC_TRUE    (1u << 1)
#define ETHC_FALSE   (1u << 2)
#define ETHC_SUCCESS (1u << 3)
#define ETHC_FAIL    (1u << 4)

#define ETHC_RETURN_IF_FALSE(expr, val)                                        \
  if (!(expr))                                                                 \
  return val

#endif
