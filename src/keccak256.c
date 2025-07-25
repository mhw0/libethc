#include <KeccakHash.h>
#include <ethc/keccak256.h>
#include <stdlib.h>
#include <stdio.h>

#define KECCAK256_RATE 1088
#define KECCAK256_CAPACITY 512
#define KECCAK256_HASHBITLEN 256
#define KECCAK256_DELIMITED_SUFFIX 0x1

int eth_keccak256(uint8_t *dest, const uint8_t *bytes, size_t len) {
  Keccak_HashInstance instance;

  if (dest == NULL || bytes == NULL)
    return -1;

  if (Keccak_HashInitialize(&instance, KECCAK256_RATE, KECCAK256_CAPACITY,
                            KECCAK256_HASHBITLEN,
                            KECCAK256_DELIMITED_SUFFIX) == KECCAK_FAIL)
    return -1;

  if (Keccak_HashUpdate(&instance, bytes, len * 8) == KECCAK_FAIL)
    return -1;

  return Keccak_HashFinal(&instance, dest) == KECCAK_SUCCESS ? 1 : -1;
}

int eth_keccak256p(uint8_t *dest, const uint8_t *bytes, size_t len) {
  char sig[len + 32];
  int size;

  if (dest == NULL || bytes == NULL)
    return -1;

  size = snprintf(sig, sizeof(sig), "\x19" "Ethereum Signed Message:\n%zu", len);

  memcpy(sig + size, (char*)bytes, len);
  size += len;

  return eth_keccak256(dest, (uint8_t*)sig, size);
}
