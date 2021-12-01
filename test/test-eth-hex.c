#include "test.h"
#include <ethc/hex.h>
#include <string.h>
#include <tap.h>

void test_eth_is_hex_str(void) {
  const char *inp0 = "";
  ok(eth_is_hex_str(inp0, strlen(inp0)) == 0);

  const char *inp1 = "0xgh";
  ok(eth_is_hex_str(inp1, strlen(inp1)) == 0);

  const char *inp2 = "0xffff00";
  ok(eth_is_hex_str(inp2, strlen(inp2)) == 1);

  const char *inp3 = "0xFFFF00";
  ok(eth_is_hex_str(inp3, strlen(inp3)) == 1);

  const char *inp4 = "0XFFFF00";
  ok(eth_is_hex_str(inp4, strlen(inp4)) == 0);
}

void test_eth_hex_pad_left(void) {
  const char *inp0 = "0xff";
  is(eth_hex_pad_left(inp0, strlen(inp0), 4), "0x0000ff");

  const char *inp1 = "0xgh";
  is(eth_hex_pad_left(inp1, strlen(inp1), 4), NULL);
}

void test_eth_hex_pad_right(void) {
  const char *inp0 = "0xff";
  is(eth_hex_pad_right(inp0, strlen(inp0), 4), "0xff0000");

  const char *inp1 = "0xgh";
  is(eth_hex_pad_right(inp1, strlen(inp1), 4), NULL);

  const char *inp2 = NULL;
  is(eth_hex_pad_right(inp2, 0, 4), NULL);
}
