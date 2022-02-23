#include <KeccakHash.h>
#include <ethc/keccak256.h>
#include <stdlib.h>
#include <gmp.h>

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

int eth_keccak256p(uint8_t *dest, const uint8_t *bytes, size_t len) {
  int size, r;
  char *sig, *tmp;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  size = gmp_asprintf(&sig, "\x19" "Ethereum Signed Message:\n%llu", len);
  tmp = realloc(sig, size + len);
  if(tmp == NULL) {
    free(sig);
    return ETHC_FAIL;
  }
  sig = tmp;

  strncpy(sig + size, (char*)bytes, len);
  r = eth_keccak256(dest, (uint8_t*)sig, size + len);
  free(sig);
  return r;
}
