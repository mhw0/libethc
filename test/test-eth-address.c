#include <tap.h>
#include "test.h"
#include <ethc/address.h>

void test_eth_is_address(void) {
  const char *in1 = NULL;
  ok(eth_is_address(in1) == -1);

  const char *in2 = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_address(in2) == 1);

  const char *in3 = "00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_address(in3) == 1);

  const char *in4 = "0x00000d42e4306c2097b";
  ok(eth_is_address(in4) <= 0);
}

void test_eth_is_checksum_address(void) {
  const char *in1 = NULL;
  ok(eth_is_checksum_address(in1) == -1);

  const char *in2 = "0x00000D42e4306c2097B2BD817Efa00dEc96903F1";
  ok(eth_is_checksum_address(in2) == 1);

  const char *in3 = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_checksum_address(in3) == 0);

  const char *in4 = "00000D42e4306c2097B2BD817Efa00dEc96903F1";
  ok(eth_is_checksum_address(in4) == 1);
}

void test_eth_to_checksum_address(void) {
  char *in1 = NULL;
  ok(eth_to_checksum_address(in1) == -1);

  char in2[] = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  char *exp2 = "0x00000D42e4306c2097B2BD817Efa00dEc96903F1";
  ok(eth_to_checksum_address(in2) == 1);
  is(in2, exp2);
}
