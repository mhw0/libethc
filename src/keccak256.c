#include <KeccakHash.h>
#include <ethc/keccak256.h>

#define KECCAK256_RATE 1088
#define KECCAK256_CAPACITY 512
#define KECCAK256_HASHBITLEN 256
#define KECCAK256_DELIMITED_SUFFIX 0x1

int eth_keccak256(const uint8_t *data, size_t len, uint8_t *out) {
  if (!data || !out)
    return 0;

  Keccak_HashInstance instance;
  if (Keccak_HashInitialize(&instance, KECCAK256_RATE, KECCAK256_CAPACITY,
                            KECCAK256_HASHBITLEN,
                            KECCAK256_DELIMITED_SUFFIX) == KECCAK_FAIL)
    return 0;

  if (Keccak_HashUpdate(&instance, data, len * 8) == KECCAK_FAIL)
    return 0;

  return Keccak_HashFinal(&instance, out) == KECCAK_SUCCESS;
}
