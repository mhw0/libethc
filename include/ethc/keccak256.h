#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H
#include <KeccakHash.h>

typedef HashReturn KeccakHashReturn;

KeccakHashReturn eth_keccak256(const char *input, unsigned char *output);

#endif
