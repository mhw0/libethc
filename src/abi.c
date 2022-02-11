#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int eth_abi_encode_event(char *dest, const char *event, int len) {
  uint8_t keccak[32];

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(event != NULL, ETHC_FAIL);

  if (len < 0)
    len = (int)strlen(event);

  if (!eth_keccak256((uint8_t *)event, len, keccak))
    return ETHC_FAIL;

  return eth_hex_from_bytes(dest, keccak, 32);
}

int eth_abi_encode_func(char *dest, const char *func, int len) {
  uint8_t keccak[32];

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(func != NULL, ETHC_FAIL);

  if (len < 0)
    len = (int)strlen(func);

  if (!eth_keccak256((uint8_t *)func, len, keccak))
    return ETHC_FAIL;

  return eth_hex_from_bytes(dest, keccak, 4);
}

int eth_abi_encode_bytes(char *dest, const uint8_t *bytes, uint8_t size) {
  char tmp[size * 2 + 1];

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  ETHC_RETURN_IF_FALSE(size > 0 && size < 32, ETHC_FAIL);

  if (!eth_hex_from_bytes(tmp, bytes, size))
    return ETHC_FAIL;

  return eth_hex_pad_right(dest, tmp, -1, 64);
}

char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len) {
  char tmp[len * 2 + 1], *buff;
  int width;
  mpz_t j;

  ETHC_RETURN_IF_FALSE(bytes != NULL, NULL);

  if (!eth_hex_from_bytes(tmp, bytes, len))
    return NULL;

  mpz_init_set_ui(j, len);

  width = (int)((64 * (ceil((len * 2) / 64.0))));

  buff = malloc(64 + width + 1);
  ETHC_RETURN_IF_FALSE(buff != NULL, NULL);

  gmp_sprintf(buff, "%Z064x", j);

  if (!eth_hex_pad_right(buff + 64, tmp, len * 2, width))
    return NULL;

  mpz_clear(j);
  return buff;
}

char *eth_abi_encode_stringd(const char *str, int len) {
  if (len < 0)
    len = (int)strlen(str);

  return eth_abi_encode_bytesd((uint8_t *)str, len);
}

int eth_abi_encode_int(char *dest, const char *intstr, uint16_t nbits) {
  mpz_t j, k, l, m;
  char tmp[64 + 1];

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(intstr != NULL, ETHC_FAIL);

  ETHC_RETURN_IF_FALSE(nbits != 0 && nbits % 8 == 0 && nbits <= 256, ETHC_FAIL);

  mpz_init(l);
  mpz_init(m);
  mpz_init_set_str(j, intstr, 0);
  mpz_init_set_ui(k, 1);
  mpz_mul_2exp(l, k, nbits - 1);

  if (mpz_sgn(j) < 0) {
    mpz_neg(m, j);

    if (mpz_cmp(m, l) > 0)
      return ETHC_FAIL;

    mpz_mul_2exp(l, k, 256);
    mpz_add(j, l, j);

    gmp_sprintf(dest, "%Zx", j);

    mpz_clears(j, k, l, m, NULL);
    return ETHC_SUCCESS;
  }

  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return ETHC_FAIL;

  gmp_sprintf(tmp, "%Zx", j);
  mpz_clears(j, k, l, m, NULL);
  return eth_hex_pad_left(dest, tmp, -1, 64);
}

int eth_abi_encode_uint(char *dest, const char *intstr, uint16_t nbits) {
  mpz_t j, k, l;
  char tmp[64 + 1];

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(intstr != NULL, ETHC_FAIL);

  ETHC_RETURN_IF_FALSE(nbits != 0 && nbits % 8 == 0 && nbits <= 256, ETHC_FAIL);

  mpz_init_set_str(j, intstr, 0);

  if (mpz_sgn(j) < 0)
    return ETHC_FAIL;

  mpz_init_set_ui(k, 1);
  mpz_init(l);
  mpz_mul_2exp(l, k, nbits);
  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return ETHC_FAIL;

  gmp_sprintf(tmp, "%Zx", j);

  mpz_clears(j, k, l, NULL);

  return eth_hex_pad_left(dest, tmp, -1, 64);
}

int eth_abi_encode_bool(char *dest, int b) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);

  ETHC_RETURN_IF_FALSE(b == 0 || b == 1, ETHC_FAIL);

  return eth_hex_pad_left(dest, b == 0 ? "0" : "1", 1, 64);
}

int eth_abi_encode_address(char *dest, char *addr) {
  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(addr != NULL, ETHC_FAIL);

  if (!eth_is_address(addr))
    return ETHC_FAIL;

  addr += 2;

  return eth_hex_pad_left(dest, addr, 40, 64);
}
