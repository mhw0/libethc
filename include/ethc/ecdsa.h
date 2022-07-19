#ifndef ETHC_ECDSA_H
#define ETHC_ECDSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

struct eth_ecdsa_signature {
  uint8_t r[32];
  uint8_t s[32];
  int recid;
};

int eth_ecdsa_pubkey_get(uint8_t *dest, const uint8_t *privkey);
int eth_ecdsa_sign(struct eth_ecdsa_signature *dest, const uint8_t *privkey,
                   const uint8_t *data32);

#ifdef __cplusplus
}
#endif

#endif
