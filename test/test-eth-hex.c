#include "test.h"
#include <ethc/hex.h>
#include <string.h>
#include <tap.h>

void test_eth_is_hex(void) {
  const char *in1 = "";
  ok(eth_is_hex(in1, strlen(in1), 0) == 0);

  const char *in2 = "0xffff00";
  ok(eth_is_hex(in2, -1, 1) == 1);

  const char *in3 = "0xFFFF00";
  ok(eth_is_hex(in3, -1, 1) == 1);

  const char *in4 = "0XFFFF00";
  ok(eth_is_hex(in4, -1, 1) == 1);

  const char *in5 = "ffff00";
  ok(eth_is_hex(in5, -1, 1) == 0);

  const char *in6 = "FFFF00";
  ok(eth_is_hex(in6, -1, 0) == 1);

  const char *in7 = "0xffff00nonhex";
  ok(eth_is_hex(in7, 8, 1) == 1);
}

void test_eth_hex_pad_left(void) {
  const char *in1 = "ff";
  char out1[5]; // TODO
  const char *exp1 = "00ff";
  ok(eth_hex_pad_left(out1, in1, -1, 4) == 1);
  is(out1, exp1);

  const char *in2 = "ffnonhex";
	char out2[5];
  const char *exp2 = "00ff";
  ok(eth_hex_pad_left(out2, in2, 2, 4) == 1);
  is(out2, exp2);
}

void test_eth_hex_pad_right(void) {
  const char *in1 = "ff";
  char out1[5]; // TODO
  const char *exp1 = "ff00";
  ok(eth_hex_pad_right(out1, in1, -1, 4) == 1);
  is(out1, exp1);

  const char *in2 = "ffnonhex";
	char out2[5];
  const char *exp2 = "ff00";
  ok(eth_hex_pad_right(out2, in2, 2, 4) == 1);
  is(out2, exp2);
}

void test_eth_hex_from_bytes(void) {
  const uint8_t in1[3] = { 0xff, 0xaa, 0x3 };
  const char *exp1 = "ffaa03";
  char out1[7];

  ok(eth_hex_from_bytes(out1, in1, 3));
  is(out1, exp1);
}

void test_eth_hex_to_bytes(void) {
  const char *in1 = "ffcfab";
  const uint8_t exp1[] = { 0xff, 0xcf, 0xab };
  uint8_t out1[3];

  ok(eth_hex_to_bytes(out1, in1, -1) == ETHC_SUCCESS);
  cmp_mem(out1, exp1, 3);

  const char *in2 = "fff";
  uint8_t out2[2];

  ok(eth_hex_to_bytes(out2, in2, -1) == ETHC_FAIL);
}
