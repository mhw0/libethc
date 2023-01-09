#include <ethc/hex.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <strings.h>
#elif defined(_MSC_VER)
#define strncasecmp _strnicmp
#endif

#define HEXCHARS "0123456789abcdef"

int eth_is_hex(const char *str, int len, int strict) {
  int prefix = 0;
  size_t i;

  if (str == NULL || len == 0)
    return -1;

  if (len < 0)
    len = (int)strlen(str);

  if (strncasecmp(str, "0x", 2) == 0)
    prefix = 1;

  if (strict && !prefix)
    return 0;

  if (prefix) {
    str += 2;
    len -= 2;
  }

  for (i = 0; i < len; i++) {
    char ch = str[i];
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

  if (eth_is_hex(str, len, 0) != 1)
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

  if (eth_is_hex(str, len, 0) == 0)
    return -1;

  strncpy(dest, str, len);
  zeros = width - len;
  memset(dest + len, '0', zeros);
  dest[width] = '\0';

  return 1;
}

int eth_hex_from_bytes(char *dest, const uint8_t *bytes, size_t len) {
  size_t i = 0, j = 0;

  if (dest == NULL || bytes == NULL)
    return -1;

  while (i < len) {
    dest[j++] = HEXCHARS[((bytes[i] & 0xFF) >> 4) & 0xF];
    dest[j++] = HEXCHARS[(bytes[i] & 0xFF) & 0xF];
    i++;
  }

  dest[j] = '\0';
  return j;
};

int eth_hex_char_to_byte(char ch) {
  if (ch >= '0' && ch <= '9')
    return ch - '0';
  else if (ch >= 'a' && ch <= 'f')
    return ch - 'a' + 10;
  else if (ch >= 'A' && ch <= 'F')
    return ch - 'A' + 10;
  else
    return 0; // is this safe?
}

int eth_hex_to_bytes(uint8_t *dest, const char *hex, int len) {
  size_t i;

  if (dest == NULL || hex == NULL)
    return -1;

  if (len < 0)
    len = strlen(hex);

  if (eth_is_hex(hex, len, 0) == 0)
    return -1;

  if (len % 2 != 0) {
    char *buf = (char*)malloc(len + 1);
    eth_hex_pad_left(buf, hex, len, len + 1);
    hex = buf;
  }

  if (strncasecmp(hex, "0x", 2) == 0) {
    hex += 2;
    len -= 2;
  }

  for (i = 0; i < len; i += 2) {
    uint8_t hnib = eth_hex_char_to_byte((hex[i]) & 0xff);
    uint8_t lnib = eth_hex_char_to_byte((hex[i + 1]) & 0xff);
    *(dest++) = (hnib << 4) | lnib;
  }

  return len / 2;
}
