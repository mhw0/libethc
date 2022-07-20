#include <ethc/account.h>
#include <ethc/ecdsa.h>
#include <ethc/hex.h>
#include <ethc/internals.h>
#include <ethc/keccak256.h>
#include <secp256k1.h>
#include <string.h>

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

  op = eth_ecdsa_pubkey_get(pubkey, privkey);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  op = eth_keccak256(keccak, pubkey, 64);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  memcpy(dest->privkey, privkey, 32);
  memcpy(dest->pubkey, pubkey, 64);
  memcpy(dest->address, keccak + 12, 20);

  return ETHC_SUCCESS;
}

int eth_account_from_privkey(struct eth_account *dest, const uint8_t *privkey) {
  uint8_t pubkey[64], keccak[32];
  int op;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(privkey != NULL, ETHC_FAIL);

  op = eth_ecdsa_pubkey_get(pubkey, privkey);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  op = eth_keccak256(keccak, pubkey, 64);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  memcpy(dest->privkey, privkey, 32);
  memcpy(dest->pubkey, pubkey, 64);
  memcpy(dest->address, keccak + 12, 20);

  return ETHC_SUCCESS;
}

int eth_account_address_get(char *dest, const struct eth_account *src) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  return eth_hex_from_bytes(dest, src->address, 20);
}

int eth_account_privkey_get(char *dest, const struct eth_account *src) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  return eth_hex_from_bytes(dest, src->privkey, 32);
}

int eth_account_pubkey_get(char *dest, const struct eth_account *src) {
  uint8_t pubkey[64];
  int op;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  op = eth_ecdsa_pubkey_get(pubkey, src->privkey);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  return eth_hex_from_bytes(dest, pubkey, 64);
}

int eth_account_sign(struct eth_signed *dest, const struct eth_account *account,
                     const uint8_t *data, size_t len) {
  uint8_t keccak[32];
  int op;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(account != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(data != NULL, ETHC_FAIL);

  /*
   * TODO(mhw0): While Geth expects the keccak256 messages to be prefixed
   * with "\x19Ethereum Signed Message:\n", other VMs may not expect/support
   * it. So this function should handle both cases in the near future.
   */
  op = eth_keccak256p(keccak, data, len);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  op = eth_ecdsa_sign(dest, account->privkey, keccak);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);
  return ETHC_SUCCESS;
}
