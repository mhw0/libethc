#include "test.h"
#include <tap.h>
#include <ethc/unit.h>

void test_eth_unit_convert(void) {
  char *r0, *r1, *r2, *r3 = NULL;

  ok(eth_unit_convert(&r0, "1", ETH_UNIT_ETHER, ETH_UNIT_WEI) == 1);
  is(r0, "1000000000000000000");
  free(r0);

  ok(eth_unit_convert(&r1, "1", ETH_UNIT_WEI, ETH_UNIT_ETHER) == 1);
  is(r1, "0.000000000000000001");
  free(r1);

  ok(eth_unit_convert(&r2, "1000000000000000000", ETH_UNIT_WEI, ETH_UNIT_ETHER) == 1);
  is(r2, "1");
  free(r2);

  ok(eth_unit_convert(&r3, "abc", ETH_UNIT_ETHER, ETH_UNIT_WEI) == -1);
  is(r3, NULL);
  free(r3);
}
