#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

int eth_abi_encode_bool(char *rstr, int b);
int eth_abi_encode_address(char *rstr, char *str);
int eth_abi_encode_int(char *rstr, const char *str, uint16_t nbits);
int eth_abi_encode_uint(char *rstr, const char *str, uint16_t nbits);
int eth_abi_encode_bytes(char *rstr, const uint8_t *bytes, uint8_t size);
char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len);
int eth_abi_encode_func(char *rstr, const char *func, int len);
int eth_abi_encode_event(char *rstr, const char *event, int len);

#ifdef __cplusplus
}
#endif

#endif
