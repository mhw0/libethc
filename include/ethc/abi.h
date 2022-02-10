#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int eth_abi_encode_bool(char *rstr, int b);
ETHC_EXPORT int eth_abi_encode_address(char *rstr, char *str);
ETHC_EXPORT int eth_abi_encode_int(char *rstr, const char *str, uint16_t nbits);
ETHC_EXPORT int eth_abi_encode_uint(char *rstr, const char *str,
                                    uint16_t nbits);
ETHC_EXPORT int eth_abi_encode_bytes(char *rstr, const uint8_t *bytes,
                                     uint8_t size);
ETHC_EXPORT char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len);
ETHC_EXPORT char *eth_abi_encode_stringd(const char *str, int len);
ETHC_EXPORT int eth_abi_encode_func(char *rstr, const char *func, int len);
ETHC_EXPORT int eth_abi_encode_event(char *rstr, const char *event, int len);

#ifdef __cplusplus
}
#endif

#endif
