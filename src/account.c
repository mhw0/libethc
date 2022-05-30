#include <ethc/account.h>
#include <ethc/ecc.h>
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

  op = eth_ec_get_pubkey(pubkey, privkey);
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
  uint8_t pubkey[64];
  int op;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  op = eth_ec_get_pubkey(pubkey, src->privkey);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  return eth_hex_from_bytes(dest, pubkey, 64);
}

int eth_account_sign(struct eth_signed *dest, const struct eth_account *account,
                     const uint8_t *data, size_t len) {
  uint8_t keccak[32], der[71], der_size = 71, *ptr = der;
  secp256k1_context *secp_ctx;
  secp256k1_ecdsa_signature secp_sig;
  int op, r;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(account != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(data != NULL, ETHC_FAIL);

  secp_ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  ETHC_RETURN_IF_FALSE(secp_ctx != NULL, ETHC_FAIL);

  op = eth_keccak256p(keccak, data, len);
  ETHC_RETURN_IF_FALSE(op == ETHC_SUCCESS, ETHC_FAIL);

  memcpy(dest->hash, keccak, 32);

  r = secp256k1_ecdsa_sign(secp_ctx, &secp_sig, keccak, account->privkey, NULL,
                           NULL);
  ETHC_RETURN_IF_FALSE(r != 0, ETHC_FAIL);

  r = secp256k1_ecdsa_signature_serialize_der(secp_ctx, der,
                                              (size_t *)&der_size, &secp_sig);
  ETHC_RETURN_IF_FALSE(r != 0, ETHC_FAIL);

  /*
    DER:
     +------+--------+------+---------+--------+------+---------+--------+
     | 0x30 | <desc> | 0x02 | <r len> | <r BE> | 0x02 | <s len> | <s BE> |
     +------+--------+------+---------+--------+------+---------+--------+
  */
  ptr += (der[3] > 0x20 ? 5 : 4);
  memcpy(dest->r, ptr, 32);
  ptr += 32 + (ptr[33] > 0x20 ? 3 : 2);
  memcpy(dest->s, ptr, 32);

  secp256k1_context_destroy(secp_ctx);
  return ETHC_SUCCESS;
}
