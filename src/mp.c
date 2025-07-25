#include <ethc/mp.h>
#include <string.h>

ETH_OP eth_mp_from_str(mp_int *dest, mp_int *scale, char *strint, int len) {
  int i = 0, found = 0, digit = 0, decimals = 0;
  char digits[256] = {0};

  if (len == -1)
    len = strlen(strint);

  for (i = 0; i < len; i++) {
    if (strint[i] == '.') {
      found = 1;
      continue;
    }

    if (found) (decimals)++;
    if (strint[i] >= '0' && strint[i] <= '9')
      digits[digit++] = strint[i];
  }

  digits[digit++] = '\0';

  if (mp_read_radix(dest, digits, 10) != MP_OKAY)
    return ETH_ERR_INVALID_ARGS;

  mp_set(scale, 1);
  for (int i = 0; i < decimals; i++)
    if (mp_mul_d(scale, 10, scale) != MP_OKAY)
      return ETH_ERR_INVALID_ARGS;

  return ETH_OK;
}

ETH_OP eth_mp_to_str(char *dest, mp_int *mpint, int decimals) {
  char raw[512], *ptr = NULL;
  int len = 0, zeros = 0, i = 0, intlen = 0;
  size_t maxlen = 512, written = 0;

  if (mp_to_radix(mpint, raw, maxlen, &written, 10) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  if (decimals == 0) {
    strncpy(dest, raw, written);
    return ETH_OK;
  }

  len = strlen(raw);

  if (len <= decimals) {
    zeros = decimals - len;
    ptr = dest;
    *ptr++ = '0';
    *ptr++ = '.';

    for(i = 0; i < zeros; i++) *ptr++ = '0';
    strcpy(ptr, raw);

    return ETH_OK;
  }

  intlen = len - decimals;
  strncpy(dest, raw, intlen);
  dest[intlen] = '.';
  strcpy(dest + intlen + 1, raw + intlen);

  return ETH_OK;
}
