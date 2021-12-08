#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H
#include <stddef.h>
#include <stdint.h>

int eth_keccak256(const uint8_t *data, size_t len, uint8_t *out);

#endif
