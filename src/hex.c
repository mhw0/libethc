#include <ethc/hex.h>
#include <string.h> // strncmp

int eth_is_hex_str(const char *hexstr, size_t len) {
  if (!hexstr || strncmp(hexstr, "0x", 2) != 0)
    return 0;

  for (size_t i = 2; i < len; i++) {
    char ch = hexstr[i];
    if (((ch < 'A' || ch > 'F') && (ch < 'a' || ch > 'f')) &&
        (ch < '0' || ch > '9'))
      return 0;
  }

  return 1;
}

char *eth_hex_pad_left(const char *hexstr, size_t hexstr_len, size_t pad_len) {
  char *new_hexstr;

  if (!hexstr || !eth_is_hex_str(hexstr, hexstr_len))
    return NULL;

  new_hexstr = malloc(hexstr_len + pad_len + 1);

  if(!new_hexstr)
    return NULL;

  memcpy(new_hexstr, hexstr, 2);
  memset(new_hexstr + 2, '0', pad_len);
  memcpy(new_hexstr + 2 + pad_len, hexstr + 2, hexstr_len - 2);
  new_hexstr[hexstr_len + pad_len] = '\0';

  return new_hexstr;
}

char *eth_hex_pad_right(const char *hexstr, size_t hexstr_len, size_t pad_len) {
  char *new_hexstr;

  if (!hexstr || !eth_is_hex_str(hexstr, hexstr_len))
    return NULL;

  new_hexstr = malloc(hexstr_len + pad_len + 1);

  if(!new_hexstr)
    return NULL;

  memcpy(new_hexstr, hexstr, hexstr_len);
  memset(new_hexstr + hexstr_len, '0', pad_len);
  new_hexstr[hexstr_len + pad_len] = '\0';

  return new_hexstr;
}
