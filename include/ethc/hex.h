#ifndef ETHC_HEX_H
#define ETHC_HEX_H
#include <stddef.h>
#include <stdint.h>

int eth_is_hexstr(const char* str, size_t len, int strict);
char *eth_hexstr_pad_left(const char* str, size_t str_len, size_t width);
char *eth_hexstr_pad_right(const char* str, size_t str_len, size_t width);
int eth_hexstr_from_bytes(const uint8_t *bytes, size_t len, int prefix, char *hexstr);

#endif
