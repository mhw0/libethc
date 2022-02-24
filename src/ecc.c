#include <ethc/ecc.h>
#include <ethc/keccak256.h>
#include <secp256k1.h>
#include <string.h>

int eth_ec_get_pubkey(uint8_t *pubkey, const uint8_t *privkey) {
  secp256k1_context *secp_ctx;
  secp256k1_pubkey secp_pub;
  size_t outlen = 65;
  uint8_t tmp[outlen];
  int r = 0;

  ETHC_RETURN_IF_FALSE(pubkey != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(privkey != NULL, ETHC_FAIL);

  secp_ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  ETHC_RETURN_IF_FALSE(secp_ctx != NULL, ETHC_FAIL);

  r = secp256k1_ec_pubkey_create(secp_ctx, &secp_pub, privkey);
  ETHC_RETURN_IF_FALSE(r != 0, ETHC_FAIL);

  secp256k1_ec_pubkey_serialize(secp_ctx, tmp, &outlen, &secp_pub,
                                SECP256K1_EC_UNCOMPRESSED);
  memcpy(pubkey, tmp + 1, 64);

  secp256k1_context_destroy(secp_ctx);
  return ETHC_SUCCESS;
}
