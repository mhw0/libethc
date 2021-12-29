#include <ethc/bloom.h>
#include <ethc/keccak256.h>

int eth_bloom_create(struct eth_bloom *bloom) {
  size_t i;
  if(!bloom)
    return 0;

  for(i = 0; i < 32; i++)
    bloom->bitsets[i] = 0;

  return 1;
}

int eth_bloom_from_bytes(struct eth_bloom *bloom, const uint8_t *bytes, size_t len) {
  uint8_t keccak[32], bitset;
  uint16_t bitpos;
  size_t i;

  if(!bloom || !bytes)
    return 0;

  if(!eth_keccak256(bytes, len, keccak))
    return 0;

  for(i = 0; i < 6; i += 2) {
    bitpos = (((keccak[i] & 0xFF) & 0x3) << 0x8) + (keccak[i + 1] & 0xFF);
    bitset = bitpos / 64;
    bloom->bitsets[bitset] |= (1ULL << (bitpos % 64));
  }

  return 1;
}

int eth_bloom_copy(struct eth_bloom *dest_bloom, const struct eth_bloom *src_bloom) {
  size_t i;

  if(!dest_bloom || !src_bloom)
    return 0;

  for(i = 0; i < 32; i++)
    dest_bloom->bitsets[i] = src_bloom->bitsets[i];

  return 1;
}

int eth_bloom_and(const struct eth_bloom *bloom1, const struct eth_bloom *bloom2) {
  struct eth_bloom copy_bloom;
  size_t i;

  if(!bloom1 || !bloom2)
    return 0;

  eth_bloom_create(&copy_bloom);
  eth_bloom_copy(&copy_bloom, bloom1);
  eth_bloom_or(&copy_bloom, bloom2);

  for(i = 0; i < 32; i++)
    if(bloom1->bitsets[i] != copy_bloom.bitsets[i])
      return 0;

  return 1;
}

int eth_bloom_or(struct eth_bloom *dest_bloom, const struct eth_bloom *src_bloom) {
  size_t i;

  if(!dest_bloom || !src_bloom)
    return 0;

  for(i = 0; i < 32; i++) {
    dest_bloom->bitsets[i] |= src_bloom->bitsets[i];
  }

  return 1;
}
