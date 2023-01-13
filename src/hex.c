#include <stdlib.h>
#include <string.h>
#include <ethc/hex.h>
#include <ethc/internals.h>

int eth_is_hex(const char *str, int len) {
  int i;
  char ch;

  if (str == NULL || len == 0)
    return -1;

  if (len < 0)
    len = (int)strlen(str);

  if (ethc_strncasecmp(str, "0x", 2) == 0) {
    if (len == 2)
      return 0;

    str += 2;
    len -= 2;
  }

  for (i = 0; i < len; i++) {
    ch = str[i];
    if (((ch < 'A' || ch > 'F') && (ch < 'a' || ch > 'f')) &&
        (ch < '0' || ch > '9'))
      return 0;
  }

  return 1;
}

int eth_hex_pad_left(char *dest, const char *str, int len, size_t width) {
  size_t zeros = 0;

  if (len < 0)
    len = (int)strlen(str);

  if (dest == NULL || str == NULL || len == 0 || len > width)
    return -1;

  if (eth_is_hex(str, len) != 1)
    return -1;

  zeros = width - len;
  memset(dest, '0', zeros);
  memcpy(dest + zeros, str, len);
  dest[width] = '\0';

  return 1;
}

int eth_hex_pad_right(char *dest, const char *str, int len, size_t width) {
  size_t zeros = 0;

  if (len < 0)
    len = (int)strlen(str);

  if (str == NULL || len == 0 || len > width)
    return -1;

  if (dest == NULL || str == NULL || len == 0 || len > width)
    return -1;

  if (eth_is_hex(str, len) == 0)
    return -1;

  strncpy(dest, str, len);
  zeros = width - len;
  memset(dest + len, '0', zeros);
  dest[width] = '\0';

  return 1;
}

int eth_hex_from_bytes(char **dest, const uint8_t *bytes, size_t len) {
  char *buf;
  size_t i = 0, j = 0;

  if (dest == NULL || bytes == NULL)
    return -1;

  buf = (char*)malloc((len * 2) + 1);
  if (buf == NULL)
    return -1;

  while (i < len) {
    buf[j++] = ethc_hexchar((bytes[i] >> 4) & 0xf);
    buf[j++] = ethc_hexchar(bytes[i] & 0xf);
    i++;
  }

  buf[j] = '\0';
  *dest = buf;
  return j;
};

int eth_hex_to_bytes(uint8_t **dest, const char *hex, int len) {
  uint8_t *buf;
  size_t i, bsize, k=0;

  if (dest == NULL || hex == NULL)
    return -1;

  if (len < 0)
    len = (int)strlen(hex);

  if (eth_is_hex(hex, len) <= 0)
    return -1;

  if (ethc_strncasecmp(hex, "0x", 2) == 0) {
    if (len == 2)
      return -1;

    hex += 2;
    len -= 2;
  }

  bsize = (len % 2 == 0 ? len : len + 1) / 2;
  buf = (uint8_t*)malloc(bsize);
  if (buf == NULL)
    return -1;

  if (len % 2 != 0) {
    buf[k++] = ethc_hexcharb(hex[0]);
    hex += 1;
    len -= 1;
  }

  for (i = 0; i < len ; i+= 2)
    buf[k++] = (ethc_hexcharb((hex[i])) << 4)
        | ethc_hexcharb((hex[i + 1]));

  *dest = buf;
  return k;
}
