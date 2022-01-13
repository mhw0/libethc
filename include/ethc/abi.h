#ifndef ETHC_ABI_H
#define ETHC_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

int eth_abi_encode_bool(char *rstr, int b);
int eth_abi_encode_address(char *rstr, char *str);

#ifdef __cplusplus
}
#endif

#endif
