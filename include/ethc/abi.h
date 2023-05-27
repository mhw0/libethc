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

/*!
 * @brief Initializes the given abi struct.
 * 
 * @param[in] abi Target abi struct that needs to be initialized.
 * @param[in] m Mode in which the abi functions should work (accepts ``ETH_ABI_ENCODE`` or ``ETH_ABI_DECODE``)
 * @return `1` on success, `-1` otherwise.
 * @see `eth_abi_free`
 */
ETHC_EXPORT int eth_abi_init(struct eth_abi *abi, int m);

/*!
 * @brief Releases internal memory allocated for the given abi struct.
 * 
 * @param[in] abi Target abi
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_free(struct eth_abi *abi);

/*!
 * @brief Encodes/decodes "boolean value" (1 or 0)
 * 
 * @param[in] abi Target abi.
 * @param[inout] b Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_bool(struct eth_abi *abi, uint8_t *b);

/*!
 * @brief Encodes/decodes signed 8 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_int8(struct eth_abi *abi, int8_t *d);

/*!
 * @brief Encodes/decodes signed 16 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int16_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_int16(struct eth_abi *abi, int16_t *d);

/*!
 * @brief Encodes/decodes signed 32 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int32_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_int32(struct eth_abi *abi, int32_t *d);

/*!
 * @brief Encodes/decodes signed 64 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int64_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_int64(struct eth_abi *abi, int64_t *d);

/*!
 * @brief Encodes/decodes unsigned 8 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_uint8(struct eth_abi *abi, uint8_t *d);

/*!
 * @brief Encodes/decodes unsigned 16 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint16_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_uint16(struct eth_abi *abi, uint16_t *d);

/*!
 * @brief Encodes/decodes unsigned 32 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint32_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_uint32(struct eth_abi *abi, uint32_t *d);

/*!
 * @brief Encodes/decodes unsigned 64 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint64_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_uint64(struct eth_abi *abi, uint64_t *d);

/*!
 * @brief Encodes/decodes address.
 * 
 * @param[in] abi Target abi.
 * @param[inout] addr Ponter to address to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_address(struct eth_abi *abi, char **addr);

/*!
 * @brief Encodes/decodes 8 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes 16 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes 32 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes variable length bytes.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t array to read/write the data from/to.
 * @param[inout] len Length of encoded/decodes bytes.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_bytes(struct eth_abi *abi, uint8_t **bytes, size_t *len);

/*!
 * @brief Converts ABI to hex string.
 * 
 * @param[in] abi Target abi.
 * @param[out] hex Pointer to string where the hexadecimal value will be placed.
 * @param[out] len Pointer to size_to where the length of hexadecimal value will be placed.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len);

/*!
 * @brief Loads ABI from hex string.
 * 
 * @param[in] abi Target abi.
 * @param[out] hex Hexadecimal string.
 * @param[out] len Length of `hex`
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT int eth_abi_from_hex(struct eth_abi *abi, char *hex, int len);

/*!
 * @brief Denotes the start of a call.
 * 
 * @param[in] abi Target abi.
 * @param[inout] fn Pointer to string to read/write the data from/to.
 * @param[inout] len Pointer to int to read/write the length from/to.
 * @return `1` on success, `-1` otherwise.
 * 
 * @code{.c}
 *   // ...
 *   char *func = "balanceOf(address)";
 *   eth_abi_call(&abi, &func, NULL);
 * @endcode
 *
 * @note `len` can be `NULL` (`NULL` means the `fn` is NULL terminated on encode and the length is not needed on decode)
 * @see `eth_abi_call_end`
 */
ETHC_EXPORT int eth_abi_call(struct eth_abi *abi, char **fn, int *len);

/*!
 * @brief Denotes the end of a call.
 * 
 * @param[in] abi Target abi.
 */
ETHC_EXPORT int eth_abi_call_end(struct eth_abi *abi);

/*!
 * @brief Denotes the start of an array.
 * 
 * @param[in] abi Target abi.
 * @param[out] len Length of the array.
 *
 * @code{.c}
 *   // ...
 *   eth_abi_call(&abi, NULL);
 *     eth_abi_uint8(&abi, &myint);
 *   eth_abi_call_end(&abi, NULL);
 * @endcode
 *
 * @note `len` is ignored on encode.
 * @see `eth_abi_array_end`
 */
ETHC_EXPORT int eth_abi_array(struct eth_abi *abi, uint64_t *len);

/*!
 * @brief Denotes the end of an array.
 * 
 * @param[in] abi Target abi.
 */
ETHC_EXPORT int eth_abi_array_end(struct eth_abi *abi);


#ifdef __cplusplus
}
#endif

#endif
