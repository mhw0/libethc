#include "test.h"
#include <ethc/abi.h>
#include <tap.h>

void test_eth_abi_encode_bool(void) {
  const char *exp1 = "0000000000000000000000000000000000000000000000000000000000000001";
  char out1[64 + 1];
  ok(eth_abi_encode_bool(out1, 1));
  is(out1, exp1);
}
