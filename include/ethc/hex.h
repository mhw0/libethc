#ifndef ETHC_HEX_H
#define ETHC_HEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

int eth_is_hex(const char *str, int len, int strict);
int eth_hex_pad_left(char *rstr, const char *str, int len, size_t width);
int eth_hex_pad_right(char *rstr, const char *str, int len, size_t width);
int eth_hex_from_bytes(char *rstr, const uint8_t *bytes, size_t len);

#ifdef __cplusplus
}
#endif

#endif
