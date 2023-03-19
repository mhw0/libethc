#include "test.h"
#include <tap.h>
#include <string.h>
#include <ethc/rlp.h>

void test_eth_rlp_uint8(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  uint8_t ed1 = 0x70, ed2 = 0xff;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_uint8(&erlp, &ed1) == 1);
    ok(eth_rlp_uint8(&erlp, &ed2) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *ehex;
  ok(eth_rlp_to_hex(&ehex, &erlp) > 0);
  ok(strncmp(ehex, "c37081ff", strlen(ehex)) == 0);
  free(ehex);
  ok(eth_rlp_free(&erlp) == 1);


  struct eth_rlp drlp;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);

  char *dhex = "c381ca0f";
  ok(eth_rlp_from_hex(&drlp, dhex, -1) == 1);

  uint8_t dd1=0, dd2=0;
  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_uint8(&drlp, &dd1) == 1);
    ok(eth_rlp_uint8(&drlp, &dd2) == 1);
    ok(eth_rlp_uint8(&drlp, &dd2) == -1);
  ok(eth_rlp_list_end(&drlp) == 1);
  ok(dd1 == 0xca);
  ok(dd2 == 0x0f);
  ok(eth_rlp_free(&drlp) == 1);
}

void test_eth_rlp_uint16(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  uint16_t ed1=0xffaa, ed2=0x0001;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_uint16(&erlp, &ed1) == 1);
    ok(eth_rlp_uint16(&erlp, &ed2) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *ehex;
  ok(eth_rlp_to_hex(&ehex, &erlp) > 0);
  ok(strncmp(ehex, "c682ffaa820001", strlen(ehex)) == 0);
  ok(eth_rlp_free(&erlp) == 1);
  free(ehex);


  struct eth_rlp drlp;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);
  char *dhex = "c682ccfa820003";
  ok(eth_rlp_from_hex(&drlp, dhex, -1) == 1);

  uint16_t dd1=0, dd2=0;
  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_uint16(&drlp, &dd1) == 1);
    ok(eth_rlp_uint16(&drlp, &dd2) == 1);
    ok(eth_rlp_uint16(&drlp, &dd2) == -1);
  ok(eth_rlp_list_end(&drlp) == 1);
  ok(dd1 == 0xccfa);
  ok(dd2 == 0x0003);
  ok(eth_rlp_free(&drlp) == 1);
}

void test_eth_rlp_uint32(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  uint32_t ed1=0x751aa49d, ed2=0x0000000a;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_uint32(&erlp, &ed1) == 1);
    ok(eth_rlp_uint32(&erlp, &ed2) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *ehex;
  ok(eth_rlp_to_hex(&ehex, &erlp) > 0);
  ok(strncmp(ehex, "ca84751aa49d840000000a", strlen(ehex)) == 0);
  ok(eth_rlp_free(&erlp) == 1);
  free(ehex);

  struct eth_rlp drlp;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);
  char *dhex = "ca84928ca2cc840000000c";
  ok(eth_rlp_from_hex(&drlp, dhex, -1) == 1);

  uint32_t dd1=0, dd2=0;
  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_uint32(&drlp, &dd1) == 1);
    ok(eth_rlp_uint32(&drlp, &dd2) == 1);
    ok(eth_rlp_uint32(&drlp, &dd2) == -1);
  ok(eth_rlp_list_end(&drlp) == 1);
  ok(dd1 == 0x928ca2cc);
  ok(dd2 == 0x0000000c);
  ok(eth_rlp_free(&drlp) == 1);
}

void test_eth_rlp_uint64(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  uint64_t ed1=0x751aa49d, ed2=0xa6e7a8734fa234ad;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_uint64(&erlp, &ed1) == 1);
    ok(eth_rlp_uint64(&erlp, &ed2) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *ehex;
  ok(eth_rlp_to_hex(&ehex, &erlp) > 0);
  ok(strncmp(ehex, "d28800000000751aa49d88a6e7a8734fa234ad", strlen(ehex)) == 0);
  ok(eth_rlp_free(&erlp) == 1);
  free(ehex);

  struct eth_rlp drlp;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);
  char *dhex = "d2880000000000000239886aada0ec555b5cf4";
  ok(eth_rlp_from_hex(&drlp, dhex, -1) == 1);

  uint64_t dd1=0, dd2=0;
  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_uint64(&drlp, &dd1) == 1);
    ok(eth_rlp_uint64(&drlp, &dd2) == 1);
    ok(eth_rlp_uint64(&drlp, &dd2) == -1);
  ok(eth_rlp_list_end(&drlp) == 1);
  ok(dd1 == 0x0000000000000239);
  ok(dd2 == 0x6aada0ec555b5cf4);
  ok(eth_rlp_free(&drlp) == 1);
}

void test_eth_rlp_bytes(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  size_t eblen = 3;
  uint8_t *ebytes = (uint8_t*)malloc(eblen);
  ebytes[0] = 0xff;
  ebytes[1] = 0xc8;
  ebytes[2] = 0x1b;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_bytes(&erlp, &ebytes, &eblen) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);
  free(ebytes);

  char *ehex;
  ok(eth_rlp_to_hex(&ehex, &erlp) > 0);
  ok(strncmp(ehex, "c483ffc81b", strlen(ehex)) == 0);
  free(ehex);
  ok(eth_rlp_free(&erlp) == 1);

  struct eth_rlp drlp;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);
  char *dhex = "c484ba00acc4";
  ok(eth_rlp_from_hex(&drlp, dhex, -1) == 1);
  uint8_t *dbytes = NULL;
  size_t dblen = 0;

  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_bytes(&drlp, &dbytes, &dblen) == 1);
  ok(eth_rlp_list_end(&drlp) == 1);

  ok(dblen == 4);
  // TODO: compare it using "cmp_mem"
  ok(dbytes[0] == 0xba);
  ok(dbytes[1] == 0x00);
  ok(dbytes[2] == 0xac);
  ok(dbytes[3] == 0xc4);
  free(dbytes);
}

void test_eth_rlp_hex(void) {
  struct eth_rlp erlp;
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);

  char *ehex = "0x78d80a80ec11105a";
  int ehex_size = -1;
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_hex(&erlp, &ehex, &ehex_size) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *rhex = NULL;
  int rsize = eth_rlp_to_hex(&rhex, &erlp);
  is(rhex, "c98878d80a80ec11105a");
  ok(rsize == 20);

  struct eth_rlp drlp;
  char *dhex = "c9881acd13d3e50bd6e0";
  int dsize = 20;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE));
  ok(eth_rlp_from_hex(&drlp, dhex, -1));

  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_hex(&drlp, &dhex, &dsize) == 1);
  ok(eth_rlp_list_end(&drlp) == 1);
  is(dhex, "1acd13d3e50bd6e0");
  ok(dsize == 16);
}

void test_eth_rlp_address(void) {
  struct eth_rlp erlp;
  char *eaddr = "0x86C4dDdd08F8153E50247eaB59e500c043F99BfF";
  ok(eth_rlp_init(&erlp, ETH_RLP_ENCODE) == 1);
  ok(eth_rlp_list(&erlp) == 1);
    ok(eth_rlp_address(&erlp, &eaddr) == 1);
  ok(eth_rlp_list_end(&erlp) == 1);

  char *eaddrrlp;
  ok(eth_rlp_to_hex(&eaddrrlp, &erlp) > 0);
  is(eaddrrlp, "d59486c4dddd08f8153e50247eab59e500c043f99bff");
  ok(eth_rlp_free(&erlp) == 1);

  struct eth_rlp drlp;
  char *daddrrlp = "d59486c4dddd08f8153e50247eab59e500c043f99bff", *daddr;
  ok(eth_rlp_init(&drlp, ETH_RLP_DECODE) == 1);
  ok(eth_rlp_from_hex(&drlp, daddrrlp, -1) == 1);

  ok(eth_rlp_list(&drlp) == 1);
    ok(eth_rlp_address(&drlp, &daddr) == 1);
  ok(eth_rlp_list_end(&drlp) == 1);
  is(daddr, "86c4dddd08f8153e50247eab59e500c043f99bff");
  ok(eth_rlp_free(&drlp) == 1);
}
