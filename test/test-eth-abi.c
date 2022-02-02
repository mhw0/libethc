#include "test.h"
#include <ethc/abi.h>
#include <tap.h>

void test_eth_abi_encode_bool(void) {
  const char *exp1 =
      "0000000000000000000000000000000000000000000000000000000000000001";
  char out1[64 + 1];

  ok(eth_abi_encode_bool(out1, 1) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_address(void) {
  char *in1 = "0xfab039650e3b529dae81a5ceec00097da1a2b451";
  char out1[64 + 1];
  const char *exp1 =
      "000000000000000000000000fab039650e3b529dae81a5ceec00097da1a2b451";

  ok(eth_abi_encode_address(out1, in1) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_int(void) {
  char out1[64 + 1];
  const char *exp1 =
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff80";

  ok(eth_abi_encode_int(out1, "-128", 8) == 1);
  is(out1, exp1);

  char out2[64 + 1];

  ok(eth_abi_encode_int(out2, "-129", 8) == 0);

  char out3[64 + 1];
  const char *exp3 =
      "000000000000000000000000000000000000000000000000000000000000007f";

  ok(eth_abi_encode_int(out3, "127", 8) == 1);
  is(out3, exp3);

  char out4[64 + 1];

  ok(eth_abi_encode_int(out3, "128", 8) == 0);
}

void test_eth_abi_encode_uint(void) {
  const char *in1 = "255";
  char out1[64 + 1];
  const char *exp1 =
      "00000000000000000000000000000000000000000000000000000000000000ff";

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

void test_eth_abi_encode_bytes(void) {
  const uint8_t in1[] = {0xaa, 0xbb, 0xcc};
  char out1[64 + 1];
  const char *exp1 =
      "aabbcc0000000000000000000000000000000000000000000000000000000000";

  ok(eth_abi_encode_bytes(out1, in1, 3) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_bytesd(void) {
  char *out1;
  const uint8_t in1[] = {
      0xc5, 0xd2, 0x46, 0x01, 0x86, 0xf7, 0x23, 0x3c, 0x92, 0x7e, 0x7d,
      0xb2, 0xdc, 0xc7, 0x03, 0xc0, 0xe5, 0x00, 0xb6, 0x53, 0xca, 0x82,
      0x27, 0x3b, 0x7b, 0xfa, 0xd8, 0x04, 0x5d, 0x85, 0xa4, 0x70,

      0xc5, 0xd2, 0x46, 0x01, 0x86, 0xf7, 0x23, 0x3c, 0x92, 0x7e, 0x7d,
      0xb2, 0xdc, 0xc7, 0x03, 0xc0, 0xe5, 0x00, 0xb6, 0x53, 0xca, 0x82,
      0x27, 0x3b, 0x7b, 0xfa, 0xd8, 0x04, 0x5d,
  };
  const char *exp1 =
      "000000000000000000000000000000000000000000000000000000000000003dc5d24601"
      "86f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470c5d2460186f7233c"
      "927e7db2dcc703c0e500b653ca82273b7bfad8045d000000";

  out1 = eth_abi_encode_bytesd(in1, 61);
  is(out1, exp1);
  free(out1);

  char *out2;
  out2 = eth_abi_encode_bytesd(NULL, 1);
  is(out2, NULL);
}

void test_eth_abi_encode_stringd(void) {
  char *in1 = "libèthc", *out1;
  const char *exp1 = "00000000000000000000000000000000000000000000000000000000000000086c6962c3a8746863000000000000000000000000000000000000000000000000";

  out1 = eth_abi_encode_stringd(in1, -1);
  is(out1, exp1);
}

void test_eth_abi_encode_func(void) {
  const char *in1 = "approve(address,uint256)";
  char out1[8 + 1];
  const char *exp1 = "095ea7b3";

  ok(eth_abi_encode_func(out1, in1, -1) == 1);
  is(out1, exp1);
}

void test_eth_abi_encode_event(void) {
  const char *in1 = "approval(address,address,uint256)";
  char out1[64 + 1];
  const char *exp1 =
      "5c52a5f2b86fd16be577188b5a83ef1165faddc00b137b10285f16162e17792a";

  ok(eth_abi_encode_event(out1, in1, -1) == 1);
  is(out1, exp1);
}
