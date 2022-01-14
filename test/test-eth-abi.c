#include "test.h"
#include <ethc/abi.h>
#include <tap.h>

void test_eth_abi_encode_bool(void) {
  const char *exp1 = "0000000000000000000000000000000000000000000000000000000000000001";
  char out1[64 + 1];
  ok(eth_abi_encode_bool(out1, 1) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_address(void) {
  char *in1 = "0xfab039650e3b529dae81a5ceec00097da1a2b451";
  char out1[64 + 1];
  const char *exp1 = "000000000000000000000000fab039650e3b529dae81a5ceec00097da1a2b451";
  ok(eth_abi_encode_address(out1, in1) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_uint(void) {
  const char *in1 = "255";
  char out1[64 + 1];
	const char *exp1 = "00000000000000000000000000000000000000000000000000000000000000ff";

  ok(eth_abi_encode_uint(out1, in1, 8) == 1);
  is(out1, exp1);

  const char *in2 = "256";
  char out2[64 + 1];

  ok(eth_abi_encode_uint(out2, in2, 8) == 0);

  const char *in3 = "-255";
  char out3[64 + 1];

  ok(eth_abi_encode_uint(out3, in3, 8) == 0);

  const char *in4 = "255";
  char out4[64 + 1];

  ok(eth_abi_encode_uint(out4, in4, 9) == 0);
}
