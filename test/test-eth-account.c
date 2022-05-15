#include "test.h"
#include <ethc/account.h>
#include <tap.h>

void test_eth_account_from_privkey(void) {
  struct eth_account account;
  char sprivkey[65], spubkey[129], saddress[41];

  uint8_t privkey[32] = {0xdc, 0xdf, 0x91, 0xb7, 0xb2, 0x9c, 0x4c, 0x06,
                         0x77, 0x33, 0x1b, 0x65, 0x5c, 0x0c, 0x6c, 0xf4,
                         0xe7, 0x1b, 0x6f, 0x87, 0x19, 0x27, 0x3a, 0x11,
                         0x2e, 0xe6, 0x5b, 0x63, 0x9b, 0x88, 0x8f, 0xfa};

  ok(eth_account_from_privkey(&account, privkey) == ETHC_SUCCESS);
  ok(eth_account_get_privkey(sprivkey, &account) == ETHC_SUCCESS);
  ok(eth_account_get_pubkey(spubkey, &account) == ETHC_SUCCESS);
  ok(eth_account_get_address(saddress, &account) == ETHC_SUCCESS);

  is(sprivkey,
     "dcdf91b7b29c4c0677331b655c0c6cf4e71b6f8719273a112ee65b639b888ffa");
  is(spubkey,
     "0699e15ad3e44a9ad7050b87cbcc30c6673b97417656604a6c4baa953e19b373c7a274e10"
     "1b777dd2c447fed7757e1f72a312b045777ab69c60deb1b0003aba5");
  is(saddress, "ed854c0a70c7eb7a720310f4ac0d8b6658e39539");
}
