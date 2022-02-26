#ifndef ETHC_ECC_H
#define ETHC_ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stdint.h>
#include <stddef.h>

struct eth_ecdsa_sig {
  uint8_t msghash[32];
  uint8_t r[32];
  uint8_t s[32];
};

int eth_ec_get_pubkey(uint8_t *pubkey, const uint8_t *privkey);
int eth_ecdsa_sign(struct eth_ecdsa_sig *sig, uint8_t *privkey, uint8_t *bytes, size_t len);

#ifdef __cplusplus
}
#endif

#endif
