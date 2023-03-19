#ifndef ETHC_RLP_H
#define ETHC_RLP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define ETH_RLP_ENCODE 1
#define ETH_RLP_DECODE 2
#define ETH_RLP_FRAME_INIT_SIZE 1024

struct eth_rlp {
  int m;
  struct eth_rlp_frame *cf;
};

struct eth_rlp_frame {
  struct eth_rlp_frame *pf;
  size_t fc;
  uint8_t *fp;
  size_t fs;
};

ETHC_EXPORT int eth_rlp_init(struct eth_rlp *dest, int m);
ETHC_EXPORT int eth_rlp_uint8(struct eth_rlp *dest, uint8_t *u8);
ETHC_EXPORT int eth_rlp_uint16(struct eth_rlp *dest, uint16_t *u16);
ETHC_EXPORT int eth_rlp_uint32(struct eth_rlp *dest, uint32_t *u32);
ETHC_EXPORT int eth_rlp_uint64(struct eth_rlp *dest, uint64_t *u64);
ETHC_EXPORT int eth_rlp_address(struct eth_rlp *dest, char **addr);
ETHC_EXPORT int eth_rlp_list(struct eth_rlp *dest);
ETHC_EXPORT int eth_rlp_list_end(struct eth_rlp *dest);
ETHC_EXPORT int eth_rlp_bytes(struct eth_rlp *dest, uint8_t **bytes, size_t *len);
ETHC_EXPORT int eth_rlp_hex(struct eth_rlp *dest, char **hex, int *len);
ETHC_EXPORT int eth_rlp_to_hex(char **dest, struct eth_rlp *src);
ETHC_EXPORT int eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len);
ETHC_EXPORT int eth_rlp_len(struct eth_rlp *dest, size_t *len, uint8_t *base);
ETHC_EXPORT int eth_rlp_free(struct eth_rlp *dest);

#ifdef __cplusplus
}
#endif

#endif
