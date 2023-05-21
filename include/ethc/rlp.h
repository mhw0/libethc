#ifndef ETHC_RLP_H
#define ETHC_RLP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define ETH_RLP_ENCODE 0
#define ETH_RLP_DECODE 1
#define ETHC_RLP_FRAME_INITIAL_SIZE 1024

struct eth_rlp {
  struct ethc_rlp_frame *cframe;
  int m;
};

struct ethc_rlp_frame {
  struct ethc_rlp_frame *pframe;
  size_t offset;
  uint8_t *buf;
  size_t len;
};

ETHC_EXPORT int eth_rlp_init(struct eth_rlp *rlp, int m);
ETHC_EXPORT int eth_rlp_uint8(struct eth_rlp *rlp, uint8_t *d);
ETHC_EXPORT int eth_rlp_uint16(struct eth_rlp *rlp, uint16_t *d);
ETHC_EXPORT int eth_rlp_uint32(struct eth_rlp *rlp, uint32_t *d);
ETHC_EXPORT int eth_rlp_uint64(struct eth_rlp *rlp, uint64_t *d);
ETHC_EXPORT int eth_rlp_address(struct eth_rlp *rlp, char **addr);
ETHC_EXPORT int eth_rlp_array(struct eth_rlp *rlp);
ETHC_EXPORT int eth_rlp_array_end(struct eth_rlp *rlp);
ETHC_EXPORT int eth_rlp_bytes(struct eth_rlp *rlp, uint8_t **bytes, size_t *len);
ETHC_EXPORT int eth_rlp_hex(struct eth_rlp *rlp, char **hex, int *len);
ETHC_EXPORT int eth_rlp_to_hex(char **dest, struct eth_rlp *src);
ETHC_EXPORT int eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len);
ETHC_EXPORT int eth_rlp_len(struct eth_rlp *rlp, size_t *len, uint8_t *base);
ETHC_EXPORT int eth_rlp_free(struct eth_rlp *rlp);

#ifdef __cplusplus
}
#endif

#endif
