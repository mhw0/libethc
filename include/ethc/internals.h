#ifndef ETHC_INTERNALS_H
#define ETHC_INTERNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

ETHC_EXPORT int ethc_rand(uint8_t *bytes, size_t len);
ETHC_EXPORT int ethc_strncasecmp(const char *s1, const char *s2, size_t len);
ETHC_EXPORT int ethc_hexcharb(char h);
ETHC_EXPORT char ethc_hexchar(uint8_t d);

#ifdef __cplusplus
}
#endif

#endif
