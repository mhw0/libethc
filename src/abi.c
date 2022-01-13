#include <ethc/abi.h>
#include <ethc/hex.h>
#include <ethc/address.h>

int eth_abi_encode_bool(char *rstr, int b) {
  if(!rstr)
    return 0;

  if(b != 0 && b != 1)
    return 0;

  return eth_hex_pad_left(rstr, b == 0 ? "0" : "1", 1, 64);
}

int eth_abi_encode_address(char *rstr, char *str) {
  if(!rstr || !str)
    return 0;

  if(!eth_is_address(str))
    return 0;

  str += 2;

  return eth_hex_pad_left(rstr, str, 40, 64);
}
