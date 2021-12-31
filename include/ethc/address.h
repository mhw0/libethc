#ifndef ETHC_ADDRESS_H
#define ETHC_ADDRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int eth_is_address(const char *addr);
int eth_is_checksum_address(const char *addr);
int eth_to_checksum_address(char *addr);

#ifdef __cplusplus
}
#endif

#endif
