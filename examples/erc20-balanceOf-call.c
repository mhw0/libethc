#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ethc/abi.h>

#define ok(ethcop) assert(ethcop >= 0)

int main(void) {
  struct eth_abi abi;
  char *fn = "balanceOf(address)", *addr="0x876D477Bd5cD050E6162cf757E1Bc02D93cdC0fE", *hex;
  size_t hexlen;

  ok(eth_abi_init(&abi, ETH_ABI_ENCODE));

  ok(eth_abi_call(&abi, &fn, NULL));  // balanceOf(
    ok(eth_abi_address(&abi, &addr)); //   0x876D477Bd5cD050E6162cf757E1Bc02D93cdC0fE
  ok(eth_abi_call_end(&abi));         // )

  ok(eth_abi_to_hex(&abi, &hex, &hexlen));
  ok(eth_abi_free(&abi));
  printf("encoded abi: %s\n", hex); // encoded abi: 70a08231000000000000000000000000876d477bd5cd050e6162cf757e1bc02d93cdc0fe
  free(hex);
  return 0;
}
