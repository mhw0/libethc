#ifndef ETHC_HEX_H
#define ETHC_HEX_H
#include <stdlib.h>

int eth_is_hex_str(const char* input, size_t len);
char *eth_hex_pad_left(const char* hexstr, size_t hexstr_len, size_t pad_len);
char *eth_hex_pad_right(const char* hexstr, size_t hexstr_len, size_t pad_len);

#endif
