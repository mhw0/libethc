#include <math.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

int eth_abi_encode_event(char *dest, const char *event, int len) {
  uint8_t keccak[32];

  if (dest == NULL || event == NULL)
    return -1;

  if (len < 0)
    len = (int)strlen(event);

  if (eth_keccak256(keccak, (uint8_t*)event, len) != 1)
    return -1;

  return eth_hex_from_bytes(dest, keccak, 32);
}

int eth_abi_encode_func(char *dest, const char *func, int len) {
  uint8_t keccak[32];

  if (dest == NULL || func == NULL)
    return -1;

  if (len < 0)
    len = (int)strlen(func);

  if (eth_keccak256(keccak, (uint8_t *)func, len) != 1)
    return -1;

  return eth_hex_from_bytes(dest, keccak, 4);
}

int eth_abi_encode_bytes(char *dest, const uint8_t *bytes, uint8_t size) {
  char tmp[size * 2 + 1];

  if (dest == NULL || bytes == NULL)
    return -1;

  if (size < 0 || size > 32)
    return - 1;

  if (eth_hex_from_bytes(tmp, bytes, size) != 1)
    return -1;

  return eth_hex_pad_right(dest, tmp, -1, 64);
}

char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len) {
  char tmp[len * 2 + 1], *buf;
  int width;
  mpz_t j;

  if (bytes == NULL)
    return NULL;

  if (eth_hex_from_bytes(tmp, bytes, len) != 1)
    return NULL;

  mpz_init_set_ui(j, len);

  width = (int)((64 * (ceil((len * 2) / 64.0))));

  buf = malloc(64 + width + 1);
  if (buf == NULL)
    return NULL;

  gmp_sprintf(buf, "%Z064x", j);

  if (eth_hex_pad_right(buf + 64, tmp, len * 2, width) == -1)
    return NULL;

  mpz_clear(j);
  return buf;
}

char *eth_abi_encode_stringd(const char *str, int len) {
  if (len < 0)
    len = (int)strlen(str);

  return eth_abi_encode_bytesd((uint8_t *)str, len);
}

int eth_abi_encode_int(char *dest, const char *intstr, uint16_t nbits) {
  mpz_t j, k, l, m;
  char tmp[64 + 1];

  if (dest == NULL || intstr == NULL)
    return -1;

  if (nbits == 0 || nbits % 8 != 0 || nbits > 256)
    return -1;

  mpz_init(l);
  mpz_init(m);
  mpz_init_set_str(j, intstr, 0);
  mpz_init_set_ui(k, 1);
  mpz_mul_2exp(l, k, nbits - 1);

  if (mpz_sgn(j) < 0) {
    mpz_neg(m, j);

    if (mpz_cmp(m, l) > 0)
      return -1;

    mpz_mul_2exp(l, k, 256);
    mpz_add(j, l, j);

    gmp_sprintf(dest, "%Zx", j);

    mpz_clears(j, k, l, m, NULL);
    return 1;
  }

  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return -1;

  gmp_sprintf(tmp, "%Zx", j);
  mpz_clears(j, k, l, m, NULL);
  return eth_hex_pad_left(dest, tmp, -1, 64);
}

int eth_abi_encode_uint(char *dest, const char *intstr, uint16_t nbits) {
  mpz_t j, k, l;
  char tmp[64 + 1];

  if (dest == NULL || intstr == NULL)
    return -1;

  if (nbits == 0 || nbits % 8 != 0 || nbits > 256)
    return -1;

  mpz_init_set_str(j, intstr, 0);

  if (mpz_sgn(j) < 0)
    return -1;

  mpz_init_set_ui(k, 1);
  mpz_init(l);
  mpz_mul_2exp(l, k, nbits);
  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return -1;

  gmp_sprintf(tmp, "%Zx", j);
  mpz_clears(j, k, l, NULL);
  return eth_hex_pad_left(dest, tmp, -1, 64);
}

int eth_abi_encode_bool(char *dest, int b) {
  if (dest == NULL)
    return -1;
  
  if (b != 0 && b != 1)
    return -1;

  return eth_hex_pad_left(dest, b == 0 ? "0" : "1", 1, 64);
}

int eth_abi_encode_address(char *dest, char *addr) {
  if (dest == NULL || addr == NULL)
    return -1;

  if (!eth_is_address(addr))
    return -1;

  addr += 2;

  return eth_hex_pad_left(dest, addr, 40, 64);
}
