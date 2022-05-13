#include <ethc/account.h>
#include <ethc/keccak256.h>
#include <ethc/ecc.h>
#include <ethc/internals.h>
#include <ethc/hex.h>
#include <string.h>
#include <stdio.h>

int eth_account_create(struct eth_account *dest, const uint8_t *entropy) {
  uint8_t pubkey[64], privkey[32], keccak[32], r[64];
  int op;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);

  op = ethc_rand(r, 32);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);
  
  if (entropy == NULL) {
    op = ethc_rand(privkey, 32);
    ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);
  } else {
    memcpy(r + 32, entropy, 32);
  }

  op = eth_keccak256(privkey, r, 64);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  op = eth_ec_get_pubkey(pubkey, privkey);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  op = eth_keccak256(keccak, pubkey, 64);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  memcpy(dest->privkey, privkey, 32);
  memcpy(dest->pubkey, pubkey, 64);
  memcpy(dest->address, keccak + 12, 20);

  return ETHC_SUCCESS;
}

int eth_account_get_address(char *dest, const struct eth_account *src) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  return eth_hex_from_bytes(dest, src->address, 20);
}

int eth_account_get_privkey(char *dest, const struct eth_account *src) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  return eth_hex_from_bytes(dest, src->privkey, 32);
}

int eth_account_get_pubkey(char *dest, const struct eth_account *src) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  return eth_hex_from_bytes(dest, src->privkey, 64);
}
