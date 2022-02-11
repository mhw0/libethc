#include <KeccakHash.h>
#include <ethc/keccak256.h>

#define KECCAK256_RATE 1088
#define KECCAK256_CAPACITY 512
#define KECCAK256_HASHBITLEN 256
#define KECCAK256_DELIMITED_SUFFIX 0x1

int eth_keccak256(uint8_t *dest, const uint8_t *bytes, size_t len) {
  Keccak_HashInstance instance;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  if (Keccak_HashInitialize(&instance, KECCAK256_RATE, KECCAK256_CAPACITY,
                            KECCAK256_HASHBITLEN,
                            KECCAK256_DELIMITED_SUFFIX) == KECCAK_FAIL)
    return ETHC_FAIL;

  if (Keccak_HashUpdate(&instance, bytes, len * 8) == KECCAK_FAIL)
    return ETHC_FAIL;

  return Keccak_HashFinal(&instance, dest) == KECCAK_SUCCESS;
}
