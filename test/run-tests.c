#include "test.h"
#include <tap.h>

int main() {
  test_eth_keccak256();

  test_eth_is_hexstr();
  test_eth_hexstr_pad_left();
  test_eth_hexstr_pad_right();

  test_eth_is_address();
  test_eth_is_checksum_address();
  test_eth_to_checksum_address();

  done_testing();
  return 0;
}
