#include "test.h"
#include <tap.h>
#include <ethc/unit.h>
#include <ethc/mp.h>
#include <string.h>
#include <assert.h>

void test_eth_unit_convert(void) {
  char *str0 = malloc(32), *str1 = malloc(32);
  int decimals0 = 0, decimals1 = 0;
  mp_int int0, int1, scale0, scale1;

  diag("eth_unit_convert()");

  assert(mp_init_multi(&int0, &int1, &scale0, &scale1, NULL) == MP_OKAY);

  assert(eth_mp_from_str(&int0, &scale0, "100", -1) == ETH_OK);
  assert(eth_unit_convert(&int0, &decimals0, &int0, &scale0, ETH_UNIT_ETHER, ETH_UNIT_WEI) == ETH_OK);
  assert(eth_mp_to_str(str0, &int0, decimals0) == ETH_OK);

  is(str0, "100000000000000000000", "100 ETHER = 100,000,000,000,000,000,000 WEI");


  assert(eth_mp_from_str(&int1, &scale1, "10000", -1) == ETH_OK);
  assert(eth_unit_convert(&int1, &decimals1, &int1, &scale1, ETH_UNIT_WEI, ETH_UNIT_ETHER) == ETH_OK);
  assert(eth_mp_to_str(str1, &int1, decimals1) == ETH_OK);

  is(str1, "0.000000000000010000", "10,000 WEI = 0.000000000000010000 ETHER");

  mp_clear_multi(&int0, &int1, &scale0, &scale1, NULL);
  free(str0);
  free(str1);
}
