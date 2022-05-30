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

struct eth_signed {
  uint8_t hash[32];
  uint8_t r[32];
  uint8_t s[32];
};

ETHC_EXPORT int eth_account_create(struct eth_account *dest,
                                   const uint8_t *entropy);
ETHC_EXPORT int eth_account_from_privkey(struct eth_account *dest,
                                         const uint8_t *privkey);
ETHC_EXPORT int eth_account_get_address(char *dest,
                                        const struct eth_account *src);
ETHC_EXPORT int eth_account_get_privkey(char *dest,
                                        const struct eth_account *src);
ETHC_EXPORT int eth_account_get_pubkey(char *dest,
                                       const struct eth_account *src);
ETHC_EXPORT int eth_account_sign(struct eth_signed *dest,
                                 const struct eth_account *account,
                                 const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
