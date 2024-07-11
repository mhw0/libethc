#include "test.h"
#include <tap.h>
#include <string.h>
#include <ethc/rlp.h>

void test_eth_rlp_uint8(void) {
  struct eth_rlp rlp0, rlp1;
  uint8_t d0=0xff, d1=0x04, d2=0x00, d3, d4;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d1) == 1);
    ok(eth_rlp_uint8(&rlp0, &d2) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 10);
  is(hex, "c481ff0480");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);

  ok(eth_rlp_from_hex(&rlp1, "c381ac08", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint8(&rlp1, &d3) == 1);
    ok(eth_rlp_uint8(&rlp1, &d4) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d3 == 0xac);
  ok(d4 == 0x08);
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

void test_eth_rlp_uint(void) {
  struct eth_rlp rlp0, rlp1;
  uint64_t d0=0x9c797e11, d1 = 0;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 12);
  is(hex, "c5849c797e11");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);

  ok(eth_rlp_from_hex(&rlp1, "0xc38101", -1) == 1);
  ok(eth_rlp_array(&rlp1) == 1);
    ok(eth_rlp_uint(&rlp1, &d1) == 1);
  ok(eth_rlp_array_end(&rlp1) == 1);
  ok(eth_rlp_free(&rlp1) == 1);
  ok(d1 == 0x01);
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

void test_eth_rlp_to_hex(void) {
  struct eth_rlp rlp0;
  uint8_t d0 = 0xff;
  char *hex;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_hex(&hex, &rlp0) == 6);
  is(hex, "c281ff");
  ok(eth_rlp_free(&rlp0) == 1);
  free(hex);
}

void test_eth_rlp_to_bytes(void) {
  struct eth_rlp rlp0;
  uint8_t d0 = 0xac, *bout, bytes[3] = {0xc2, 0x81, 0xac};
  size_t boutlen;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == 1);

  ok(eth_rlp_array(&rlp0) == 1);
    ok(eth_rlp_uint8(&rlp0, &d0) == 1);
  ok(eth_rlp_array_end(&rlp0) == 1);

  ok(eth_rlp_to_bytes(&bout, &boutlen, &rlp0) == 1);
  ok(eth_rlp_free(&rlp0) == 1);
  cmp_mem(bout, bytes, boutlen);
  ok(boutlen == 3);
}

void test_eth_rlp_decode_eip1559_tx(void) {
  // https://sepolia.etherscan.io/tx/0xd116c294d3eb46d5db8602ff38f0aae414fa7950747e20cdefe5982b7bd567f9
  // This tx is EIP-1559 format, and the Txtype prefix `0x02` has been stripped
  char *hex =
      "f8d583aa36a78180843b9aca00851bf08eb000835b8d80942a21bf63839e571c2fc819"
      "373296e0b50b4d6ad480b8648340f54900000000000000000000000066c444632ae2840a"
      "a6cd3daa435f359f547ef9d30000000000000000000000005683c35fbbe9811340fba97f"
      "aea8ef5c33f28be000000000000000000000000000000000000000000000000000000000"
      "0016e360c001a099c06b4f79b805ae7be2dcd21191e470362c9d66b7cfea90b185015893"
      "a1477ea03c16ce20c94c5ee0598154007a67fae010769fe3db29a1e40ac9532a91835a0"
      "c";

  // tx field
  char *abi_hex;
  char *to_addr;
  char *value;
  char *max_priority_fee_per_gas;
  char *max_fee_per_gas;
  char *gas_limit;
  char *chain_id_hex;
  uint64_t nonce = 0;
  uint8_t v = 0;
  char *r;
  char *s;
  size_t rlp0len, rlp1len, siglen = 32;

  // decode tx
  struct eth_rlp rlp;
  ok(eth_rlp_from_hex(&rlp, hex, -1) == 1);
  ok(eth_rlp_array(&rlp) == 1);
  ok(eth_rlp_hex(&rlp, &chain_id_hex, NULL) == 1);
  ok(eth_rlp_uint(&rlp, &nonce));
  ok(eth_rlp_hex(&rlp, &max_priority_fee_per_gas, NULL));
  ok(eth_rlp_hex(&rlp, &max_fee_per_gas, NULL));
  ok(eth_rlp_hex(&rlp, &gas_limit, NULL));
  ok(eth_rlp_address(&rlp, &to_addr));
  ok(eth_rlp_hex(&rlp, &value, NULL));
  ok(eth_rlp_hex(&rlp, &abi_hex, NULL));
  ok(eth_rlp_array(&rlp));  // access list
  ok(eth_rlp_array_end(&rlp));
  ok(eth_rlp_uint8(&rlp, &v));
  ok(eth_rlp_hex(&rlp, &r, NULL));
  ok(eth_rlp_hex(&rlp, &s, NULL));
  ok(eth_rlp_array_end(&rlp));

  // check
  is(chain_id_hex, "aa36a7");
  ok(nonce == 0x80);
  is(max_priority_fee_per_gas, "3b9aca00");
  is(max_fee_per_gas, "1bf08eb000");
  is(gas_limit, "5b8d80");
  is(to_addr, "2a21bf63839e571c2fc819373296e0b50b4d6ad4");
  is(value, "0");
  is(abi_hex,
     "8340f54900000000000000000000000066c444632ae2840aa6cd3daa435f359f547ef9d30"
     "000000000000000000000005683c35fbbe9811340fba97faea8ef5c33f28be00000000000"
     "00000000000000000000000000000000000000000000000016e360");
  ok(v == 1);
  is(r, "99c06b4f79b805ae7be2dcd21191e470362c9d66b7cfea90b185015893a1477e");
  is(s, "3c16ce20c94c5ee0598154007a67fae010769fe3db29a1e40ac9532a91835a0c");
}
