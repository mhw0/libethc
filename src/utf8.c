#include <ethc/utf8.h>
#include <string.h>

size_t eth_utf8_strlen(const char *str, int len) {
  size_t i = 0;

  const unsigned char *ptr = (unsigned char *)str;

  len == -1 ? strlen(str) : len;

  while(*ptr != 0x00) {
    if((*ptr & 0x80) == 0x00) { // 0xxxxxxx
      ptr++;
    } else if ((*ptr & 0xE0) == 0xC0) { // 110xxxxx
      ptr += 2;
    } else if ((*ptr & 0xF0) == 0xE0) { // 1110xxxx
      ptr += 3;
    } else if ((*ptr & 0xF8) == 0xF0) { // 11110xxx
      ptr += 4;
    } else {
      return 0;
    }
    i++;
  }

  return i;
}
