#include <ethc/bloom.h>
#include <stdio.h>
#include <tap.h>

static const uint8_t address[20] = {0x00, 0x00, 0x0d, 0x42, 0xe4, 0x30, 0x6c,
                       0x20, 0x97, 0xb2, 0xbd, 0x81, 0x7e, 0xfa,
                       0x00, 0xde, 0xc9, 0x69, 0x03, 0xf1};
static const uint8_t topic[32] = {0x9d, 0xf8, 0xc6, 0x9d, 0xd1, 0xa5, 0x75, 0x25,
                     0x65, 0x2a, 0x81, 0x43, 0x67, 0x94, 0x9d, 0xa6,
                     0x13, 0xf0, 0x8f, 0x6c, 0xc4, 0xd2, 0x75, 0xed,
                     0x8b, 0x47, 0x96, 0x40, 0xc5, 0xc2, 0x67, 0x59};

void test_eth_bloom_copy(void) {
  struct eth_bloom bloom1, bloom2;

  eth_bloom_from_bytes(&bloom1, address, 20);
  eth_bloom_create(&bloom2);

  eth_bloom_copy(&bloom2, &bloom1);

  ok(eth_bloom_and(&bloom1, &bloom2));
}

void test_eth_bloom_and(void) {
  struct eth_bloom bloom1, bloom2;

  eth_bloom_from_bytes(&bloom1, address, 20);
  eth_bloom_from_bytes(&bloom2, address, 20);

  ok(eth_bloom_and(&bloom1, &bloom2) == 1);
}

void test_eth_bloom_or(void) {
  struct eth_bloom bloom1, bloom2, bloom3;

  eth_bloom_from_bytes(&bloom1, address, 20);
  eth_bloom_from_bytes(&bloom2, topic, 32);

  eth_bloom_or(&bloom3, &bloom1);
  eth_bloom_or(&bloom3, &bloom2);

  ok(eth_bloom_and(&bloom3, &bloom1) == 1);
  ok(eth_bloom_and(&bloom3, &bloom2) == 1);
}
