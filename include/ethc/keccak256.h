#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief Computes the keccak hash for the input data.
 *
 * @param[out] dest A pointer to a 32-byte array to write the hash to.
 * @param[in] bytes A pointer to the input data.
 * @param[in] len The length of the input data.
 *
 * @code{.c}
 *   #include <stdio.h>
 *   #include <ethc/keccak256.h>
 *
 *   int main(void) {
 *     uint8_t keccak[32], data[2] = {0x28, 0xa1};
 *     size_t datalen = 2, i;
 *
 *     eth_keccak256(keccak, data, datalen);
 *
 *     for(i = 0; i < 32; i++)
 *       printf("%.2x", keccak[i]);
 *     printf("\n");
 *
 *     return 0;
 *   }
 * @endcode
 */
ETHC_EXPORT int eth_keccak256(uint8_t *dest, const uint8_t *bytes, size_t len);

/*!
 * @brief Computes the keccak hash for the input data with `\x19Ethereum Signed Message:\n` prefix.
 *
 * @param[out] dest A pointer to a 32-byte array to write the hash to.
 * @param[in] bytes A pointer to the input data.
 * @param[in] len The length of the input data.
 */
ETHC_EXPORT int eth_keccak256p(uint8_t *dest, const uint8_t *bytes, size_t len);

#ifdef __cplusplus
}
#endif

#endif
