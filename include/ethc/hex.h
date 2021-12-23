#ifndef ETHC_HEX_H
#define ETHC_HEX_H
#include <stddef.h>

int eth_is_hexstr(const char* str, size_t len, int strict);
char *eth_hexstr_pad_left(const char* str, size_t str_len, size_t width);
char *eth_hexstr_pad_right(const char* str, size_t str_len, size_t width);

#endif
