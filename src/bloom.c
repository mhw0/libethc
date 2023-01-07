#include <ethc/bloom.h>
#include <ethc/keccak256.h>

int eth_bloom_init(struct eth_bloom *dest) {
  size_t i;

  if (dest == NULL)
    return -1;

  for (i = 0; i < BITSETS; i++)
    dest->bitsets[i] = 0;

  return 1;
}

int eth_bloom_from_bytes(struct eth_bloom *bloom, const uint8_t *bytes,
                         size_t len) {
  uint8_t keccak[32], bitset;
  uint16_t bitpos;
  size_t i;

  if (bloom == NULL || bytes == NULL)
    return -1;

  if (eth_bloom_init(bloom) != 1)
    return -1;

  if (!eth_keccak256(keccak, bytes, len))
    return 1;

  for (i = 0; i < 6; i += 2) {
    bitpos = ((keccak[i] & 0xFF) << 0x8) + (keccak[i + 1] & 0xFF) & 0x7ff;
    bitset = (BITSETS - (bitpos / BITS_PER_BITSET)) - 1;
    bloom->bitsets[bitset] |= (1ULL << (bitpos % BITS_PER_BITSET));
  }

  return 1;
}

int eth_bloom_copy(struct eth_bloom *dest, const struct eth_bloom *src) {
  size_t i;

  if (dest == NULL || src == NULL)
    return -1;

  for (i = 0; i < BITSETS; i++)
    dest->bitsets[i] = src->bitsets[i];

  return 1;
}

int eth_bloom_and(const struct eth_bloom *lhs,
                  const struct eth_bloom *rhs) {
  struct eth_bloom tmpbloom;
  size_t i;

  if (lhs == NULL || rhs == NULL)
    return -1;

  if (eth_bloom_init(&tmpbloom) != 1)
    return -1;

  if (eth_bloom_copy(&tmpbloom, lhs) != 1)
    return -1;

  if (eth_bloom_or(&tmpbloom, rhs) != 1)
    return -1;

  for (i = 0; i < BITSETS; i++)
    if (lhs->bitsets[i] != tmpbloom.bitsets[i])
      return 0;

  return 1;
}

int eth_bloom_or(struct eth_bloom *lhs,
                 const struct eth_bloom *rhs) {
  size_t i;

  if (lhs == NULL || rhs == NULL)
    return -1;

  for (i = 0; i < BITSETS; i++)
    lhs->bitsets[i] |= rhs->bitsets[i];

  return 1;
}
