#ifndef ETHC_COMMON_H
#define ETHC_COMMON_H

#ifdef _WIN32
#define ETHC_EXPORT __declspec(dllexport)
#elif __GNUC__
#define ETHC_EXPORT __attribute__((visibility("default")))
#else
#define ETHC_EXPORT
#endif

#endif
