#include <assert.h>
#include <ethc/keccak256.h>

HashReturn eth_keccak256(const char* input, unsigned char* output) {
  assert(input != NULL);
  Keccak_HashInstance instance;

  if(Keccak_HashInitialize(&instance, KECCAK_RATE, KECCAK_CAPACITY, KECCAK_HASHBITLEN, KECCAK_DELIMITED_SUFFIX) == KECCAK_FAIL)
    return KECCAK_FAIL;

  if(Keccak_HashUpdate(&instance, input, strlen(input) * 8) == KECCAK_FAIL)
    return KECCAK_FAIL;

  return Keccak_HashFinal(&instance, output);
}
