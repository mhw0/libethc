#include <ethc/hex.h>
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

  if (len < 0)
    len = (int)strlen(str);

  ETHC_RETURN_IF_FALSE(str != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(len != 0, ETHC_FAIL);

  if (strncasecmp(str, "0x", 2) == 0)
    prefix = 1;

  if(strict && !prefix)
    return ETHC_FAIL;

  if (prefix) {
    str += 2;
    len -= 2;
  }

  for (i = 0; i < len; i++) {
    char ch = str[i];
    if (((ch < 'A' || ch > 'F') && (ch < 'a' || ch > 'f')) &&
        (ch < '0' || ch > '9'))
      return ETHC_FAIL;
  }

  return ETHC_SUCCESS;
}

int eth_hex_pad_left(char *dest, const char *str, int len, size_t width) {
  size_t fill_len = 0;

  if (len < 0)
    len = (int)strlen(str);

  ETHC_RETURN_IF_FALSE(str != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(len != 0, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(len < width, ETHC_FAIL);

  if (!eth_is_hex(str, len, 0))
    return ETHC_FAIL;

  fill_len = width - len;
  memset(dest, '0', fill_len);
  memcpy(dest + fill_len, str, len);
  dest[width] = '\0';

  return ETHC_SUCCESS;
}

int eth_hex_pad_right(char *dest, const char *str, int len, size_t width) {
  size_t fill_len = 0;

  if (len < 0)
    len = (int)strlen(str);

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(str != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(len < width, ETHC_FAIL);

  if (!eth_is_hex(str, len, 0))
    return ETHC_FAIL;

  strncpy(dest, str, len);
  fill_len = width - len;
  memset(dest + len, '0', fill_len);
  dest[width] = '\0';

  return ETHC_SUCCESS;
}

int eth_hex_from_bytes(char *dest, const uint8_t *bytes, size_t len) {
  size_t i = 0, j = 0;

  ETHC_RETURN_IF_FALSE(dest != NULL, ETHC_FAIL);
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  while (i < len) {
    dest[j++] = HEXCHARS[((bytes[i] & 0xFF) >> 4) & 0xF];
    dest[j++] = HEXCHARS[(bytes[i] & 0xFF) & 0xF];
    i++;
  }

  dest[j] = '\0';

  return ETHC_SUCCESS;
};
