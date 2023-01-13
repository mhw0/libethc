#include "test.h"
#include <ethc/hex.h>
#include <string.h>
#include <tap.h>

void test_eth_is_hex(void) {
  const char *in1 = "";
  ok(eth_is_hex(in1, strlen(in1)) == -1);

  const char *in2 = "0xffff00";
  ok(eth_is_hex(in2, -1) == 1);

  const char *in3 = "0xFFFF00";
  ok(eth_is_hex(in3, -1) == 1);

  const char *in4 = "0XFFFF00";
  ok(eth_is_hex(in4, -1) == 1);

  const char *in5 = "ffff00";
  ok(eth_is_hex(in5, -1) == 1);

  const char *in6 = "FFFF00";
  ok(eth_is_hex(in6, -1) == 1);

  const char *in7 = "0xffff00nonhex";
  ok(eth_is_hex(in7, 8) == 1);

  const char *in8 = "0x";
  ok(eth_is_hex(in8, -1) == 0);
}

void test_eth_hex_pad_left(void) {
  const char *in1 = "ff";
  char out1[5];
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
  char out1[5];
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
  const uint8_t *in1 = NULL;
  char *out1 = NULL;
  ok(eth_hex_from_bytes(&out1, in1, 1) == -1);

  const uint8_t in2[] = {0xff, 0xaa, 0x03};
  char *out2;
  char *exp2 = "ffaa03";
  ok(eth_hex_from_bytes(&out2, in2, 3) == 6);
  cmp_mem(out2, exp2, 7);
  free(out2);
}

void test_eth_hex_to_bytes(void) {
  const char *in1 = NULL;
  ok(eth_hex_to_bytes(NULL, NULL, 1) == -1);


  const char *in2 = "0xffcfab";
  uint8_t *out2;
  const uint8_t exp2[] = {0xff, 0xcf, 0xab};

  ok(eth_hex_to_bytes(&out2, in2, 8) == 3);
  cmp_mem(out2, exp2, 3);
  free(out2);


  const char *in3 = "7c40a04d";
  uint8_t *out3;
  const uint8_t exp3[] = {0x7c, 0x40, 0xa0, 0x4d};

  ok(eth_hex_to_bytes(&out3, in3, 8) == 4);
  cmp_mem(out3, exp3, 4);
  free(out3);


  const char *in4= "d2160780";
  uint8_t *out4;
  const uint8_t exp4[] = {0xd2, 0x16, 0x07, 0x80};

  ok(eth_hex_to_bytes(&out4, in4, -1) == 4);
  cmp_mem(out4, exp4, 4);
  free(out4);


  const char *in5 = "143b500dnonhex";
  uint8_t *out5;
  const uint8_t exp5[] = {0x14, 0x3b, 0x50, 0x0d};

  ok(eth_hex_to_bytes(&out5, in5, 8) == 4);
  cmp_mem(out5, exp5, 4);
  free(out5);


  const char *in6 = "49ac44a";
  uint8_t *out6;
  const uint8_t exp6[] = {0x04, 0x9a, 0xc4, 0x4a};

  ok(eth_hex_to_bytes(&out6, in6, -1) == 4);
  cmp_mem(out6, exp6, 4);
  free(out6);
}
