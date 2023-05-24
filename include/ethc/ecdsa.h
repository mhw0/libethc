#ifndef ETHC_ECDSA_H
#define ETHC_ECDSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

/*! @brief Holds ECDSA signature */
struct eth_ecdsa_signature {
  /*! @brief R value */
  uint8_t r[32];
  /*! @brief S value */
  uint8_t s[32];
  /*! @brief Recovery id */
  int recid;
};

#define eth_signed eth_ecdsa_signature

/*!
 * @brief Extracts public key from private key.
 *
 * @param[out] dest A pointer to 64-byte array where the public key will be placed.
 * @param[in] privkey A pointer to 32-byte array to read the private key from.
 * @return `1` on success, `-1` otherwise.
 */
int eth_ecdsa_pubkey_get(uint8_t *dest, const uint8_t *privkey);

/*!
 * @brief Creates an ECDSA signature.
 *
 * @param[out] dest A pointer to `eth_ecdsa_signature` where the signature will be placed..
 * @param[in] privkey A pointer to 32-byte array to read the private key from.
 * @param[in] data32 A pointer to 32-byte input data.
 * @return `1` on success, `-1` otherwise.
 */
int eth_ecdsa_sign(struct eth_ecdsa_signature *dest, const uint8_t *privkey,
                   const uint8_t *data32);

#ifdef __cplusplus
}
#endif

#endif
