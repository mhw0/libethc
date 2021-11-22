#ifndef ETHC_KECCAK256_H
#define ETHC_KECCAK256_H
#include <KeccakHash.h>

#define KECCAK_RATE 1088
#define KECCAK_CAPACITY 512
#define KECCAK_HASHBITLEN 256
#define KECCAK_DELIMITED_SUFFIX 0x1

HashReturn eth_keccak256(const char* input, unsigned char* output);

#endif
