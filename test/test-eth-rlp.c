#include "test.h"
#include <assert.h>
#include <tap.h>
#include <string.h>
#include <ethc/rlp.h>

void test_eth_rlp_uint8(void) {
  struct eth_rlp rlp0, rlp1;
  uint8_t d0=0xff, d1=0x04, d2=0x00, d3, d4;
  char *hex;

  diag("eth_rlp_uint8()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint8(&rlp0, &d0) == ETH_OK);
    assert(eth_rlp_uint8(&rlp0, &d1) == ETH_OK);
    assert(eth_rlp_uint8(&rlp0, &d2) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);

  is(hex, "c481ff0480", "encode");

  assert(eth_rlp_free(&rlp0) == ETH_OK);

  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "c381ac08", -1) == ETH_OK);
  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_uint8(&rlp1, &d3) == ETH_OK);
    assert(eth_rlp_uint8(&rlp1, &d4) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);
  assert(eth_rlp_free(&rlp1) == ETH_OK);

  ok(d3 == 0xac && d4 == 0x08, "decode");
}

void test_eth_rlp_uint16(void) {
  struct eth_rlp rlp0, rlp1;
  uint16_t d0=0xb60a, d1;
  char *hex;

  diag("eth_rlp_uint16()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint16(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);

  is(hex, "c382b60a", "encode");

  assert(eth_rlp_free(&rlp0) == ETH_OK);

  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "0xc382265d", -1) == ETH_OK);
  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_uint16(&rlp1, &d1) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  ok(d1 == 0x265d, "decode");
}

void test_eth_rlp_uint32(void) {
  struct eth_rlp rlp0, rlp1;
  uint32_t d0=0x62d07b34, d1;
  char *hex;

  diag("eth_rlp_uint32()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint32(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);

  is(hex, "c58462d07b34", "encode");

  assert(eth_rlp_free(&rlp0) == ETH_OK);
  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "0xc5842a6ba6af", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_uint32(&rlp1, &d1) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  ok(d1 == 0x2a6ba6af, "decode");
}

void test_eth_rlp_uint64(void) {
  struct eth_rlp rlp0, rlp1;
  uint64_t d0=0xbba4fcad9c797e11, d1;
  char *hex;

  diag("eth_rlp_uint64()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint64(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);

  is(hex, "c988bba4fcad9c797e11", "encode");

  assert(eth_rlp_free(&rlp0) == ETH_OK);

  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "0xc98879d27c6d2d1fb824", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_uint64(&rlp1, &d1) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);
  assert(eth_rlp_free(&rlp1) == ETH_OK);

  ok(d1 == 0x79d27c6d2d1fb824, "decode");
}

void test_eth_rlp_uint(void) {
  struct eth_rlp rlp0, rlp1;
  uint64_t d0=0x9c797e11, d1 = 0;
  char *hex;

  diag("eth_rlp_uint()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);
  is(hex, "c5849c797e11", "encode");
  assert(eth_rlp_free(&rlp0) == ETH_OK);

  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "0xc38101", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_uint(&rlp1, &d1) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  ok(d1 == 0x01, "decode");
}

void test_eth_rlp_bytes(void) {
  struct eth_rlp rlp0, rlp1;
  uint8_t d0[] = {0x8f, 0x70}, *b0 = d0, *b1,
          b2[] = {0xd2, 0x25, 0xc1};
  size_t b0len = 2, b1len;
  char *hex;

  diag("eth_rlp_bytes()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_bytes(&rlp0, &b0, &b0len) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);
  assert(eth_rlp_free(&rlp0) == ETH_OK);

  is(hex, "c3828f70", "encode");

  free(hex);

  assert(eth_rlp_from_hex(&rlp1, "0xc483d225c1", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_bytes(&rlp1, &b1, &b1len) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  cmp_mem(b2, b1, b1len, "decode");

  free(b1);
}

void test_eth_rlp_hex(void) {
  struct eth_rlp rlp0, rlp1;
  char *hex0 = "0x78d80a80ec11105a", *hex1;
  int hex1len;

  diag("eth_rlp_hex()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_hex(&rlp0, &hex0, NULL) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex0, &rlp0) == ETH_OK);
  assert(eth_rlp_free(&rlp0) == ETH_OK);

  is(hex0, "c98878d80a80ec11105a", "encode");

  free(hex0);

  assert(eth_rlp_from_hex(&rlp1, "0xc5849a23483a", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_hex(&rlp1, &hex1, &hex1len) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  is(hex1, "9a23483a", "decode");
  ok(hex1len == 8, "decoded data length");

  free(hex1);
}

void test_eth_rlp_address(void) {
  struct eth_rlp rlp0, rlp1, rlp2, rlp3;
  char *addr0 = "0x86C4dDdd08F8153E50247eaB59e500c043F99BfF",
       *addr1, *addr2 = "", *addr3, *hex0, *hex1;

  diag("eth_rlp_address()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_address(&rlp0, &addr0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex0, &rlp0) == ETH_OK);
  assert(eth_rlp_free(&rlp0) == ETH_OK);

  is(hex0, "d59486c4dddd08f8153e50247eab59e500c043f99bff", "encode");
  free(hex0);


  assert(eth_rlp_from_hex(&rlp1, "0xd59486c4dddd08f8153e50247eab59e500c043f99bff", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp1) == ETH_OK);
    assert(eth_rlp_address(&rlp1, &addr1) == ETH_OK);
  assert(eth_rlp_array_end(&rlp1) == ETH_OK);

  assert(eth_rlp_free(&rlp1) == ETH_OK);

  is(addr1, "86c4dddd08f8153e50247eab59e500c043f99bff", "decode");
  free(addr1);


  // test case for empty address
  assert(eth_rlp_init(&rlp2, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp2) == ETH_OK);
    assert(eth_rlp_address(&rlp2, &addr2) == ETH_OK);
  assert(eth_rlp_array_end(&rlp2) == ETH_OK);

  assert(eth_rlp_to_hex(&hex1, &rlp2) == ETH_OK);
  assert(eth_rlp_free(&rlp2) == ETH_OK);

  is(hex1, "c180", "encode empty address");
  free(hex1);

  assert(eth_rlp_from_hex(&rlp3, "c180", -1) == ETH_OK);

  assert(eth_rlp_array(&rlp3) == ETH_OK);
    assert(eth_rlp_address(&rlp3, &addr3) == ETH_OK);
  assert(eth_rlp_array_end(&rlp3) == ETH_OK);

  assert(eth_rlp_free(&rlp3) == ETH_OK);

  is(addr3, "", "decode empty address");
  free(addr3);
}

void test_eth_rlp_to_hex(void) {
  struct eth_rlp rlp0;
  uint8_t d0 = 0xff;
  char *hex;

  diag("eth_rlp_to_hex()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint8(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_hex(&hex, &rlp0) == ETH_OK);
  is(hex, "c281ff");
  assert(eth_rlp_free(&rlp0) == ETH_OK);
  free(hex);
}

void test_eth_rlp_to_bytes(void) {
  struct eth_rlp rlp0;
  uint8_t d0 = 0xac, *bout, bytes[3] = {0xc2, 0x81, 0xac};
  size_t boutlen;

  diag("eth_rlp_to_bytes()");

  assert(eth_rlp_init(&rlp0, ETH_RLP_ENCODE) == ETH_OK);

  assert(eth_rlp_array(&rlp0) == ETH_OK);
    assert(eth_rlp_uint8(&rlp0, &d0) == ETH_OK);
  assert(eth_rlp_array_end(&rlp0) == ETH_OK);

  assert(eth_rlp_to_bytes(&bout, &boutlen, &rlp0) == ETH_OK);
  assert(eth_rlp_free(&rlp0) == ETH_OK);

  cmp_mem(bout, bytes, boutlen);
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
  struct eth_rlp rlp;

  diag("decode EIP1559");

  // decode tx
  assert(eth_rlp_from_hex(&rlp, hex, -1) == ETH_OK);

  assert(eth_rlp_array(&rlp) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &chain_id_hex, NULL) == ETH_OK);
    assert(eth_rlp_uint(&rlp, &nonce) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &max_priority_fee_per_gas, NULL) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &max_fee_per_gas, NULL) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &gas_limit, NULL) == ETH_OK);
    assert(eth_rlp_address(&rlp, &to_addr) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &value, NULL) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &abi_hex, NULL) == ETH_OK);
      assert(eth_rlp_array(&rlp) == ETH_OK);  // access list
      assert(eth_rlp_array_end(&rlp) == ETH_OK);
    assert(eth_rlp_uint8(&rlp, &v) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &r, NULL) == ETH_OK);
    assert(eth_rlp_hex(&rlp, &s, NULL) == ETH_OK);
  assert(eth_rlp_array_end(&rlp) == ETH_OK);

  // check
  is(chain_id_hex, "aa36a7", "chain id");
  ok(nonce == 0x80, "nonce");
  is(max_priority_fee_per_gas, "3b9aca00", "max priority fee per gas");
  is(max_fee_per_gas, "1bf08eb000", "max fee per gas");
  is(gas_limit, "5b8d80", "gas limit");
  is(to_addr, "2a21bf63839e571c2fc819373296e0b50b4d6ad4", "to addr");
  is(value, "0", "value");
  is(abi_hex,
     "8340f54900000000000000000000000066c444632ae2840aa6cd3daa435f359f547ef9d30"
     "000000000000000000000005683c35fbbe9811340fba97faea8ef5c33f28be00000000000"
     "00000000000000000000000000000000000000000000000016e360", "abi");
  ok(v == 1, "v");
  is(r, "99c06b4f79b805ae7be2dcd21191e470362c9d66b7cfea90b185015893a1477e", "r");
  is(s, "3c16ce20c94c5ee0598154007a67fae010769fe3db29a1e40ac9532a91835a0c", "s");
}
