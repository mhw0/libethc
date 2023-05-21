#ifndef ETHC_ERC20_H
#define ETHC_ERC20_H

#ifdef __cplusplus
extern "C" {
#endif

ETHC_EXPORT int eth_erc20_abi_name(struct eth_abi *abi);
ETHC_EXPORT int eth_erc20_abi_symbol(struct eth_abi *abi);
ETHC_EXPORT int eth_erc20_abi_decimals(struct eth_abi *abi);
ETHC_EXPORT int eth_erc20_abi_totalSupply(struct eth_abi *abi);
ETHC_EXPORT int eth_erc20_abi_balanceOf(struct eth_abi *abi, char **owner);
ETHC_EXPORT int eth_erc20_abi_transfer(struct eth_abi *abi, char **to, char **value);
ETHC_EXPORT int eth_erc20_abi_transferFrom(struct eth_abi *abi, char **from, char **to, char **value);
ETHC_EXPORT int eth_erc20_abi_approve(struct eth_abi *abi, char **spender, char **value);
ETHC_EXPORT int eth_erc20_abi_allowance(struct eth_abi *abi, char **owner, char **spender);

int eth_erc20_abi_name(struct eth_abi *abi) {
  char *func = "name";

  if (eth_abi_call(abi, &func, NULL) < 0)
    return -1;

  if (eth_abi_call_end(abi) < 0)
    return -1;

  return 1;
}

char *addr = "0xff";
struct eth_abi abi;

ethc_ok(eth_abi_init(&abi, ETH_ABI_ENCODE));
ethc_ok(eth_erc20_abi_balanceOf(&abi, &addr));

#ifdef __cplusplus
}
#endif

#endif
