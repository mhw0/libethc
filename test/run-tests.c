#include "test.h"
#include <tap.h>

int main() {
  test_eth_keccak256();
  test_eth_keccak256p();

  test_eth_is_hex();
  test_eth_hex_pad_left();
  test_eth_hex_pad_right();
  test_eth_hex_from_bytes();
  test_eth_hex_to_bytes();

  test_eth_is_address();
  test_eth_is_checksum_address();
  test_eth_to_checksum_address();

  test_eth_bloom_copy();
  test_eth_bloom_and();
  test_eth_bloom_or();

  test_eth_unit_convert();

  test_eth_ecdsa_pubkey_get();
  test_eth_ecdsa_sign();

  test_eth_account_privkey_get();
  test_eth_account_pubkey_get();
  test_eth_account_address_get();
  test_eth_account_sign();

  test_eth_abi_bool();
  test_eth_abi_uint8();
  test_eth_abi_uint16();
  test_eth_abi_uint32();
  test_eth_abi_uint64();
  test_eth_abi_int8();
  test_eth_abi_int16();
  test_eth_abi_int32();
  test_eth_abi_int64();
  test_eth_abi_bytes8();
  test_eth_abi_bytes16();
  test_eth_abi_bytes32();
  test_eth_abi_address();
  test_eth_abi_bytes();
  test_eth_abi_array();
  test_eth_abi_call();

  test_eth_rlp_uint8();
  test_eth_rlp_uint16();
  test_eth_rlp_uint32();
  test_eth_rlp_uint64();
  test_eth_rlp_uint();
  test_eth_rlp_bytes();
  test_eth_rlp_hex();
  test_eth_rlp_address();
  test_eth_rlp_to_hex();
  test_eth_rlp_to_bytes();
  test_eth_abi_mpint();

  done_testing();
  return 0;
}
