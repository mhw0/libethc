#include "test.h"
#include <ethc/hex.h>
#include <string.h>
#include <tap.h>

void test_eth_is_hexstr(void) {
  const char *in0 = "";
  ok(eth_is_hexstr(in0, strlen(in0), 0) == 0);

  const char *in1 = "0xffff00";
  ok(eth_is_hexstr(in1, -1, 1) == 1);

  const char *in2 = "0xFFFF00";
  ok(eth_is_hexstr(in2, -1, 1) == 1);

  const char *in3 = "0XFFFF00";
  ok(eth_is_hexstr(in3, -1, 1) == 1);

  const char *in4 = "ffff00";
  ok(eth_is_hexstr(in4, -1, 1) == 0);

  const char *in5 = "FFFF00";
  ok(eth_is_hexstr(in5, -1, 0) == 1);

  const char *in6 = "0xffff00nonhex";
  ok(eth_is_hexstr(in6, 8, 1) == 1);
}

void test_eth_hexstr_pad_left(void) {
  const char *in0 = "0xff";
  is(eth_hexstr_pad_left(in0, strlen(in0), 4), "0x00ff");

  const char *in1 = "ff";
  is(eth_hexstr_pad_left(in1, strlen(in1), 4), "00ff");
}

void test_eth_hexstr_pad_right(void) {
  const char *in0 = "0xff";
  is(eth_hexstr_pad_right(in0, strlen(in0), 4), "0xff00");

  const char *in1 = "ff";
  is(eth_hexstr_pad_right(in1, strlen(in1), 4), "ff00");
}
