#ifndef ETHC_HEX_H
#define ETHC_HEX_H
#include <stdlib.h>

int eth_is_hexstr(const char* input, size_t len, int prefix);
char *eth_hexstr_pad_left(const char* hexstr, size_t hexstr_len, size_t pad_len);
char *eth_hexstr_pad_right(const char* hexstr, size_t hexstr_len, size_t pad_len);

#endif
