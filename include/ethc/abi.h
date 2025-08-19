#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>
#include <tommath.h>

#define ETH_ABI_WORD_SIZE 32
#define ETH_ABI_FUNCTION_SELECTOR_SIZE 4
#define ETH_ABI_FRAME_MAX_HEAD_WORDS 64
#define ETH_ABI_FRAME_MAX_TAIL_WORDS 128
#define ETH_ABI_FRAME_MAX_PATCHES 64
#define ETH_ABI_FRAME_MAX_STACK_SIZE 64
#define ETH_ABI_FRAME_MAX_POOL_SIZE 64

int s_mp_get_bit(const mp_int *a, int b);

// ABI mode enum (encoding or decoding)
enum eth_abi_mode {
  ETH_ABI_ENCODE = 0,
  ETH_ABI_DECODE = 1
};

// patch struct where dynamic data offets stored
struct ethc_abi_patch {
  size_t head_offset;
  size_t tail_offset;
};

struct ethc_abi_frame {
  // buffer for storing static elements
  size_t head_offset;
  uint8_t head[ETH_ABI_WORD_SIZE * ETH_ABI_FRAME_MAX_HEAD_WORDS];

  // buffer for storing dynamic and nested data
  size_t tail_offset;
  uint8_t tail[ETH_ABI_WORD_SIZE * ETH_ABI_FRAME_MAX_TAIL_WORDS];

  // buffer for dynamic data patches
  size_t patch_offset;
  struct ethc_abi_patch patches[ETH_ABI_FRAME_MAX_PATCHES];

  // size of the tail, used on decode
  size_t tail_size;
};

struct eth_abi {
  // ABI mode
  enum eth_abi_mode m;

  // dynamic data frame pool
  struct ethc_abi_frame pool[ETH_ABI_FRAME_MAX_POOL_SIZE];
  size_t pool_head;

  // frame stack for nested elements
  struct ethc_abi_frame *stack[ETH_ABI_FRAME_MAX_STACK_SIZE];
  int stack_top;

  // function selector
  uint8_t func[4];
  int func_set;
};

/*!
 * @brief Initializes the given abi struct.
 * 
 * @param[in] abi Target abi struct that needs to be initialized.
 * @param[in] m Mode in which the abi functions should work (accepts ``ETH_ABI_ENCODE`` or ``ETH_ABI_DECODE``)
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_init(struct eth_abi *abi, enum eth_abi_mode m);

/*!
 * @brief Encodes/decodes "boolean value" (1 or 0)
 * 
 * @param[in] abi Target abi.
 * @param[inout] b Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_bool(struct eth_abi *abi, uint8_t *b);

/*!
 * @brief Encodes/decodes signed 8 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_int8(struct eth_abi *abi, int8_t *d);

/*!
 * @brief Encodes/decodes signed 16 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int16_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_int16(struct eth_abi *abi, int16_t *d);

/*!
 * @brief Encodes/decodes signed 32 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int32_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_int32(struct eth_abi *abi, int32_t *d);

/*!
 * @brief Encodes/decodes signed 64 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to int64_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_int64(struct eth_abi *abi, int64_t *d);

/*!
 * @brief Encodes/decodes unsigned 8 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_uint8(struct eth_abi *abi, uint8_t *d);

/*!
 * @brief Encodes/decodes unsigned 16 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint16_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_uint16(struct eth_abi *abi, uint16_t *d);

/*!
 * @brief Encodes/decodes unsigned 32 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint32_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_uint32(struct eth_abi *abi, uint32_t *d);

/*!
 * @brief Encodes/decodes unsigned 64 bit integer.
 * 
 * @param[in] abi Target abi.
 * @param[inout] d Ponter to uint64_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_uint64(struct eth_abi *abi, uint64_t *d);

/*!
 * @brief Encodes/decodes arbitrarily large integer.
 *
 * @param[in] abi Target abi.
 * @param[inout] mpz Initialized mpz_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_mpint(struct eth_abi *abi, mp_int *mpint);

/*!
 * @brief Encodes/decodes address.
 * 
 * @param[in] abi Target abi.
 * @param[inout] addr Ponter to address to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_address(struct eth_abi *abi, char *addr);

/*!
 * @brief Encodes/decodes 8 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes 16 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes 32 byte array.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t to read/write the data from/to.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes);

/*!
 * @brief Encodes/decodes variable length bytes.
 * 
 * @param[in] abi Target abi.
 * @param[inout] bytes Ponter to uint8_t array to read/write the data from/to.
 * @param[inout] len Length of encoded/decodes bytes.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_bytes(struct eth_abi *abi, uint8_t *bytes, size_t *len);

/*!
 * @brief Converts ABI to hex string.
 * 
 * @param[in] abi Target abi.
 * @param[out] hex Pointer to string where the hexadecimal value will be placed.
 * @param[out] len Pointer to size_to where the length of hexadecimal value will be placed.
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len);

/*!
 * @brief Loads ABI from hex string.
 * 
 * @param[in] abi Target abi.
 * @param[out] hex Hexadecimal string.
 * @param[out] len Length of `hex`
 * @return `1` on success, `-1` otherwise.
 */
ETHC_EXPORT ETH_OP eth_abi_from_hex(struct eth_abi *abi, char *hex, int len);

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
ETHC_EXPORT ETH_OP eth_abi_call(struct eth_abi *abi, char **fn, int *len);

/*!
 * @brief Denotes the end of a call
 * 
 * @param[in] abi Target abi.
 */
ETHC_EXPORT ETH_OP eth_abi_call_end(struct eth_abi *abi);

/*!
 * @brief Denotes the start of an array
 * 
 * @param[in] abi Target abi
 * @param[out] len Length of the array
 *
 * @code{.c}
 *   // ...
 *   eth_abi_call(&abi, NULL);
 *     eth_abi_uint8(&abi, &myint);
 *   eth_abi_call_end(&abi, NULL);
 * @endcode
 *
 * @note `len` is ignored on encode
 * @see `eth_abi_array_end`
 */
ETHC_EXPORT ETH_OP eth_abi_array(struct eth_abi *abi, size_t *len);

/*!
 * @brief Denotes the end of an array
 * 
 * @param[in] abi Target abi.
 */
ETHC_EXPORT ETH_OP eth_abi_array_end(struct eth_abi *abi);

#ifdef __cplusplus
}
#endif

#endif
