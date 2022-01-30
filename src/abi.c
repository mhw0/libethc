#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>
#include <gmp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int eth_abi_encode_event(char *rstr, const char *event, int len) {
  uint8_t keccak[32];

  if (!rstr || !event)
    return 0;

  if (len < 0)
    len = (int)strlen(event);

  if (eth_keccak256((uint8_t *)event, len, keccak) == 0)
    return 0;

  return eth_hex_from_bytes(rstr, keccak, 32);
}

int eth_abi_encode_func(char *rstr, const char *func, int len) {
  uint8_t keccak[32];

  if (!rstr || !func)
    return 0;

  if (len < 0)
    len = (int)strlen(func);

  if (eth_keccak256((uint8_t *)func, len, keccak) == 0)
    return 0;

  return eth_hex_from_bytes(rstr, keccak, 4);
}

int eth_abi_encode_bytes(char *rstr, const uint8_t *bytes, uint8_t size) {
  char tmp[size * 2 + 1];

  if (size <= 0 || size > 32)
    return 0;

  if (!bytes || !eth_hex_from_bytes(tmp, bytes, size))
    return 0;

  return eth_hex_pad_right(rstr, tmp, -1, 64);
}

char *eth_abi_encode_bytesd(const uint8_t *bytes, size_t len) {
  char tmp[len * 2 + 1], *buff;
  int width;

  if (!bytes || !eth_hex_from_bytes(tmp, bytes, len))
    return NULL;

  width = (int)(64 * (ceil((len * 2) / 64.0)));

  buff = malloc(width);

  if (!buff || !eth_hex_pad_right(buff, tmp, -1, width))
    return NULL;

  return buff;
}

int eth_abi_encode_int(char *rstr, const char *str, uint16_t nbits) {
  mpz_t j, k, l, m;
  char tmp[64 + 1];

  if (!rstr || !str)
    return 0;

  if (nbits == 0 || nbits % 8 != 0 || nbits > 256)
    return 0;

  mpz_init(l);
  mpz_init(m);
  mpz_init_set_str(j, str, 0);
  mpz_init_set_ui(k, 1);
  mpz_mul_2exp(l, k, nbits - 1);

  if (mpz_sgn(j) < 0) {
    mpz_neg(m, j);

    if (mpz_cmp(m, l) > 0)
      return 0;

    mpz_mul_2exp(l, k, 256);
    mpz_add(j, l, j);

    gmp_sprintf(rstr, "%Zx", j);

    mpz_clears(j, k, l, m, NULL);
    return 1;
  }

  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return 0;

  gmp_sprintf(tmp, "%Zx", j);
  mpz_clears(j, k, l, m, NULL);
  return eth_hex_pad_left(rstr, tmp, -1, 64);
}

int eth_abi_encode_uint(char *rstr, const char *str, uint16_t nbits) {
  mpz_t j, k, l;
  char tmp[64 + 1];

  if (!rstr || !str)
    return 0;

  if (nbits == 0 || nbits % 8 != 0 || nbits > 256)
    return 0;

  mpz_init_set_str(j, str, 0);

  if (mpz_sgn(j) < 0)
    return 0;

  mpz_init_set_ui(k, 1);
  mpz_init(l);
  mpz_mul_2exp(l, k, nbits);
  mpz_sub(l, l, k);

  if (mpz_cmp(j, l) > 0)
    return 0;

  gmp_sprintf(tmp, "%Zx", j);

  mpz_clears(j, k, l, NULL);

  return eth_hex_pad_left(rstr, tmp, -1, 64);
}

int eth_abi_encode_bool(char *rstr, int b) {
  if (!rstr)
    return 0;

  if (b != 0 && b != 1)
    return 0;

  return eth_hex_pad_left(rstr, b == 0 ? "0" : "1", 1, 64);
}

int eth_abi_encode_address(char *rstr, char *str) {
  if (!rstr || !str)
    return 0;

  if (!eth_is_address(str))
    return 0;

  str += 2;

  return eth_hex_pad_left(rstr, str, 40, 64);
}
