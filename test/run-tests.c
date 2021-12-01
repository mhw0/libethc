#include "test.h"
#include <tap.h>

int main(int argc, char *argv[]) {
  test_eth_keccak256();

  test_eth_is_hex_str();
  test_eth_hex_pad_left();
  test_eth_hex_pad_right();
  done_testing();
  return 0;
}
