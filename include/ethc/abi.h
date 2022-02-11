#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int eth_abi_encode_bool(char *dest, int b);
ETHC_EXPORT int eth_abi_encode_address(char *dest, char *addr);
ETHC_EXPORT int eth_abi_encode_int(char *dest, const char *intstr, uint16_t nbits);
ETHC_EXPORT int eth_abi_encode_uint(char *dest, const char *intstr,
                                    uint16_t nbits);
ETHC_EXPORT int eth_abi_encode_bytes(char *dest, const uint8_t *bytes,
                                     uint8_t size);
ETHC_EXPORT char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len);
ETHC_EXPORT char *eth_abi_encode_stringd(const char *str, int len);
ETHC_EXPORT int eth_abi_encode_func(char *dest, const char *func, int len);
ETHC_EXPORT int eth_abi_encode_event(char *dest, const char *event, int len);

#ifdef __cplusplus
}
#endif

#endif
