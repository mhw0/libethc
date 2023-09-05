#include "test.h"
#include <ethc/abi.h>
#include <tap.h>

void test_eth_abi_bool(void) {
  struct eth_abi abi0, abi1;
  uint8_t b0=1, b1=0, b2, b3;
  char *hex;
  size_t hexlen;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_bool(&abi0, &b0) == 1);
  ok(eth_abi_bool(&abi0, &b1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);

  ok(hexlen == 128);

  is(hex, "0000000000000000000000000000000000000000000000000000000000000001" 
          "0000000000000000000000000000000000000000000000000000000000000000");
  ok(eth_abi_free(&abi0) == 1);
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000001", -1) == 1);
  ok(eth_abi_bool(&abi1, &b2) == 1);
  ok(eth_abi_bool(&abi1, &b3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(b2 == 0);
  ok(b3 == 1);
}

void test_eth_abi_uint8(void) {
  struct eth_abi abi0, abi1;
  uint8_t d0=0x32, d1=0xf2, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_uint8(&abi0, &d0) == 1);
  ok(eth_abi_uint8(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "0000000000000000000000000000000000000000000000000000000000000032" 
          "00000000000000000000000000000000000000000000000000000000000000f2");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "00000000000000000000000000000000000000000000000000000000000000ff"
        "00000000000000000000000000000000000000000000000000000000000000ac", -1) == 1);
  ok(eth_abi_uint8(&abi1, &d2) == 1);
  ok(eth_abi_uint8(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == 0xff);
  ok(d3 == 0xac);
}

void test_eth_abi_uint16(void) {
  struct eth_abi abi0, abi1;
  uint16_t d0=0x6109, d1=0x3d0c, d2, d3;
  size_t hexlen;
  char *hex, *method;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_uint16(&abi0, &d0) == 1);
  ok(eth_abi_uint16(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "0000000000000000000000000000000000000000000000000000000000006109" 
          "0000000000000000000000000000000000000000000000000000000000003d0c");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000000000000000e0cb"
        "000000000000000000000000000000000000000000000000000000000000f38f", -1) == 1);
  ok(eth_abi_uint16(&abi1, &d2) == 1);
  ok(eth_abi_uint16(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == 0xe0cb);
  ok(d3 == 0xf38f);
}

void test_eth_abi_uint32(void) {
  struct eth_abi abi0, abi1;
  uint32_t d0=0xef20f08f, d1=0x35d6887c, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_uint32(&abi0, &d0) == 1);
  ok(eth_abi_uint32(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "00000000000000000000000000000000000000000000000000000000ef20f08f" 
          "0000000000000000000000000000000000000000000000000000000035d6887c");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000000000007114ee5e"
        "00000000000000000000000000000000000000000000000000000000e704d479", -1) == 1);
  ok(eth_abi_uint32(&abi1, &d2) == 1);
  ok(eth_abi_uint32(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == 0x7114ee5e);
  ok(d3 == 0xe704d479);
}

void test_eth_abi_uint64(void) {
  struct eth_abi abi0, abi1;
  uint64_t d0=0xab16c0a9773532a4, d1=0xfb4ddbc3bb9fd42f, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_uint64(&abi0, &d0) == 1);
  ok(eth_abi_uint64(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "000000000000000000000000000000000000000000000000ab16c0a9773532a4" 
          "000000000000000000000000000000000000000000000000fb4ddbc3bb9fd42f");
  free(hex);
  
  ok(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000a61611e899af529c"
        "0000000000000000000000000000000000000000000000007f6d3cbc578d68b0", -1) == 1);
  ok(eth_abi_uint64(&abi1, &d2) == 1);
  ok(eth_abi_uint64(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == 0xa61611e899af529c);
  ok(d3 == 0x7f6d3cbc578d68b0);
}

void test_eth_abi_int8(void) {
  struct eth_abi abi0, abi1;
  int8_t d0=-0x04, d1=0x64, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_int8(&abi0, &d0) == 1);
  ok(eth_abi_int8(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc" 
          "0000000000000000000000000000000000000000000000000000000000000064");
  free(hex);
  
  ok(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff85"
        "0000000000000000000000000000000000000000000000000000000000000064", -1) == 1);
  ok(eth_abi_int8(&abi1, &d2) == 1);
  ok(eth_abi_int8(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == -0x7b);
  ok(d3 == 0x64);
}

void test_eth_abi_int16(void) {
  struct eth_abi abi0, abi1;
  int16_t d0=-0x7830, d1=0xf03, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_int16(&abi0, &d0) == 1);
  ok(eth_abi_int16(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff87d0" 
          "0000000000000000000000000000000000000000000000000000000000000f03");
  free(hex);
  
  ok(eth_abi_from_hex(&abi1,
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc00"
        "0000000000000000000000000000000000000000000000000000000000000400", -1) == 1);

  ok(eth_abi_int16(&abi1, &d2) == 1);
  ok(eth_abi_int16(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == -0x400);
  ok(d3 == 0x400);
}

void test_eth_abi_int32(void) {
  struct eth_abi abi0, abi1;
  int32_t d0=-0x784053B0, d1=0x3ca589b0, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_int32(&abi0, &d0) == 1);
  ok(eth_abi_int32(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff87bfac50" 
          "000000000000000000000000000000000000000000000000000000003ca589b0");
  free(hex);
  
  ok(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff81cafe58"
        "00000000000000000000000000000000000000000000000000000000783f14c0", -1) == 1);

  ok(eth_abi_int32(&abi1, &d2) == 1);
  ok(eth_abi_int32(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == -0x7e3501a8);
  ok(d3 == 0x783f14c0);
}

void test_eth_abi_int64(void) {
  struct eth_abi abi0, abi1;
  int64_t d0=-0x7d15b78acecc37fc, d1=0x3ca589b0, d2, d3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_int64(&abi0, &d0) == 1);
  ok(eth_abi_int64(&abi0, &d1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 128);
  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffff82ea48753133c804" 
          "000000000000000000000000000000000000000000000000000000003ca589b0");
  free(hex);
  
  ok(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffff830dcf6847e523d0"
        "00000000000000000000000000000000000000000000000045fd66a734b67d50", -1) == 1);

  ok(eth_abi_int64(&abi1, &d2) == 1);
  ok(eth_abi_int64(&abi1, &d3) == 1);
  ok(eth_abi_free(&abi1) == 1);
  ok(d2 == -0x7cf23097b81adc30);
  ok(d3 == 0x45fd66a734b67d50);
}

void test_eth_abi_mpint(void) {
  struct eth_abi abi0, abi1;
  size_t hexlen;
  char *hex;
  mpz_t mpz0, mpz1;

  mpz_init_set_str(mpz0, "0xff", 0);
  mpz_init_set_str(mpz1, "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffbb", 0);

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_mpint(&abi0, mpz0) == 1);
  ok(eth_abi_mpint(&abi0, mpz1) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);

  is(hex, "00000000000000000000000000000000000000000000000000000000000000ff"
          "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  mpz_clears(mpz0, mpz1, NULL);
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff", -1) == 1);
  mpz_init_set_str(mpz0, "0x0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff", 0);
  mpz_init(mpz1);
  ok(eth_abi_mpint(&abi1, mpz1) == 1);
  ok(mpz_cmp(mpz0, mpz1) == 0);
  mpz_clears(mpz0, mpz1, NULL);
}

void test_eth_abi_bytes8() {
  struct eth_abi abi0, abi1;
  uint8_t b0[8] = {0xd1, 0x71, 0xf0, 0x4f, 0x6d, 0xd7, 0x69, 0x74}, b1[8];
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_bytes8(&abi0, b0) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 64);
  is(hex, "d171f04f6dd76974000000000000000000000000000000000000000000000000");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "d171f04f6dd76974000000000000000000000000000000000000000000000000", -1) == 1);
  ok(eth_abi_bytes8(&abi1, b1) == 1);
  ok(eth_abi_free(&abi1) == 1);
  cmp_mem(b1, b0, 8);
}

void test_eth_abi_bytes16() {
  struct eth_abi abi0, abi1;
  uint8_t b0[16] = {0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2,
                    0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d}, b1[16];
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_bytes16(&abi0, b0) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 64);
  is(hex, "f507a54fef38fbd2718fff5f6b54a65d00000000000000000000000000000000");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "f507a54fef38fbd2718fff5f6b54a65d00000000000000000000000000000000", -1) == 1);
  ok(eth_abi_bytes16(&abi1, b1) == 1);
  ok(eth_abi_free(&abi1) == 1);
  cmp_mem(b1, b0, 16);
}

void test_eth_abi_bytes32() {
  struct eth_abi abi0, abi1;
  uint8_t b0[32] = {0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2,
                    0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d,
                    0x57, 0xb0, 0x79, 0xdf, 0x17, 0x0b, 0x0c, 0xd7,
                    0x98, 0x5d, 0x9c, 0xe6, 0x99, 0xc0, 0x31, 0xc3},
          b1[32];
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_bytes32(&abi0, b0) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 64);
  is(hex, "f507a54fef38fbd2718fff5f6b54a65d57b079df170b0cd7985d9ce699c031c3");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "f507a54fef38fbd2718fff5f6b54a65d57b079df170b0cd7985d9ce699c031c3", -1) == 1);
  ok(eth_abi_bytes32(&abi1, b1) == 1);
  ok(eth_abi_free(&abi0) == 1);
  cmp_mem(b1, b0, 32);
}

void test_eth_abi_address(void) {
  struct eth_abi abi0, abi1;
  char *addr0 = "0x415F71C759868930B12243fBb0fb0feA4AA4435b",
       *addr1 = "1aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53",
       *addr2, *hex;
  size_t hexlen;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);
  ok(eth_abi_address(&abi0, &addr0) == 1);
  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 64);
  is(hex, "000000000000000000000000415f71c759868930b12243fbb0fb0fea4aa4435b");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
        "0000000000000000000000001aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53", -1) == 1);
  ok(eth_abi_address(&abi1, &addr2) == 1);
  ok(eth_abi_free(&abi1) == 1);
  cmp_mem(addr1, addr2, 40);
  free(addr2);
};

void test_eth_abi_bytes(void) {
  struct eth_abi abi0, abi1;
  uint8_t data0[] = {0x77, 0x23, 0xe4, 0x8a},
          *bytes0[] = {data0},
          bytes1[] = {0x71, 0xa3, 0x76, 0xab, 0xf9, 0x52, 0xa7, 0x49},
          *bytes2;
  uint64_t arrlen;
  size_t bytes0len=4, bytes1len=8, bytes2len, hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);

  ok(eth_abi_bytes(&abi0, bytes0, &bytes0len) == 1);
  ok(eth_abi_array(&abi0, NULL) == 1);
    ok(eth_abi_bytes(&abi0, bytes0, &bytes0len) == 1);
  ok(eth_abi_array_end(&abi0) == 1);

  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 512);
  is(hex, 
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000080"
    "0000000000000000000000000000000000000000000000000000000000000004"
    "7723e48a00000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000004"
    "7723e48a00000000000000000000000000000000000000000000000000000000");
  free(hex);

  ok(eth_abi_from_hex(&abi1, 
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000010"
    "71a376abf952a749000000000000000000000000000000000000000000000000", -1) == 1);

  ok(eth_abi_array(&abi1, &arrlen) == 1);
    ok(eth_abi_bytes(&abi1, &bytes2, &bytes2len) == 1);
  ok(eth_abi_array_end(&abi1) == 1);
  ok(eth_abi_free(&abi1) == 1);

  cmp_mem(bytes2, bytes1, bytes1len);
  free(bytes2);
};

void test_eth_abi_array(void) {
  struct eth_abi abi0, abi1;
  uint8_t d0 = 0xff, d1, d2;
  uint64_t l0, l1, l2, l3;
  size_t hexlen;
  char *hex;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);

  ok(eth_abi_array(&abi0, NULL) == 1);
    ok(eth_abi_array(&abi0, NULL) == 1);
      ok(eth_abi_uint8(&abi0, &d0) == 1);
    ok(eth_abi_array_end(&abi0) == 1);
    ok(eth_abi_array(&abi0, NULL) == 1);
      ok(eth_abi_uint8(&abi0, &d0) == 1);
    ok(eth_abi_array_end(&abi0) == 1);
  ok(eth_abi_array_end(&abi0) == 1);

  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  ok(hexlen == 512);

  is(hex,
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000002"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000080"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff");
  free(hex);

  ok(eth_abi_from_hex(&abi1,
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000003"
    "0000000000000000000000000000000000000000000000000000000000000060"
    "00000000000000000000000000000000000000000000000000000000000000a0"
    "00000000000000000000000000000000000000000000000000000000000000e0"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ac"
    "0000000000000000000000000000000000000000000000000000000000000000", -1) == 1);

  ok(eth_abi_array(&abi1, &l0) == 1);
    ok(eth_abi_array(&abi1, &l1) == 1);
      ok(eth_abi_uint8(&abi1, &d1) == 1);
    ok(eth_abi_array_end(&abi1) == 1);

    ok(eth_abi_array(&abi1, &l2) == 1);
      ok(eth_abi_uint8(&abi1, &d2) == 1);
    ok(eth_abi_array_end(&abi1) == 1);

    ok(eth_abi_array(&abi1, &l3) == 1);
    ok(eth_abi_array_end(&abi1) == 1);
  ok(eth_abi_array_end(&abi1) == 1);
  ok(eth_abi_free(&abi1) == 1);

  ok(l0 == 3);
  ok(l1 == 1 && d1 == 0xff);
  ok(l2 == 1 && d2 == 0xac);
  ok(l3 == 0);
};

void test_eth_abi_call(void) {
  struct eth_abi abi0, abi1;
  char *fn0 = "foo(bytes[])", *fn1, *hex;
  size_t hexlen;
  uint8_t d0 = 0xff, d1;
  int fnlen;

  ok(eth_abi_init(&abi0, ETH_ABI_ENCODE) == 1);

  ok(eth_abi_call(&abi0, &fn0, NULL) == 1);
    ok(eth_abi_array(&abi0, NULL) == 1);
      ok(eth_abi_uint8(&abi0, &d0) == 1);
    ok(eth_abi_array_end(&abi0) == 1);
  ok(eth_abi_call_end(&abi0) == 1);

  ok(eth_abi_to_hex(&abi0, &hex, &hexlen) == 1);
  ok(eth_abi_free(&abi0) == 1);
  is(hex,
      "36fe9f8d"
      "0000000000000000000000000000000000000000000000000000000000000020"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000ff");
  ok(hexlen == 200);
  free(hex);

  ok(eth_abi_from_hex(&abi1,
      "0x11602fb3"
      "0000000000000000000000000000000000000000000000000000000000000020", -1) == 1);

  ok(eth_abi_call(&abi1, &fn1, &fnlen) == 1);
    ok(eth_abi_uint8(&abi1, &d1) == 1);
  ok(eth_abi_call_end(&abi1) == 1);
  ok(eth_abi_free(&abi1) == 1);

  ok(d1 == 32);
  cmp_mem(fn1, "11602fb3", fnlen);
  ok(fnlen == 8);
};
