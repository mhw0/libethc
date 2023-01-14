#include <stdlib.h>
#include <secp256k1.h>
#include <string.h>
#include <ethc/account.h>
#include <ethc/ecdsa.h>
#include <ethc/hex.h>
#include <ethc/internals.h>
#include <ethc/keccak256.h>

int eth_account_create(struct eth_account *dest, const uint8_t *entropy) {
  uint8_t pubkey[64], privkey[32], keccak[32], r[64];
  int op;

  if (dest == NULL)
    return -1;

  if (ethc_rand(r, 32) != 1)
    return -1;

  if (entropy == NULL) {
    if (ethc_rand(privkey, 32) != 1)
      return -1;
  } else {
    memcpy(r + 32, entropy, 32);
  }

  if (eth_keccak256(privkey, r, 64) != 1)
    return -1;

  if (eth_ecdsa_pubkey_get(pubkey, privkey) != 1)
    return -1;

  if (eth_keccak256(keccak, pubkey, 64) != 1)
    return -1;

  memcpy(dest->privkey, privkey, 32);
  memcpy(dest->pubkey, pubkey, 64);
  memcpy(dest->address, keccak + 12, 20);

  return 1;
}

int eth_account_from_privkey(struct eth_account *dest, const uint8_t *privkey) {
  uint8_t pubkey[64], keccak[32];
  int op;

  if (dest == NULL || privkey == NULL)
    return -1;

  if (eth_ecdsa_pubkey_get(pubkey, privkey) != 1)
    return -1;

  if (eth_keccak256(keccak, pubkey, 64) != 1)
    return -1;

  memcpy(dest->privkey, privkey, 32);
  memcpy(dest->pubkey, pubkey, 64);
  memcpy(dest->address, keccak + 12, 20);

  return 1;
}

int eth_account_address_get(char *dest, const struct eth_account *src) {
  int saddr;
  char *buf;

  if (dest == NULL || src == NULL)
    return -1;

  if ((saddr = eth_hex_from_bytes(&buf, src->address, 20)) < 0)
    return -1;

  memcpy(dest, buf, saddr);
  free(buf);

  return saddr;
}

int eth_account_privkey_get(char *dest, const struct eth_account *src) {
  int spriv;
  char *buf;

  if (dest == NULL || src == NULL)
    return -1;

  if ((spriv = eth_hex_from_bytes(&buf, src->privkey, 32)) < 0)
    return -1;

  memcpy(dest, buf, spriv);
  free(buf);

  return spriv;
}

int eth_account_pubkey_get(char *dest, const struct eth_account *src) {
  char *buf;
  uint8_t pubkey[64], spub;

  if (dest == NULL || src == NULL)
    return -1;

  if ((spub = eth_hex_from_bytes(&buf, src->pubkey, 64)) < 0)
    return -1;

  memcpy(dest, buf, spub);
  free(buf);

  return spub;
}

int eth_account_sign(struct eth_ecdsa_signature *dest,
                    const struct eth_account *account, 
                    const uint8_t *data, size_t len) {
  uint8_t keccak[32];
  int op;

  if (dest == NULL || account == NULL || data == NULL)
    return -1;

  if (eth_keccak256p(keccak, data, len) != 1)
    return -1;

  if (eth_ecdsa_sign(dest, account->privkey, keccak) != 1)
    return -1;

  return 1;
}
