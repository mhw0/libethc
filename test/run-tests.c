#include "test.h"
#include <tap.h>

int main(int argc, char *argv[]) {
  test_eth_keccak256();
  done_testing();
  return 0;
}
