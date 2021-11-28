#include "test.h"

int main(int argc, char *argv[]) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_eth_keccak256)
  };

  cmocka_set_message_output(CM_OUTPUT_TAP);
  return cmocka_run_group_tests (tests, NULL,  NULL);
}
