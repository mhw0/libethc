#include "test.h"
#include <ethc/keccak256.h>
#include <tap.h>

void test_eth_keccak256(void) {
  const uint8_t in1[] = {};
  uint8_t out1[32];
  const char exp1[] = {0xc5, 0xd2, 0x46, 0x01, 0x86, 0xf7, 0x23, 0x3c,
                       0x92, 0x7e, 0x7d, 0xb2, 0xdc, 0xc7, 0x03, 0xc0,
                       0xe5, 0x00, 0xb6, 0x53, 0xca, 0x82, 0x27, 0x3b,
                       0x7b, 0xfa, 0xd8, 0x04, 0x5d, 0x85, 0xa4, 0x70};

  ok(eth_keccak256(out1, in1, 0) == 1);
  cmp_mem(out1, exp1, 32);

  const uint8_t in2[4] = {0x74, 0x65, 0x73, 0x74};
  uint8_t out2[32];
  const char exp2[] = {0x9c, 0x22, 0xff, 0x5f, 0x21, 0xf0, 0xb8, 0x1b,
                       0x11, 0x3e, 0x63, 0xf7, 0xdb, 0x6d, 0xa9, 0x4f,
                       0xed, 0xef, 0x11, 0xb2, 0x11, 0x9b, 0x40, 0x88,
                       0xb8, 0x96, 0x64, 0xfb, 0x9a, 0x3c, 0xb6, 0x58};

  ok(eth_keccak256(out2, in2, 4) == 1);
  cmp_mem(out2, exp2, 32);
}

void test_eth_keccak256p(void) {
  const uint8_t in1[4] = { 0x74, 0x65, 0x73, 0x74 };
  uint8_t out1[32];
  const uint8_t exp1[32] = {
    0x4a, 0x5c, 0x5d, 0x45, 0x47, 0x21, 0xbb, 0xbb, 0x25, 0x54, 0x0c, 0x33,
    0x17, 0x52, 0x1e, 0x71, 0xc3, 0x73, 0xae, 0x36, 0x45, 0x8f, 0x96, 0x0d,
    0x2a, 0xd4, 0x6e, 0xf0, 0x88, 0x11, 0x0e, 0x95
  };

  ok(eth_keccak256p(out1, in1, 4) == 1);
  cmp_mem(out1, exp1, 32);
}
