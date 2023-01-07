#ifndef ETHC_BLOOM_H
#define ETHC_BLOOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

#define BITS_PER_BITSET 64
#define BITSETS 32

struct eth_bloom {
  uint64_t bitsets[BITSETS];
};

ETHC_EXPORT int eth_bloom_init(struct eth_bloom *dest);
ETHC_EXPORT int eth_bloom_from_bytes(struct eth_bloom *bloom,
                                     const uint8_t *bytes, size_t len);
ETHC_EXPORT int eth_bloom_and(const struct eth_bloom *lhs,
                              const struct eth_bloom *rhs);
ETHC_EXPORT int eth_bloom_or(struct eth_bloom *lhs,
                             const struct eth_bloom *rhs);
ETHC_EXPORT int eth_bloom_copy(struct eth_bloom *dest,
                               const struct eth_bloom *src);

#ifdef __cplusplus
}
#endif

#endif
