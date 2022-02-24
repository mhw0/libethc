#ifndef ETHC_ECC_H
#define ETHC_ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stdint.h>

int eth_ec_get_pubkey(uint8_t *pubkey, const uint8_t *privkey);

#ifdef __cplusplus
}
#endif

#endif
