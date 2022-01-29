#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int eth_abi_encode_bool(char *rstr, int b);
int eth_abi_encode_address(char *rstr, char *str);
int eth_abi_encode_int(char *rstr, const char *str, uint16_t nbits);
int eth_abi_encode_uint(char *rstr, const char *str, uint16_t nbits);
int eth_abi_encode_bytes(char *rstr, const uint8_t *bytes, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif
