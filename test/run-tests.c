#include "test.h"
#include <tap.h>

int main(int argc, char *argv[]) {
  test_eth_keccak256();

  test_eth_is_hexstr();
  test_eth_hexstr_pad_left();
  test_eth_hexstr_pad_right();

  done_testing();
  return 0;
}
