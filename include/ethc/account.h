#ifndef ETHC_ACCOUNT_H
#define ETHC_ACCOUNT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

struct eth_account {
  uint8_t address[20];
  uint8_t privkey[32];
  uint8_t pubkey[64];
};

ETHC_EXPORT int eth_account_create(struct eth_account *dest, const uint8_t *entropy);
ETHC_EXPORT int eth_account_get_address(char *dest, const struct eth_account *src);
ETHC_EXPORT int eth_account_get_privkey(char *dest, const struct eth_account *src);
ETHC_EXPORT int eth_account_get_pubkey(char *dest, const struct eth_account *src);

#ifdef __cplusplus
}
#endif

#endif
