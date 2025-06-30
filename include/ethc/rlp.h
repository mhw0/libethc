#ifndef ETHC_RLP_H
#define ETHC_RLP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define ETHC_RLP_BUFFER_INIT_SIZE 1024

/* represents the RLP mode (encoding or decoding) */
enum eth_rlp_mode {
  ETH_RLP_ENCODE = 0,
  ETH_RLP_DECODE = 1
};

struct eth_rlp {
  /* points to the current buffer */
  struct ethc_rlp_buffer *cbuf;
  /* RLP mode (ETH_RLP_ENCODE or ETH_RLP_DECODE) */
  enum eth_rlp_mode m;
};

struct ethc_rlp_buffer {
  /* pointer to the parent buffer */
  struct ethc_rlp_buffer *pbuf;
  /* indicates where we are at */
  size_t offset;
  /* pointer to the raw buffer */
  char *rawbuf;
  /* raw buffer size */
  size_t len;
};

/*!
 * @brief Initializes the rlp
 * @details Before using rlp struct, it must be initializes using this function
 *
 * @param[in] rlp Target rlp
 * @param[in] m RLP mode (ETH_RLP_ENCODE or ETH_RLP_DECODE)
 * @return `enum ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_init(struct eth_rlp *rlp, enum eth_rlp_mode m);
ETHC_EXPORT ETH_OP eth_rlp_uint8(struct eth_rlp *rlp, uint8_t *d);
ETHC_EXPORT ETH_OP eth_rlp_uint16(struct eth_rlp *rlp, uint16_t *d);
ETHC_EXPORT ETH_OP eth_rlp_uint32(struct eth_rlp *rlp, uint32_t *d);
ETHC_EXPORT ETH_OP eth_rlp_uint64(struct eth_rlp *rlp, uint64_t *d);

/*!
 * @brief Encodes/decodes at most 64 bit unsigned integer.
 * @details The difference between this function and `eth_rlp_uint64` is that,
 *          `eth_rlp_uint64` reads exactly 8 bytes from the integer, while this
 *          function will read bytes based on the size of the integer.
 *
 * @param[in] rlp Target rlp.
 * @param[inout] d Ponter to uint64_t to read/write the data from/to.
 * @return `enum ETH_OP`.
 */
ETHC_EXPORT ETH_OP eth_rlp_uint(struct eth_rlp *rlp, uint64_t *d);

/*!
 * @brief Encodes/decodes address
 *
 * @param[in] rlp Target rlp
 * @param[inout] addr Ponter to address read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_address(struct eth_rlp *rlp, char **addr);
ETHC_EXPORT ETH_OP eth_rlp_array(struct eth_rlp *rlp);
ETHC_EXPORT ETH_OP eth_rlp_array_end(struct eth_rlp *rlp);
ETHC_EXPORT ETH_OP eth_rlp_bytes(struct eth_rlp *rlp, uint8_t **bytes, size_t *len);
ETHC_EXPORT ETH_OP eth_rlp_hex(struct eth_rlp *rlp, char **hex, int *len);
ETHC_EXPORT ETH_OP eth_rlp_len(struct eth_rlp *rlp, size_t *len, uint8_t *base);
ETHC_EXPORT ETH_OP eth_rlp_to_hex(char **dest, struct eth_rlp *src);
ETHC_EXPORT ETH_OP eth_rlp_to_bytes(uint8_t **dest, size_t *len, struct eth_rlp *src);
ETHC_EXPORT ETH_OP eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len);
ETHC_EXPORT ETH_OP eth_rlp_free(struct eth_rlp *rlp);

#ifdef __cplusplus
}
#endif

#endif
