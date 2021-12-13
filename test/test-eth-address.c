#include <tap.h>
#include "test.h"
#include <ethc/address.h>
#include <string.h>

void test_eth_is_address(void) {
  const char *in0 = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_address(in0, strlen(in0)) == 1);

  const char *in1 = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_address(in1, -1) == 1);

  const char *in2 = "0x00000d42e4306c2097b";
  ok(eth_is_address(in2, -1) == 0);
}

void test_eth_is_checksum_address(void) {
  const char *in0 = "0x00000D42e4306c2097B2BD817Efa00dEc96903F1";
  ok(eth_is_checksum_address(in0, strlen(in0)) == 1);

  const char *in1 = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_is_checksum_address(in1, -1) == 0);
}
