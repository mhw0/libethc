#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

int eth_keccak256(const uint8_t *data, size_t len, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif
