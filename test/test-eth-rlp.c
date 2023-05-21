#include "test.h"
#include <tap.h>
#include <string.h>
#include <ethc/rlp.h>

void test_eth_rlp_uint8(void) {
  struct eth_rlp rlp0, rlp1;
  uint8_t d0=0xff, d1=0x04, d2, d3;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d1) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 8);
  is(hex, "c381ff04");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);


  ok(eth_rlp_from_hex(&rlp1, "c381ac08", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint8(&rlp1, &d2) == 1);
    ok(eth_rlp_uint8(&rlp1, &d3) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d2 == 0xac);
  ok(d3 == 0x08);
}

void test_eth_rlp_uint16(void) {
  struct eth_rlp rlp0, rlp1;
  uint16_t d0=0xb60a, d1;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint16(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 8);
  is(hex, "c382b60a");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);


  ok(eth_rlp_from_hex(&rlp1, "0xc382265d", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint16(&rlp1, &d1) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d1 == 0x265d);
}

void test_eth_rlp_uint32(void) {
  struct eth_rlp rlp0, rlp1;
  uint32_t d0=0x62d07b34, d1;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint32(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 12);
  is(hex, "c58462d07b34");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);


  ok(eth_rlp_from_hex(&rlp1, "0xc5842a6ba6af", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint32(&rlp1, &d1) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d1 == 0x2a6ba6af);
}

void test_eth_rlp_uint64(void) {
  struct eth_rlp rlp0, rlp1;
  uint64_t d0=0xbba4fcad9c797e11, d1;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint64(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 20);
  is(hex, "c988bba4fcad9c797e11");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);


  ok(eth_rlp_from_hex(&rlp1, "0xc98879d27c6d2d1fb824", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint64(&rlp1, &d1) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d1 == 0x79d27c6d2d1fb824);
}

void test_eth_rlp_bytes(void) {
  struct eth_rlp rlp0, rlp1;
  uint8_t d0[] = {0x8f, 0x70}, *b0 = d0, *b1,
          b2[] = {0xd2, 0x25, 0xc1};
  size_t b0len = 2, b1len;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_bytes(&rlp0, &b0, &b0len) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 8);
  ok(eth_rlp_free(&rlp0) == 1);
  is(hex, "c3828f70");
  free(hex);


  ok(eth_rlp_from_hex(&rlp1, "0xc483d225c1", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_bytes(&rlp1, &b1, &b1len) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);

  cmp_mem(b2, b1, b1len);
  ok(b1len == 3);
  free(b1);
}

void test_eth_rlp_hex(void) {
  struct eth_rlp rlp0, rlp1;
  char *hex0 = "0x78d80a80ec11105a", *hex1;
  int hex1len;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_hex(&rlp0, &hex0, NULL) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex0, &rlp0) == 20);
  ok(eth_rlp_free(&rlp0) == 1);
  is(hex0, "c98878d80a80ec11105a");
  free(hex0);


  ok(eth_rlp_from_hex(&rlp1, "0xc5849a23483a", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_hex(&rlp1, &hex1, &hex1len) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);

  is(hex1, "9a23483a");
  ok(hex1len == 8);
  free(hex1);
}

void test_eth_rlp_address(void) {
  struct eth_rlp rlp0, rlp1;
  char *addr0 = "0x86C4dDdd08F8153E50247eaB59e500c043F99BfF",
       *addr1, *hex0;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);
  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_address(&rlp0, &addr0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex0, &rlp0) == 44);
  ok(eth_rlp_free(&rlp0) == 1);
  is(hex0, "d59486c4dddd08f8153e50247eab59e500c043f99bff");
  free(hex0);


  ok(eth_rlp_from_hex(&rlp1, "0xd59486c4dddd08f8153e50247eab59e500c043f99bff", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_address(&rlp1, &addr1) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);

  is(addr1, "86c4dddd08f8153e50247eab59e500c043f99bff");
  free(addr1);
}
