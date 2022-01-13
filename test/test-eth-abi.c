#include "test.h"
#include <ethc/abi.h>
#include <tap.h>

void test_eth_abi_encode_bool(void) {
  const char *exp1 = "0000000000000000000000000000000000000000000000000000000000000001";
  char out1[64 + 1];
  ok(eth_abi_encode_bool(out1, 1));
  is(out1, exp1);
}

void test_eth_abi_encode_address(void) {
  char *in1 = "0xfab039650e3b529dae81a5ceec00097da1a2b451";
  char out1[64 + 1];
  const char *exp1 = "000000000000000000000000fab039650e3b529dae81a5ceec00097da1a2b451";
  ok(eth_abi_encode_address(out1, in1));
  is(out1, exp1);
}
