#include <ethc/hex.h>
#include <stdlib.h>
#include <strings.h>

#if defined(_MSC_VER)
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#define HEXCHARS "0123456789abcdef"

int eth_is_hexstr(const char *str, size_t len, int strict) {
  int has_prefix = 0;
  size_t i;

  len = len == -1 ? strlen(str) : len;

  if (!str || len == 0)
    return 0;

  if (strncasecmp(str, "0x", 2) == 0)
    has_prefix = 1;

  if (strict && !has_prefix)
    return 0;

  if (has_prefix) {
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

char *eth_hexstr_pad_left(const char *str, size_t str_len, size_t width) {
  char *new_str = NULL;
  int has_prefix = 0;
  size_t fill_len = 0;

  str_len = str_len == -1 ? strlen(str) : str_len;

  // should we return `str` or copy of `str` when `str_len` > `width` ?
  if (!str || str_len > width || !eth_is_hexstr(str, str_len, 0))
    return NULL;

  if (strncasecmp(str, "0x", 2) == 0)
    has_prefix = 1;

  new_str = malloc(has_prefix ? (width + 2 + 1) : (width + 1));
  if (!new_str)
    return NULL;

  if (has_prefix) {
    strncpy(new_str, str, 2);
    fill_len = width - (str_len - 2);
    memset(new_str + 2, '0', fill_len);
    memcpy(new_str + 2 + fill_len, str + 2, str_len - 2);
    new_str[width + 2] = '\0';
    return new_str;
  }

  fill_len = width - str_len;
  memset(new_str, '0', fill_len);
  memcpy(new_str + fill_len, str, str_len);
  new_str[width] = '\0';
  return new_str;
}

char *eth_hexstr_pad_right(const char *str, size_t str_len, size_t width) {
  char *new_str = NULL;
  int has_prefix = 0;
  size_t fill_len = 0;

  str_len = str_len == -1 ? strlen(str) : str_len;

  if (!str || str_len > width || !eth_is_hexstr(str, str_len, 0))
    return NULL;

  if (strncasecmp(str, "0x", 2) == 0) {
    has_prefix = 1;
  }

  new_str = malloc(has_prefix ? (width + 2 + 1) : (width + 1));
  if (!new_str)
    return NULL;

  strncpy(new_str, str, str_len);

  if (has_prefix) {
    fill_len = width - (str_len - 2);
    memset(new_str + str_len, '0', fill_len);
    new_str[width + 2] = '\0';
    return new_str;
  }

  fill_len = width - str_len;
  memset(new_str + str_len, '0', fill_len);
  new_str[width] = '\0';

  return new_str;
}

int eth_hexstr_from_bytes(const uint8_t *bytes, size_t len, int prefix, char *hexstr) {
  size_t i = 0, j = 0;

  if(!bytes || !hexstr || len < 1)
    return 0;

  if(prefix) {
    hexstr[0] = '0';
    hexstr[1] = 'x';
    j += 2;
  }

  while(i < len) {
    hexstr[j++] = HEXCHARS[((bytes[i] & 0xFF) >> 4) & 0xF];
    hexstr[j++] = HEXCHARS[(bytes[i] & 0xFF) & 0xF];
    i++;
  }

  hexstr[j] = '\0';

  return 1;
};

