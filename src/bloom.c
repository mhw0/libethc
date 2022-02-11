#include <ethc/bloom.h>
#include <ethc/keccak256.h>

int eth_bloom_create(struct eth_bloom *bloom) {
  size_t i;

  ETHC_RETURN_IF_FALSE(bloom != NULL, ETHC_FAIL);

  for (i = 0; i < BITSETS; i++)
    bloom->bitsets[i] = 0;

  return ETHC_SUCCESS;
}

int eth_bloom_from_bytes(struct eth_bloom *bloom, const uint8_t *bytes,
                         size_t len) {
  uint8_t keccak[32], bitset;
  uint16_t bitpos;
  size_t i;

  ETHC_RETURN_IF_FALSE(bloom != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  eth_bloom_create(bloom);

  if (!eth_keccak256(keccak, bytes, len))
    return ETHC_FAIL;

  for (i = 0; i < 6; i += 2) {
    bitpos = ((keccak[i] & 0xFF) << 0x8) + (keccak[i + 1] & 0xFF) & 0x7ff;
    bitset = (BITSETS - (bitpos / BITS_PER_BITSET)) - 1;
    bloom->bitsets[bitset] |= (1ULL << (bitpos % BITS_PER_BITSET));
  }

  return ETHC_SUCCESS;
}

int eth_bloom_copy(struct eth_bloom *dest, const struct eth_bloom *src) {
  size_t i;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  for (i = 0; i < BITSETS; i++)
    dest->bitsets[i] = src->bitsets[i];

  return ETHC_SUCCESS;
}

int eth_bloom_and(const struct eth_bloom *bloom1,
                  const struct eth_bloom *bloom2) {
  struct eth_bloom tmp_bloom;
  size_t i;

  ETHC_RETURN_IF_FALSE(bloom1 != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bloom2 != NULL, ETHC_FAIL);

  eth_bloom_create(&tmp_bloom);
  eth_bloom_copy(&tmp_bloom, bloom1);
  eth_bloom_or(&tmp_bloom, bloom2);

  for (i = 0; i < BITSETS; i++)
    if (bloom1->bitsets[i] != tmp_bloom.bitsets[i])
      return ETHC_FAIL;

  return ETHC_SUCCESS;
}

int eth_bloom_or(struct eth_bloom *dest,
                 const struct eth_bloom *src) {
  size_t i;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(src != NULL, ETHC_FAIL);

  for (i = 0; i < BITSETS; i++) {
    dest->bitsets[i] |= src->bitsets[i];
  }

  return ETHC_SUCCESS;
}
