#include "test.h"
#include <ethc/utf8.h>
#include <tap.h>

void test_eth_utf8_strlen(void) {
  const char *in1 = "ΘΔΩ";
  ok(eth_utf8_strlen(in1, -1) == 3);

  const char *in2 = "abcd";
  ok(eth_utf8_strlen(in2, -1) == 4);
}
