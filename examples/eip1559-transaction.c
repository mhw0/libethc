#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ethc/ecdsa.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>

#define ok(ethcop) assert(ethcop >= 0)

int main(void) {
  struct eth_rlp rlp0, rlp1;
  struct eth_ecdsa_signature sign;
  uint8_t privkey[] = {0xdf, 0x57, 0x08, 0x9f, 0xeb, 0xba, 0xcf, 0x7b,
                       0xa0, 0xbc, 0x22, 0x7d, 0xaf, 0xbf, 0xfa, 0x9f,
                       0xc0, 0x8a, 0x93, 0xfd, 0xc6, 0x8e, 0x1e, 0x42,
                       0x41, 0x1a, 0x14, 0xef, 0xcf, 0x23, 0x65, 0x6e};

  uint8_t nonce = 0x00, zero = 0x00, keccak[32], *rlp0bytes, v, *r, *s;
  char *gaslimit = "0x5208";
  char *max_priority_fee_per_gas = "0x3B9ACA00";  // 1 Gwei
  char *max_fee_per_gas = "0x1bf08eb000";         // 120 Gwei
  char *value = "0x0de0b6b3a7640000";
  char *toaddr = "0x3535353535353535353535353535353535353535", *txn;
  char *chain_id = "0xAA36A7";  // Ethereum Sepolia testnet
  char *abi_data = "0x1234abcd"; // replace it with abi-encoded data
  char *accees_list = "0x";
  size_t rlp0len, rlp1len, siglen = 32;
  uint8_t tx_type_prefix = 0x02;
  char* tx_type_prefix_str = "0x02";

  // EIP-1559 unsigned tx
  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE));
    ok(eth_rlp_array(&rlp0));
    ok(eth_rlp_hex(&rlp0, &chain_id, NULL));
    ok(eth_rlp_uint8(&rlp0, &nonce));
    ok(eth_rlp_hex(&rlp0, &max_priority_fee_per_gas, NULL));
    ok(eth_rlp_hex(&rlp0, &max_fee_per_gas, NULL));
    ok(eth_rlp_hex(&rlp0, &gaslimit, NULL));
    ok(eth_rlp_address(&rlp0, &toaddr));
    ok(eth_rlp_hex(&rlp0, &value, NULL));
    ok(eth_rlp_hex(&rlp0, &abi_data, NULL));  // empty 
    ok(eth_rlp_array(&rlp0));
      // ok(eth_rlp_hex(&rlp0, &accees_list, NULL)); // empty for access_list
    ok(eth_rlp_array_end(&rlp0));
  ok(eth_rlp_array_end(&rlp0));

  ok(eth_rlp_to_bytes(&rlp0bytes, &rlp0len, &rlp0));
  ok(eth_rlp_free(&rlp0));

  // put EIP-1559 prefix
  size_t unsigned_tx_with_type_len = sizeof(tx_type_prefix) + rlp0len;
  uint8_t *unsigned_tx_with_type = (uint8_t*)malloc(unsigned_tx_with_type_len);
  memcpy(unsigned_tx_with_type, &tx_type_prefix, sizeof(tx_type_prefix));
  memcpy(unsigned_tx_with_type + sizeof(tx_type_prefix), rlp0bytes, rlp0len);

  // compute the keccak hash of the encoded rlp elements
  ok(eth_keccak256(keccak, unsigned_tx_with_type, unsigned_tx_with_type_len));
  free(rlp0bytes);
  free(unsigned_tx_with_type);

  // sign the transaction
  ok(eth_ecdsa_sign(&sign, privkey, keccak));

  // calculate v
  v = sign.recid;
  r = sign.r;
  s = sign.s;

  // encode tx
  ok(eth_rlp_init(&rlp1, ETH_RLP_ENCODE));

  ok(eth_rlp_array(&rlp1));
    ok(eth_rlp_hex(&rlp1, &chain_id, NULL));
    ok(eth_rlp_uint8(&rlp1, &nonce));
    ok(eth_rlp_hex(&rlp1, &max_priority_fee_per_gas, NULL));
    ok(eth_rlp_hex(&rlp1, &max_fee_per_gas, NULL));
    ok(eth_rlp_hex(&rlp1, &gaslimit, NULL));
    ok(eth_rlp_address(&rlp1, &toaddr));
    ok(eth_rlp_hex(&rlp1, &value, NULL));
    ok(eth_rlp_hex(&rlp1, &abi_data, NULL));
    ok(eth_rlp_array(&rlp1));
      // ok(eth_rlp_hex(&rlp1, &accees_list, NULL)); // empty for access list
    ok(eth_rlp_array_end(&rlp1));
    ok(eth_rlp_uint8(&rlp1, &v));
    ok(eth_rlp_bytes(&rlp1, &r, &siglen));
    ok(eth_rlp_bytes(&rlp1, &s, &siglen));
  ok(eth_rlp_array_end(&rlp1));

  ok(eth_rlp_to_hex(&txn, &rlp1));
  ok(eth_rlp_free(&rlp1));

  int tx_size = strlen(txn);
  int tx_prefix_size = strlen(tx_type_prefix_str);
  char signed_tx[tx_size + tx_prefix_size];
  sprintf(signed_tx, "%s%s", tx_type_prefix_str, txn);

  printf("EIP-1559 Signed transaction is:\n%s\n", signed_tx);

  free(txn);
  return 0;
}
