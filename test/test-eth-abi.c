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

  diag("eth_abi_int64()");

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

  ok(d2 == -0x7cf23097b81adc30 && d3 == 0x45fd66a734b67d50, "decode -0x7cf23097b81adc30 and 0x45fd66a734b67d50");
}

void test_eth_abi_mpint(void) {
  struct eth_abi abi0={0}, abi1={0};
  mp_int int0, int1, int2, int3, int4, int5, int6;
  size_t hexlen;
  char *hex;

  diag("eth_abi_mpint()");

  assert(mp_init_multi(&int0, &int1, &int2, &int3, &int4, &int5, &int6, NULL) == MP_OKAY);

  assert(mp_read_radix(&int0, "ff", 16) == MP_OKAY);
  assert(mp_read_radix(&int1, "-256", 10) == MP_OKAY);
  assert(mp_read_radix(&int2, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", 16) == MP_OKAY);

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_mpint(&abi0, &int0) == ETH_OK);
  assert(eth_abi_mpint(&abi0, &int1) == ETH_OK);
  assert(eth_abi_mpint(&abi0, &int2) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);
  is(hex, "00000000000000000000000000000000000000000000000000000000000000ff"
          "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00"
          "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
          "encode 0xff, -256, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff8dc56"
        "0000000000000000000000000000000000000000000000000000000001f9b628", -1) == ETH_OK);

  assert(eth_abi_mpint(&abi1, &int3) == ETH_OK);
  assert(eth_abi_mpint(&abi1, &int4) == ETH_OK);

  assert(mp_read_radix(&int5, "-467882", 10) == MP_OKAY);
  assert(mp_read_radix(&int6, "33142312", 10) == MP_OKAY);

  ok(mp_cmp(&int3, &int5) == MP_EQ, "decode -467882");
  ok(mp_cmp(&int4, &int6) == MP_EQ, "decode 33142312");

  mp_clear_multi(&int0, &int1, &int2, &int3, &int4, &int5, &int6, NULL);
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

void test_eth_abi_address(void) {
  struct eth_abi abi0={0}, abi1={0};
  char *addr0 = "0x415F71C759868930B12243fBb0fb0feA4AA4435b",
       *addr1 = "1aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53",
       addr2[41], *hex;
  size_t hexlen;

  diag("eth_abi_address()");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);
  assert(eth_abi_address(&abi0, addr0) == ETH_OK);
  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex, "000000000000000000000000415f71c759868930b12243fbb0fb0fea4aa4435b",
        "encode 0x415F71C759868930B12243fBb0fb0feA4AA4435b");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
        "0000000000000000000000001aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53", -1) == ETH_OK);
  assert(eth_abi_address(&abi1, addr2) == ETH_OK);

  is(addr2, addr1, "decode 0x1aecd309e0a30d8d36ebac07f97ed3cbc7ac1b53");
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
  ok(len1 == 1 && d1 == 0xff, "decoded first inner array has 1 element and it is 0xff");
  ok(len2 == 1 && d2 == 0xab, "decoded second inner array has 1 element and it is 0xab");
};

void test_eth_abi_tuple(void) {
  struct eth_abi abi0={0}, abi1={0};
  uint8_t d0 = 0xab, d1, d2, d3;
  size_t hexlen, i = 0, len0 = 0, len1 = 0;
  char *hex = NULL;

  diag("encode/decode tuple");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);
  assert(eth_abi_array(&abi0, NULL) == ETH_OK);     //   [
    assert(eth_abi_array(&abi0, NULL) == ETH_OK);   //     [
      assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);  //       0xab
    assert(eth_abi_array_end(&abi0) == ETH_OK);     //     ],
    assert(eth_abi_uint8(&abi0, &d0) == ETH_OK);    //     0xab,
  assert(eth_abi_array_end(&abi0) == ETH_OK);       //   ]

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex,
    "00000000000000000000000000000000000000000000000000000000000000ab"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000002"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "00000000000000000000000000000000000000000000000000000000000000ab"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000ab" , "encode 0xab, [[0xab], 0xab]");
  free(hex);

  assert(eth_abi_from_hex(&abi1,
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000020"
    "0000000000000000000000000000000000000000000000000000000000000002"
    "0000000000000000000000000000000000000000000000000000000000000040"
    "00000000000000000000000000000000000000000000000000000000000000aa"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "00000000000000000000000000000000000000000000000000000000000000aa", -1) == ETH_OK);

  assert(eth_abi_array(&abi1, &len0) == ETH_OK);
    assert(eth_abi_array(&abi1, &len1) == ETH_OK);
      assert(eth_abi_uint8(&abi1, &d1) == ETH_OK);
    assert(eth_abi_array_end(&abi1) == ETH_OK);
    assert(eth_abi_uint8(&abi1, &d2) == ETH_OK);
  assert(eth_abi_array_end(&abi1) == ETH_OK);
  assert(eth_abi_uint8(&abi1, &d3) == ETH_OK);

  pass("decode [[0xaa], 0xaa], 0x20");
  ok(len0 == 2, "decoded outer array has 2 elements");
  ok(len1 == 1 && d1 == 0xaa, "decoded first inner array has 1 element and it is 0xaa");
  ok(d2 == 0xaa, "decoded second element is 0xaa");
  ok(d3 == 0x20, "decoded second element is 0x20");
}

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

void test_eth_abi_other_case_0(void) {
  struct eth_abi abi0={0}, abi1={0};
  char *hex = NULL;
  size_t tuple0len = 2, hexlen = 0;
  uint8_t uint0 = 8;

  char addr0[] = "e6b4903642b4a1637d7b411d009af5c91617860c", addr9[41];
  char addr1[] = "12d59b7849989f677a2d67fcf21b3d2e28291da6", addr10[41];
  char addr2[] = "2c7fe80e727408751b086d5e2f257ab20cd97f69", addr11[41];
  char addr3[] = "68f940fb79e216faafbc96a6701b11b44f5255c2", addr12[41];
  char addr4[] = "8da0d9418e74c8523cc189be07589a788a1c49e7", addr13[41];
  char addr5[] = "e3b3aa98faa70bf098ea5e75e11193de95ab7cda", addr14[41];
  char addr6[] = "9950c1f3754fb8a3ebbaf24b8573cafc7474c00f", addr15[41];
  char addr7[] = "afecdd2fc04f0939d7b6835529677608470c063d", addr16[41];
  char addr8[] = "c4081a370585681e265ec9efb84143549469e77e", addr17[41];
  uint8_t int0 = 1, int9;
  uint8_t int1 = 0, int10;
  uint8_t int2 = 0, int11;
  uint8_t int3 = 1, int12;
  uint8_t int4 = 0, int13;
  uint8_t int5 = 0, int14;
  uint8_t int6 = 1, int15;
  uint8_t int7 = 1, int16;
  uint8_t int8 = 0, int17;

  diag("encode/decode getPoolInfoBatchAsString(address[],uint8[])");

  assert(eth_abi_init(&abi0, ETH_ABI_ENCODE) == ETH_OK);

  assert(eth_abi_array(&abi0, NULL) == ETH_OK);
    eth_abi_address(&abi0, addr0);
    eth_abi_address(&abi0, addr1);
    eth_abi_address(&abi0, addr2);
    eth_abi_address(&abi0, addr3);
    eth_abi_address(&abi0, addr4);
    eth_abi_address(&abi0, addr5);
    eth_abi_address(&abi0, addr6);
    eth_abi_address(&abi0, addr7);
    eth_abi_address(&abi0, addr8);
  assert(eth_abi_array_end(&abi0) == ETH_OK);

  assert(eth_abi_array(&abi0, NULL) == ETH_OK);
    eth_abi_uint8(&abi0, &int0);
    eth_abi_uint8(&abi0, &int1);
    eth_abi_uint8(&abi0, &int2);
    eth_abi_uint8(&abi0, &int3);
    eth_abi_uint8(&abi0, &int4);
    eth_abi_uint8(&abi0, &int5);
    eth_abi_uint8(&abi0, &int6);
    eth_abi_uint8(&abi0, &int7);
    eth_abi_uint8(&abi0, &int8);
  assert(eth_abi_array_end(&abi0) == ETH_OK);

  assert(eth_abi_to_hex(&abi0, &hex, &hexlen) == ETH_OK);

  is(hex,
    "0000000000000000000000000000000000000000000000000000000000000040"
    "0000000000000000000000000000000000000000000000000000000000000180"
    "0000000000000000000000000000000000000000000000000000000000000009"
    "000000000000000000000000e6b4903642b4a1637d7b411d009af5c91617860c"
    "00000000000000000000000012d59b7849989f677a2d67fcf21b3d2e28291da6"
    "0000000000000000000000002c7fe80e727408751b086d5e2f257ab20cd97f69"
    "00000000000000000000000068f940fb79e216faafbc96a6701b11b44f5255c2"
    "0000000000000000000000008da0d9418e74c8523cc189be07589a788a1c49e7"
    "000000000000000000000000e3b3aa98faa70bf098ea5e75e11193de95ab7cda"
    "0000000000000000000000009950c1f3754fb8a3ebbaf24b8573cafc7474c00f"
    "000000000000000000000000afecdd2fc04f0939d7b6835529677608470c063d"
    "000000000000000000000000c4081a370585681e265ec9efb84143549469e77e"
    "0000000000000000000000000000000000000000000000000000000000000009"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000001"
    "0000000000000000000000000000000000000000000000000000000000000000", "encode");

  assert(eth_abi_from_hex(&abi1, hex, -1) == ETH_OK);

  assert(eth_abi_array(&abi1, NULL) == ETH_OK);
    eth_abi_address(&abi1, addr9);
    eth_abi_address(&abi1, addr10);
    eth_abi_address(&abi1, addr11);
    eth_abi_address(&abi1, addr12);
    eth_abi_address(&abi1, addr13);
    eth_abi_address(&abi1, addr14);
    eth_abi_address(&abi1, addr15);
    eth_abi_address(&abi1, addr16);
    eth_abi_address(&abi1, addr17);
  assert(eth_abi_array_end(&abi1) == ETH_OK);

  assert(eth_abi_array(&abi1, NULL) == ETH_OK);
    eth_abi_uint8(&abi1, &int9);
    eth_abi_uint8(&abi1, &int10);
    eth_abi_uint8(&abi1, &int11);
    eth_abi_uint8(&abi1, &int12);
    eth_abi_uint8(&abi1, &int13);
    eth_abi_uint8(&abi1, &int14);
    eth_abi_uint8(&abi1, &int15);
    eth_abi_uint8(&abi1, &int16);
    eth_abi_uint8(&abi1, &int17);
  assert(eth_abi_array_end(&abi1) == ETH_OK);

  is(addr9, addr0, "decode e6b4903642b4a1637d7b411d009af5c91617860c");
  is(addr10, addr1, "decode 12d59b7849989f677a2d67fcf21b3d2e28291da6");
  is(addr11, addr2, "decode 2c7fe80e727408751b086d5e2f257ab20cd97f69");
  is(addr12, addr3, "decode 68f940fb79e216faafbc96a6701b11b44f5255c2");
  is(addr13, addr4, "decode 8da0d9418e74c8523cc189be07589a788a1c49e7");
  is(addr14, addr5, "decode e3b3aa98faa70bf098ea5e75e11193de95ab7cda");
  is(addr15, addr6, "decode 9950c1f3754fb8a3ebbaf24b8573cafc7474c00f");
  is(addr16, addr7, "decode afecdd2fc04f0939d7b6835529677608470c063d");
  is(addr17, addr8, "decode c4081a370585681e265ec9efb84143549469e77e");

  ok(int9 == int0, "decode 1");
  ok(int10 == int1, "decode 0");
  ok(int11 == int2, "decode 0");
  ok(int12 == int3, "decode 1");
  ok(int13 == int4, "decode 0");
  ok(int14 == int5, "decode 0");
  ok(int15 == int6, "decode 1");
  ok(int16 == int7, "decode 1");
  ok(int17 == int8, "decode 0");

  free(hex);
}
