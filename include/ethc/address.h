#ifndef ETHC_ADDRESS_H
#define ETHC_ADDRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>

ETHC_EXPORT int eth_is_address(const char *addr);
ETHC_EXPORT int eth_is_checksum_address(const char *addr);
ETHC_EXPORT int eth_to_checksum_address(char *addr);

#ifdef __cplusplus
}
#endif

#endif
