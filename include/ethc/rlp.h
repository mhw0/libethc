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
  uint8_t *rawbuf;
  /* raw buffer size */
  size_t len;
};

/*!
 * @brief Initializes given RLP
 * @details Before using `eth_rlp` struct, it must be initializes using this function
 *
 * @param[in] rlp Target RLP
 * @param[in] m RLP mode (`ETH_RLP_ENCODE` or `ETH_RLP_DECODE`)
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_init(struct eth_rlp *rlp, enum eth_rlp_mode m);

/*!
 * @brief Encodes/decodes `uint8` data type
 *
 * @param[in] rlp Target RLP
 * @param[inout] d Ponter to `uint8` variable read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_uint8(struct eth_rlp *rlp, uint8_t *d);

/*!
 * @brief Encodes/decodes `uint16` data type
 *
 * @param[in] rlp Target RLP
 * @param[inout] d Ponter to `uint16` variable read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_uint16(struct eth_rlp *rlp, uint16_t *d);

/*!
 * @brief Encodes/decodes `uint32` data type
 *
 * @param[in] rlp Target RLP
 * @param[inout] d Ponter to `uint32` variable read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_uint32(struct eth_rlp *rlp, uint32_t *d);

/*!
 * @brief Encodes/decodes `uint64` data type
 *
 * @param[in] rlp Target RLP
 * @param[inout] d Ponter to `uint64` variable read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_uint64(struct eth_rlp *rlp, uint64_t *d);

/*!
 * @brief Encodes/decodes at most 64 bit unsigned integer
 * @details The difference between this function and `eth_rlp_uint64` is that,
 *          `eth_rlp_uint64` reads exactly 8 bytes from the integer, while this
 *          function will read bytes based on the size of the integer
 *
 * @param[in] rlp Target RLP
 * @param[inout] d Ponter to uint64_t to read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_uint(struct eth_rlp *rlp, uint64_t *d);

/*!
 * @brief Encodes/decodes address
 *
 * @param[in] rlp Target RLP
 * @param[inout] addr Ponter to address read/write the data from/to
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_address(struct eth_rlp *rlp, char **addr);

/*!
 * @brief Opens the array
 * @details As the C programming language does not support dynamic arrays, encoding/decoding
 *          of arrays are implemented using function calls.
 *
 *          When array is opened, `eth_rlp_array` should be called and `eth_rlp_array_end`
 *          when closed.
 *
 *          On decode, if the RLP data has empty array, it MUST be opened and closed
 *          to move to the next element in the array:
 *
 * @code
 * // RLP: ["a", [], "b", "c"]
 *
 * eth_rlp_array(&rlp)             // [
 *   // ...decode the element "a"  //  "a",
 *   eth_rlp_array(&rlp)           //  [
 *     // even if the array        //
 *     // is empty, we have to     //
 *     // call these               //
 *   eth_rlp_array_end(&rlp)       //  ],
 *   // ...decode the element "c"  //  "c"
 * eth_rlp_array_end(&rlp)         // ]
 *
 * @endcode
 *
 * @param[in] rlp Target RLP
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_array(struct eth_rlp *rlp);

/*!
 * @brief Closes the array
 *
 * @param[in] rlp Target RLP
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_array_end(struct eth_rlp *rlp);

/*!
 * @brief Encodes/decodes bytes
 *
 * @param[in] rlp Target RLP
 * @param[inout] bytes Ponter to byte array to read/write the data from/to
 * @param[inout] len Length of the `bytes` argument
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_bytes(struct eth_rlp *rlp, uint8_t **bytes, size_t *len);

/*!
 * @brief Encodes/decodes hexadecimal string
 *
 * @param[in] rlp Target RLP
 * @param[inout] bytes Ponter to hexadecimal string to read/write the data from/to
 * @param[inout] len Length of the hexadecimal string (`-1` means, the hexdecimal string
 *                   is NUL-terminated)
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_hex(struct eth_rlp *rlp, char **hex, int *len);

/*!
 * @brief Encodes/decodes data length
 *
 * @param[in] rlp Target RLP
 * @param[inout] len Ponter to data length to read/write the data from/to
 * @param[inout] base Pointer to length base
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_len(struct eth_rlp *rlp, size_t *len, uint8_t *base);

/*!
 * @brief Converts the data of given `eth_rlp` struct into hexadecimal string
 *
 * @param[out] rlp Target RLP
 * @param[out] dest Ponter to string to write the data to
 * @param[in] src Source RLP struct
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_to_hex(char **dest, struct eth_rlp *src);

/*!
 * @brief Converts the data of given `eth_rlp` struct into bytes
 *
 * @param[out] rlp Target RLP
 * @param[out] dest Pointer to byte array
 * @param[out] len Pointer to the length of the byte array to write the data to
 * @param[in] src Source RLP struct
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_to_bytes(uint8_t **dest, size_t *len, struct eth_rlp *src);

/*!
 * @brief Constructs `eth_rlp` struct from the given hexadecimal string
 *
 * @param[out] rlp Destination RLP struct
 * @param[in] hex Pointer to the hexadecimal string
 * @param[in] len Length of the hexadecimal string (`-1` if the hexadecimal string is
 *                NUL-terminated)
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len);

/*!
 * @brief Closes given `eth_rlp` struct by calling `free` on internal buffer
 *
 * @param[in] rlp Target RLP
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_rlp_free(struct eth_rlp *rlp);

#ifdef __cplusplus
}
#endif

#endif
