#ifndef ETHC_ADDRESS_H
#define ETHC_ADDRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>

/**
 * @brief Checks whether the `addr` is an address or not.
 *
 * @param[in] addr Target address to check.
 * @return `1` if `addr` is valid, `-1` otherwise.
 */
ETHC_EXPORT int eth_is_address(const char *addr);

/**
 * @brief Checks whether the `addr` is a valid checksum address or not.
 *
 * @param[in] addr Target address to check.
 * @return `1` if `addr` is valid, `-1` otherwise.
 *
 * @code{.c}
 *   #include <stdio.h>
 *   #include <ethc/address.h>
 *
 *   int main(void) {
 *     const char *addr0 = "0x1B9F402eaE18F10B1fD20d15b0c91EA64e3Ae1Ed";
 *     const char *addr1 = "0x51fabbb96a297368f0bf2dedffa285a921f7d613";
 *
 *     printf("addr0 is checksum address: %d\n", eth_is_checksum_address(addr0));
 *     printf("addr1 is checksum address: %d\n", eth_is_checksum_address(addr1));
 *
 *     return 0
 *   }
 * @endcode
 */
ETHC_EXPORT int eth_is_checksum_address(const char *addr);

/**
 * @brief Converts given `addr` to checksum address.
 *
 * @param[out] addr Target address.
 * @return `1` on success, `-1` otherwise.
 *
 * @code{.c}
 *   #include <stdio.h>
 *   #include <ethc/address.h>
 *
 *   int main(void) {
 *     char *addr = "0x1b9f402eae18f10b1fd20d15b0c91ea64e3ae1ed";
 *
 *     printf("before checksum: %s\n", addr);
 *
 *     eth_to_checksum_address(addr);
 *
 *     printf("after checksumm: %s\n", addr);
 *
 *     return 0
 *   }
 * @endcode
 */
ETHC_EXPORT int eth_to_checksum_address(char *addr);

#ifdef __cplusplus
}
#endif

#endif
