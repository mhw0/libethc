#include "test.h"
#include <tap.h>

int main() {
  test_eth_keccak256();
  test_eth_keccak256p();

  test_eth_is_hex();
  test_eth_hex_pad_left();
  test_eth_hex_pad_right();
  test_eth_hex_from_bytes();

  test_eth_is_address();
  test_eth_is_checksum_address();
  test_eth_to_checksum_address();

  test_eth_bloom_copy();
  test_eth_bloom_and();
  test_eth_bloom_or();

  test_eth_abi_encode_bool();
  test_eth_abi_encode_address();
  test_eth_abi_encode_int();
  test_eth_abi_encode_uint();
  test_eth_abi_encode_bytes();
  test_eth_abi_encode_bytesd();
  test_eth_abi_encode_stringd();
  test_eth_abi_encode_func();
  test_eth_abi_encode_event();

  test_eth_unit_convert();

  test_eth_utf8_strlen();

  test_eth_ec_get_pubkey();

  done_testing();
  return 0;
}
