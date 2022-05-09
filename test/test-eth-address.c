#include <tap.h>
#include "test.h"
#include <ethc/address.h>

static const char *address = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
static const char *checksum_address = "0x00000D42e4306c2097B2BD817Efa00dEc96903F1";
static const char *hex = "0x00000d42e4306c2097b";

void test_eth_is_address(void) {
  ok(eth_is_address(address) == ETHC_TRUE);
  ok(eth_is_address(hex) == ETHC_FALSE);
}

void test_eth_is_checksum_address(void) {
  ok(eth_is_checksum_address(checksum_address) == ETHC_TRUE);
  ok(eth_is_checksum_address(address) == ETHC_FALSE);
}

void test_eth_to_checksum_address(void) {
  char address2[] = "0x00000d42e4306c2097b2bd817efa00dec96903f1";
  ok(eth_to_checksum_address(address2) == ETHC_SUCCESS);
  is(address2, checksum_address);
}
