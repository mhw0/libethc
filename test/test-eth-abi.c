#include "test.h"
#include <ethc/abi.h>
#include <tap.h>
#include <assert.h>

void test_eth_abi_bool(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t b0=1, b1=0, b2, b3;
  char *hex;
  size_t hexlen;

  diag("eth_abi_bool()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_bool(&abi0, &b0) == ETH_OK);

  assert(eth_abi_bool(&abi0, &b1) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "0000000000000000000000000000000000000000000000000000000000000001" 
          "0000000000000000000000000000000000000000000000000000000000000000", "encode true(1) and false(0)");
  
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000001", -1) == ETH_OK);
  
  assert(eth_abi_bool(&abi1, &b2) == ETH_OK);
  assert(eth_abi_bool(&abi1, &b3) == ETH_OK);

  ok(b2 == 0 && b3 == 1, "decode false(0) and true(1)");
}

void test_eth_abi_uint8(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t d0=0x32, d1=0xf2, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_uint8()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);
  assert(eth_abi_uint8(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "0000000000000000000000000000000000000000000000000000000000000032" 
          "00000000000000000000000000000000000000000000000000000000000000f2", "encode 0x32 and 0xf2");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "00000000000000000000000000000000000000000000000000000000000000ff"
        "00000000000000000000000000000000000000000000000000000000000000ac", -1) == ETH_OK);
  assert(eth_abi_uint8(&abi1, &d2) == ETH_OK);
  assert(eth_abi_uint8(&abi1, &d3) == ETH_OK);

  ok(d2 == 0xff && d3 == 0xac, "decode 0xff and 0xac");
}

void test_eth_abi_uint16(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint16_t d0=0x6109, d1=0x3d0c, d2, d3;
  size_t hexlen;
  char *hex, *method;

  diag("eth_abi_uint16()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_uint16(&abi0, &d0) == ETH_OK);
  assert(eth_abi_uint16(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "0000000000000000000000000000000000000000000000000000000000006109" 
          "0000000000000000000000000000000000000000000000000000000000003d0c", "encode 0x6109 and 0x3d0c");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000000000000000e0cb"
        "000000000000000000000000000000000000000000000000000000000000f38f", -1) == ETH_OK);
  assert(eth_abi_uint16(&abi1, &d2) == ETH_OK);
  assert(eth_abi_uint16(&abi1, &d3) == ETH_OK);

  ok(d2 == 0xe0cb && d3 == 0xf38f, "decode 0xe0cb and 0xf38f");
}

void test_eth_abi_uint32(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint32_t d0=0xef20f08f, d1=0x35d6887c, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_uint32()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_uint32(&abi0, &d0) == ETH_OK);
  assert(eth_abi_uint32(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "00000000000000000000000000000000000000000000000000000000ef20f08f" 
          "0000000000000000000000000000000000000000000000000000000035d6887c", "encode 0xef20f08f and 0x35d6887c");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000000000007114ee5e"
        "00000000000000000000000000000000000000000000000000000000e704d479", -1) == ETH_OK);
  assert(eth_abi_uint32(&abi1, &d2) == ETH_OK);
  assert(eth_abi_uint32(&abi1, &d3) == ETH_OK);

  ok(d2 == 0x7114ee5e && d3 == 0xe704d479, "decode 0x7114ee5e and 0x704d479");
}

void test_eth_abi_uint64(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint64_t d0=0xab16c0a9773532a4, d1=0xfb4ddbc3bb9fd42f, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_uint64()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_uint64(&abi0, &d0) == ETH_OK);
  assert(eth_abi_uint64(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "000000000000000000000000000000000000000000000000ab16c0a9773532a4" 
          "000000000000000000000000000000000000000000000000fb4ddbc3bb9fd42f", "encode 0xab16c0a9773532a4 and 0xfb4ddbc3bb9fd42f");
  free(hex);
  
  assert(eth_abi_from_hex(&abi1,
        "000000000000000000000000000000000000000000000000a61611e899af529c"
        "0000000000000000000000000000000000000000000000007f6d3cbc578d68b0", -1) == ETH_OK);
  assert(eth_abi_uint64(&abi1, &d2) == ETH_OK);
  assert(eth_abi_uint64(&abi1, &d3) == ETH_OK);

  ok(d2 == 0xa61611e899af529c && d3 == 0x7f6d3cbc578d68b0, "decode 0xa61611e899af529c and 0x7f6d3cbc578d68b0");
}

void test_eth_abi_int8(void) {
  struct eth_abi abi0={0}, abi1={0};
  int8_t d0=-0x04, d1=0x64, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_int8()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_int8(&abi0, &d0) == ETH_OK);
  assert(eth_abi_int8(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc" 
          "0000000000000000000000000000000000000000000000000000000000000064", "encode -0x04 and 0x64");
  free(hex);
  
  assert(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff85"
        "0000000000000000000000000000000000000000000000000000000000000064", -1) == ETH_OK);
  assert(eth_abi_int8(&abi1, &d2) == ETH_OK);
  assert(eth_abi_int8(&abi1, &d3) == ETH_OK);

  ok(d2 == -0x7b && d3 == 0x64, "decode -0x7b and 0x64");
}

void test_eth_abi_int16(void) {
  struct eth_abi abi0={0}, abi1={0};
  int16_t d0=-0x7830, d1=0xf03, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_int16()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_int16(&abi0, &d0) == ETH_OK);
  assert(eth_abi_int16(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff87d0" 
          "0000000000000000000000000000000000000000000000000000000000000f03", "encode -0x7830 and 0xf03");
  free(hex);
  
  assert(eth_abi_from_hex(&abi1,
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc00"
        "0000000000000000000000000000000000000000000000000000000000000400", -1) == ETH_OK);

  assert(eth_abi_int16(&abi1, &d2) == ETH_OK);
  assert(eth_abi_int16(&abi1, &d3) == ETH_OK);

  ok(d2 == -0x400 && d3 == 0x400, "decode -0x400 and 0x400");
}

void test_eth_abi_int32(void) {
  struct eth_abi abi0={0}, abi1={0};
  int32_t d0=-0x784053b0, d1=0x3ca589b0, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_int32()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_int32(&abi0, &d0) == ETH_OK);
  assert(eth_abi_int32(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff87bfac50" 
          "000000000000000000000000000000000000000000000000000000003ca589b0", "encode -0x784053b0 and 0x3ca589b0");
  free(hex);
  
  assert(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff81cafe58"
        "00000000000000000000000000000000000000000000000000000000783f14c0", -1) == ETH_OK);

  assert(eth_abi_int32(&abi1, &d2) == ETH_OK);
  assert(eth_abi_int32(&abi1, &d3) == ETH_OK);

  ok(d2 == -0x7e3501a8 && d3 == 0x783f14c0, "decode -0x7e3501a8 and 0x783f14c0");
}

void test_eth_abi_int64(void) {
  struct eth_abi abi0={0}, abi1={0};
  int64_t d0=-0x7d15b78acecc37fc, d1=0x3ca589b0, d2, d3;
  size_t hexlen;
  char *hex;

  diag("eth_abi_int32()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_int64(&abi0, &d0) == ETH_OK);
  assert(eth_abi_int64(&abi0, &d1) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "ffffffffffffffffffffffffffffffffffffffffffffffff82ea48753133c804" 
          "000000000000000000000000000000000000000000000000000000003ca589b0", "encode -0x7d15b78acecc37fc and 0x3ca589b0");
  free(hex);
  
  assert(eth_abi_from_hex(&abi1,
        "ffffffffffffffffffffffffffffffffffffffffffffffff830dcf6847e523d0"
        "00000000000000000000000000000000000000000000000045fd66a734b67d50", -1) == ETH_OK);

  assert(eth_abi_int64(&abi1, &d2) == ETH_OK);
  assert(eth_abi_int64(&abi1, &d3) == ETH_OK);

  ok(d2 == -0x7cf23097b81adc30 && d3 == 0x45fd66a734b67d50, "encode -0x7cf23097b81adc30 and 0x45fd66a734b67d50");
}

void test_eth_abi_mpint(void) {
  struct eth_abi abi0={0}, abi1={0};
  mp_int int0, int1, int2, int3, int4;
  size_t hexlen;
  char *hex;

  diag("eth_abi_mpint()");

  assert(mp_init_multi(&int0, &int1, &int2, &int3, &int4, NULL) == MP_OKAY);

  assert(mp_read_radix(&int0, "ff", 16) == MP_OKAY);
  assert(mp_read_radix(&int1, "fff", 16) == MP_OKAY);
  assert(mp_read_radix(&int2, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", 16) == MP_OKAY);

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_mpint(&abi0, &int0) == ETH_OK);
  assert(eth_abi_mpint(&abi0, &int1) == ETH_OK);
  assert(eth_abi_mpint(&abi0, &int2) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);
  is(hex, "00000000000000000000000000000000000000000000000000000000000000ff"
          "0000000000000000000000000000000000000000000000000000000000000fff"
          "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
          "encode 0xff, 0xfff, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  free(hex);


  assert(eth_abi_from_hex(&abi1,
        "0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff", -1) == ETH_OK);
  assert(mp_read_radix(&int3, "0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff", 16) == ETH_OK);

  assert(eth_abi_mpint(&abi1, &int4) == ETH_OK);

  ok(mp_cmp(&int3, &int4) == MP_EQ, "decode 0x0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  mp_clear_multi(&int0, &int1, &int2, &int3, &int4, NULL);
}

void test_eth_abi_bytes8() {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t b0[8] = {0xd1, 0x71, 0xf0, 0x4f, 0x6d, 0xd7, 0x69, 0x74}, b1[8];
  size_t hexlen;
  char *hex;

  diag("eth_abi_bytes8()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_bytes8(&abi0, b0) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "d171f04f6dd76974000000000000000000000000000000000000000000000000", "encode 0xd1, 0x71, 0xf0, 0x4f, 0x6d, 0xd7, 0x69, 0x74");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "d171f04f6dd76974000000000000000000000000000000000000000000000000", -1) == ETH_OK);
  assert(eth_abi_bytes8(&abi1, b1) == ETH_OK);

  cmp_mem(b1, b0, 8, "decode 0xd1, 0x71, 0xf0, 0x4f, 0x6d, 0xd7, 0x69, 0x74");
}

void test_eth_abi_bytes16() {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t b0[16] = {0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2,
                    0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d}, b1[16];
  size_t hexlen;
  char *hex;

  diag("eth_abi_bytes16()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_bytes16(&abi0, b0) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "f507a54fef38fbd2718fff5f6b54a65d00000000000000000000000000000000",
        "encode 0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2, 0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "f507a54fef38fbd2718fff5f6b54a65d00000000000000000000000000000000", -1) == ETH_OK);
  assert(eth_abi_bytes16(&abi1, b1) == ETH_OK);

  cmp_mem(b1, b0, 16, "decode 0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2, 0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d");
}

void test_eth_abi_bytes32() {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t b0[32] = {0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2,
                    0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d,
                    0x57, 0xb0, 0x79, 0xdf, 0x17, 0x0b, 0x0c, 0xd7,
                    0x98, 0x5d, 0x9c, 0xe6, 0x99, 0xc0, 0x31, 0xc3},
          b1[32];
  size_t hexlen;
  char *hex;

  diag("eth_abi_bytes32()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_bytes32(&abi0, b0) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "f507a54fef38fbd2718fff5f6b54a65d57b079df170b0cd7985d9ce699c031c3",
      "encode 0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2, "
      "0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d, "
      "0x57, 0xb0, 0x79, 0xdf, 0x17, 0x0b, 0x0c, 0xd7, "
      "0x98, 0x5d, 0x9c, 0xe6, 0x99, 0xc0, 0x31, 0xc3");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
          "f507a54fef38fbd2718fff5f6b54a65d57b079df170b0cd7985d9ce699c031c3", -1) == ETH_OK);
  assert(eth_abi_bytes32(&abi1, b1) == ETH_OK);

  cmp_mem(b1, b0, 32,
      "encode 0xf5, 0x07, 0xa5, 0x4f, 0xef, 0x38, 0xfb, 0xd2, "
      "0x71, 0x8f, 0xff, 0x5f, 0x6b, 0x54, 0xa6, 0x5d, "
      "0x57, 0xb0, 0x79, 0xdf, 0x17, 0x0b, 0x0c, 0xd7, "
      "0x98, 0x5d, 0x9c, 0xe6, 0x99, 0xc0, 0x31, 0xc3");
}

void test_eth_abi_address(void) {
  struct eth_abi abi0={0}, abi1={0};
  char *addr0 = "0x415F71C759868930B12243fBb0fb0feA4AA4435b",
       *addr1 = "1aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53",
       *addr2, *hex;
  size_t hexlen;

  diag("eth_abi_address()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_address(&abi0, &addr0) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "000000000000000000000000415f71c759868930b12243fbb0fb0fea4aa4435b",
        "encode 0x415F71C759868930B12243fBb0fb0feA4AA4435b");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "0000000000000000000000001aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53", -1) == ETH_OK);
  assert(eth_abi_address(&abi1, &addr2) == ETH_OK);

  cmp_mem(addr1, addr2, 40, "decode 0x1aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53");

  free(addr2);
};

void test_eth_abi_bytes(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t data0[] = {0x77, 0x23, 0xe4, 0x8a},
          data1[] = {0x00, 0x00, 0x00, 0x00};
  uint64_t arrlen;
  size_t datalen=4, hexlen;
  char *hex;

  diag("eth_abi_bytes()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_bytes(&abi0, data0, &datalen) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, 
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000004"
    "7723e48a00000000000000000000000000000000000000000000000000000000", "encode 0x77, 0x23, 0xe4, 0x8a");
  free(hex);

  assert(eth_abi_from_hex(&abi1, 
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000004"
    "7723e48a00000000000000000000000000000000000000000000000000000000", -1) == ETH_OK);

  assert(eth_abi_bytes(&abi1, data1, &datalen) == ETH_OK);

  cmp_mem(data1, data0, datalen, "decode 0x77, 0x23, 0xe4, 0x8a");
};

void test_eth_abi_array(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t d0 = 0xff, d1, d2;
  size_t len0, len1, len2, len3;
  size_t hexlen, i = 0;
  char *hex;

  diag("eth_abi_array()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_array(&abi0, NULL) == ETH_OK);
    assert(eth_abi_array(&abi0, NULL) == ETH_OK);
      assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);
    assert(eth_abi_array_end(&abi0) == ETH_OK);
    assert(eth_abi_array(&abi0, NULL) == ETH_OK);
      assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);
    assert(eth_abi_array_end(&abi0) == ETH_OK);
  assert(eth_abi_array_end(&abi0) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex,
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000002"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000080"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff", "encode [[0xff], [0xff]]");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000002"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000080"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ff"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ab", -1) == ETH_OK);

  assert(eth_abi_array(&abi1, &len0) == ETH_OK);
    assert(eth_abi_array(&abi1, &len1) == ETH_OK);
      assert(eth_abi_uint8(&abi1, &d1) == ETH_OK);
    assert(eth_abi_array_end(&abi1) == ETH_OK);
    assert(eth_abi_array(&abi1, &len2) == ETH_OK);
      assert(eth_abi_uint8(&abi1, &d2) == ETH_OK);
    assert(eth_abi_array_end(&abi1) == ETH_OK);
  assert(eth_abi_array_end(&abi1) == ETH_OK);

  ok(len0 == 2, "decoded outer array has 2 elements");
  ok(len1 == 1 && d1 == 0xff, "decoded first inner array has 1 element and it is0xff");
  ok(len2 == 1 && d2 == 0xab, "decoded second inner array has 1 element and it is 0xab");
};

void test_eth_abi_call(void) {
  struct eth_abi abi0={0}, abi1={0};
  char *fn0 = "foo(bytes[])", *fn1 = malloc(9), *hex;
  uint8_t d0 = 0xff, d1;
  size_t hexlen;
  int fnlen;

  diag("eth_abi_call()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_call(&abi0, &fn0, NULL) == ETH_OK);
    assert(eth_abi_array(&abi0, NULL) == ETH_OK);
      assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);
    assert(eth_abi_array_end(&abi0) == ETH_OK);
  assert(eth_abi_call_end(&abi0) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex,
      "36fe9f8d"
      "0000000000000000000000000000000000000000000000000000000000000020"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000ff", "encode foo([0xff])");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
      "0x11602fb3"
      "0000000000000000000000000000000000000000000000000000000000000020", -1) == ETH_OK);

  assert(eth_abi_call(&abi1, &fn1, &fnlen) == ETH_OK);
    assert(eth_abi_uint8(&abi1, &d1) == ETH_OK);
  assert(eth_abi_call_end(&abi1) == ETH_OK);

  ok(d1 == 0x20, "decoded uint is 0x20");
  cmp_mem(fn1, "11602fb3", fnlen, "decoded function selector is 0x11602fb3");

  free(fn1);
};
