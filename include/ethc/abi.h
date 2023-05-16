#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

#define ETH_ABI_ENCODE 0
#define ETH_ABI_DECODE 1
#define ETH_ABI_WORD_SIZE 32
#define ETHC_ABI_BUFFER_INITIAL_SIZE ETH_ABI_WORD_SIZE * 128

struct eth_abi {
  struct ethc_abi_frame *cframe;
  int m;
};

struct ethc_abi_buf {
  uint8_t *buf;
  size_t len;
  size_t offset;
  size_t doffset;
};

struct ethc_abi_frame {
  struct ethc_abi_frame *pframe;
  struct ethc_abi_buf *buf;
  struct ethc_abi_buf *dybufs[64];
  uint8_t dybuflen;
  uint64_t len;
};

ETHC_EXPORT int eth_abi_init(struct eth_abi *abi, int m);
ETHC_EXPORT int eth_abi_free(struct eth_abi *abi);
ETHC_EXPORT int eth_abi_bool(struct eth_abi *abi, uint8_t *b);
ETHC_EXPORT int eth_abi_int8(struct eth_abi *abi, int8_t *d);
ETHC_EXPORT int eth_abi_int16(struct eth_abi *abi, int16_t *d);
ETHC_EXPORT int eth_abi_int32(struct eth_abi *abi, int32_t *d);
ETHC_EXPORT int eth_abi_int64(struct eth_abi *abi, int64_t *d);
ETHC_EXPORT int eth_abi_uint8(struct eth_abi *abi, uint8_t *d);
ETHC_EXPORT int eth_abi_uint16(struct eth_abi *abi, uint16_t *d);
ETHC_EXPORT int eth_abi_uint32(struct eth_abi *abi, uint32_t *d);
ETHC_EXPORT int eth_abi_uint64(struct eth_abi *abi, uint64_t *d);
ETHC_EXPORT int eth_abi_address(struct eth_abi *abi, char **addr);
ETHC_EXPORT int eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes);
ETHC_EXPORT int eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes);
ETHC_EXPORT int eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes);
ETHC_EXPORT int eth_abi_bytes(struct eth_abi *abi, uint8_t **bytes, size_t *len);
ETHC_EXPORT int eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len);
ETHC_EXPORT int eth_abi_from_hex(struct eth_abi *abi, char *hex, int len);
ETHC_EXPORT int eth_abi_call(struct eth_abi *abi, char **fn, int *len);
ETHC_EXPORT int eth_abi_call_end(struct eth_abi *abi);
ETHC_EXPORT int eth_abi_array(struct eth_abi *abi, uint64_t *len);
ETHC_EXPORT int eth_abi_array_end(struct eth_abi *abi);


#ifdef __cplusplus
}
#endif

#endif
