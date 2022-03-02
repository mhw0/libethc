#ifndef ETHC_HEX_H
#define ETHC_HEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int eth_is_hex(const char *str, int len, int strict);
ETHC_EXPORT int eth_hex_pad_left(char *dest, const char *str, int len,
                                 size_t width);
ETHC_EXPORT int eth_hex_pad_right(char *dest, const char *str, int len,
                                  size_t width);
ETHC_EXPORT int eth_hex_from_bytes(char *dest, const uint8_t *bytes,
                                   size_t len);
ETHC_EXPORT int eth_hex_to_bytes(uint8_t *dest, const char *hex, int len);

#ifdef __cplusplus
}
#endif

#endif
