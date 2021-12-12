#include <ethc/hex.h>
#include <string.h> // strcpy

#define HEXCHARS "0123456789abcdef"

int eth_is_hexstr(const char *hexstr, size_t len, int prefix) {
  size_t i;

  len = len == -1 ? strlen(hexstr) : len;

  if (hexstr == NULL || len == 0)
    return 0;

  if (prefix &&
      (len < 3 || hexstr[0] != '0' && (hexstr[1] != 'x' || hexstr[1] != 'X')))
    return 0;

  if (prefix) {
    hexstr += 2; // skip the first two characters
    len -= 2;
  }

  for (i = 0; i < len; i++) {
    char ch = hexstr[i];
    if (((ch < 'A' || ch > 'F') && (ch < 'a' || ch > 'f')) &&
        (ch < '0' || ch > '9'))
      return 0;
  }

  return 1;
}

char *eth_hexstr_pad_left(const char *hexstr, size_t hexstr_len,
                          size_t pad_len) {
  char *new_hexstr;

  if (!hexstr || !eth_is_hexstr(hexstr, hexstr_len, 1))
    return NULL;

  new_hexstr = malloc(hexstr_len + pad_len + 1);

  if (!new_hexstr)
    return NULL;

  memcpy(new_hexstr, hexstr, 2);
  memset(new_hexstr + 2, '0', pad_len);
  memcpy(new_hexstr + 2 + pad_len, hexstr + 2, hexstr_len - 2);
  new_hexstr[hexstr_len + pad_len] = '\0';

  return new_hexstr;
}

char *eth_hexstr_pad_right(const char *hexstr, size_t hexstr_len,
                           size_t pad_len) {
  char *new_hexstr;

  if (!hexstr || !eth_is_hexstr(hexstr, hexstr_len, 1))
    return NULL;

  new_hexstr = malloc(hexstr_len + pad_len + 1);

  if (!new_hexstr)
    return NULL;

  memcpy(new_hexstr, hexstr, hexstr_len);
  memset(new_hexstr + hexstr_len, '0', pad_len);
  new_hexstr[hexstr_len + pad_len] = '\0';

  return new_hexstr;
}
