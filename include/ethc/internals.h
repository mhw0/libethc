#ifndef ETHC_INTERNALS_H
#define ETHC_INTERNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int ethc_rand(uint8_t *bytes, size_t len);

#ifdef __cplusplus
}
#endif

#endif
