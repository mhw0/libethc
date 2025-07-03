#ifndef ETHC_ENS_H
#define ETHC_ENS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"

#define ETH_ENS_DELIMITER "."
#define ETH_ENS_MAX_TOKENS 32
#define ETH_ENS_MAX_COPIED_NAME_BUFFER_SIZE 128

/*!
 * @brief Applies `namehash` hashing algorithm on the given string
 * @details By default this function accepts only `ETH_ENS_MAX_COPIED_NAME_BUFFER_SIZE` (128)
 *          character long string and the max tokens should not exceed `ETH_ENS_MAX_TOKENS` (32).
 *
 *          This behavior can be tuned by overriding `ETH_ENS_MAX_TOKENS` and
 *          `ETH_ENS_MAX_COPIED_NAME_BUFFER_SIZE`.
 *
 * @param[out] dest Pointer to 32-byte allocated buffer
 * @param[in] name Target name
 * @return `ETH_OP`
 */
ETHC_EXPORT ETH_OP eth_ens_namehash(uint8_t *dest, char *name);

#ifdef __cplusplus
}
#endif

#endif
