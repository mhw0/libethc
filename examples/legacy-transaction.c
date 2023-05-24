#include <stdio.h>
#include <assert.h>
#include <ethc/account.h>
#include <ethc/ecdsa.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>

#define ok(ethcop) assert(ethcop >= 0)

int main(void) {
  struct eth_account account;
  struct eth_rlp rlp0, rlp1;
  struct eth_ecdsa_signature sign;
  uint8_t privkey[] = {0xdf, 0x57, 0x08, 0x9f, 0xeb, 0xba, 0xcf, 0x7b,
                       0xa0, 0xbc, 0x22, 0x7d, 0xaf, 0xbf, 0xfa, 0x9f,
                       0xc0, 0x8a, 0x93, 0xfd, 0xc6, 0x8e, 0x1e, 0x42,
                       0x41, 0x1a, 0x14, 0xef, 0xcf, 0x23, 0x65, 0x6e};

  uint8_t nonce=0x00, zero=0x00, keccak[32], *rlp0bytes, *r, *s;
  char *gasprice = "0x04a817c800", *gaslimit = "0x5208", *value = "0x0de0b6b3a7640000";
  char *toaddr = "0x3535353535353535353535353535353535353535", *txn;
  uint16_t chainid = 0x7a69, v;
  size_t rlp0len, rlp1len, siglen=32;

  ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE));

  ok(eth_rlp_array(&rlp0));                  // [
    ok(eth_rlp_uint8(&rlp0, &nonce));        //   0x00,
    ok(eth_rlp_hex(&rlp0, &gasprice, NULL)); //   0x04a817c800, 
    ok(eth_rlp_hex(&rlp0, &gaslimit, NULL)); //   0x5208,
    ok(eth_rlp_address(&rlp0, &toaddr));     //   0x3535353535353535353535353535353535353535,
    ok(eth_rlp_hex(&rlp0, &value, NULL));    //   0x0de0b6b3a7640000,
    ok(eth_rlp_uint8(&rlp0, &zero));         //   0x,
    ok(eth_rlp_uint16(&rlp0, &chainid));     //   0x7a69,
    ok(eth_rlp_uint8(&rlp0, &zero));         //   0x,
    ok(eth_rlp_uint8(&rlp0, &zero));         //   0x,
  ok(eth_rlp_array_end(&rlp0));              // ]

  ok(eth_rlp_to_bytes(&rlp0bytes, &rlp0len, &rlp0));
  ok(eth_rlp_free(&rlp0));

  // compute the keccak hash of the encoded rlp elements
  ok(eth_keccak256(keccak, rlp0bytes, rlp0len));
  free(rlp0bytes);

  // sign the transaction
  ok(eth_ecdsa_sign(&sign, privkey, keccak));

  // calculate v
  v = sign.recid + chainid * 2 + 35;
  r = sign.r;
  s = sign.s;

  ok(eth_rlp_init(&rlp1, ETH_RLP_ENCODE));

  ok(eth_rlp_array(&rlp1));
    ok(eth_rlp_uint8(&rlp1, &nonce));
    ok(eth_rlp_hex(&rlp1, &gasprice, NULL));
    ok(eth_rlp_hex(&rlp1, &gaslimit, NULL));
    ok(eth_rlp_address(&rlp1, &toaddr));
    ok(eth_rlp_hex(&rlp1, &value, NULL));
    ok(eth_rlp_uint8(&rlp1, &zero));
    ok(eth_rlp_uint16(&rlp1, &v));
    ok(eth_rlp_bytes(&rlp1, &r, &siglen));
    ok(eth_rlp_bytes(&rlp1, &s, &siglen));
  ok(eth_rlp_array_end(&rlp1));

  ok(eth_rlp_to_hex(&txn, &rlp1));
  ok(eth_rlp_free(&rlp1));

  printf("Signed transaction: \n%s\n", txn);
  free(txn);
  return 0;
}
