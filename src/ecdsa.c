#include <ethc/ecdsa.h>
#include <ethc/keccak256.h>
#include <secp256k1_recovery.h>
#include <string.h>

int eth_ecdsa_pubkey_get(uint8_t *dest, const uint8_t *privkey) {
  secp256k1_context *secp_ctx;
  secp256k1_pubkey secp_pub;
  size_t outlen = 65;
  uint8_t tmp[outlen];
  int r = 0;

  if (dest == NULL || privkey == NULL)
    return -1;

  secp_ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  if (secp_ctx == NULL)
    return -1;

  r = secp256k1_ec_pubkey_create(secp_ctx, &secp_pub, privkey);
  if (r == 0)
    return -1;

  secp256k1_ec_pubkey_serialize(secp_ctx, tmp, &outlen, &secp_pub,
                                SECP256K1_EC_UNCOMPRESSED);
  memcpy(dest, tmp + 1, 64);
  secp256k1_context_destroy(secp_ctx);
  return 1;
}

int eth_ecdsa_sign(struct eth_ecdsa_signature *dest, const uint8_t *privkey,
                   const uint8_t *bytes32) {
  secp256k1_context *secp_ctx;
  secp256k1_ecdsa_recoverable_signature secp_sig;
  uint8_t signature[64];
  int r = 0;

  if (dest == NULL || privkey == NULL || bytes32 == NULL)
    return -1;

  secp_ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  if (secp_ctx == NULL)
    return -1;

  r = secp256k1_ecdsa_sign_recoverable(secp_ctx, &secp_sig, bytes32, privkey,
                                       NULL, NULL);
  if (r == 0)
    return -1;

  r = secp256k1_ecdsa_recoverable_signature_serialize_compact(
      secp_ctx, signature, &dest->recid, &secp_sig);
  if (r == 0)
    return -1;

  memcpy(dest->r, signature, 32);
  memcpy(dest->s, signature + 32, 32);

  secp256k1_context_destroy(secp_ctx);
  return 1;
}
