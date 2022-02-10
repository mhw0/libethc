#ifndef ETHC_HEX_H
#define ETHC_HEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int eth_is_hex(const char *str, ETHC_EXPORT int len,
                           ETHC_EXPORT int strict);
ETHC_EXPORT int eth_hex_pad_left(char *rstr, const char *str,
                                 ETHC_EXPORT int len, size_t width);
ETHC_EXPORT int eth_hex_pad_right(char *rstr, const char *str,
                                  ETHC_EXPORT int len, size_t width);
ETHC_EXPORT int eth_hex_from_bytes(char *rstr, const uint8_t *bytes,
                                   size_t len);

#ifdef __cplusplus
}
#endif

#endif
