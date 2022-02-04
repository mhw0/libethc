#include "test.h"
#include <tap.h>
#include <ethc/unit.h>

void test_eth_unit_convert(void) {
  char *out1 = eth_unit_convert("1", ETH_UNIT_ETHER, ETH_UNIT_WEI);
  const char *exp1 = "1000000000000000000";
  is(out1, exp1);

  char *out2 = eth_unit_convert("1", ETH_UNIT_WEI, ETH_UNIT_ETHER);
  const char *exp2 = "0.000000000000000001";
  is(out2, exp2);

  char *out3 = eth_unit_convert("1000000000000000000", ETH_UNIT_WEI, ETH_UNIT_ETHER);
  const char *exp3 = "1";
  is(out3, exp3);

  char *out4 = eth_unit_convert("abc", ETH_UNIT_ETHER, ETH_UNIT_WEI);
  const char *exp4 = NULL;
  is(out4, exp4);
}
