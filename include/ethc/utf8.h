#ifndef ETHC_UTF8_H
#define ETHC_UTF8_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>

ETHC_EXPORT size_t eth_utf8_strlen(const char *str, int len);

#ifdef __cplusplus
}
#endif

#endif
