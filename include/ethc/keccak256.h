#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int eth_keccak256(uint8_t *dest, const uint8_t *bytes, size_t len);

#ifdef __cplusplus
}
#endif

#endif
