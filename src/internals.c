#include <ethc/internals.h>
#include <stdio.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <strings.h>
#elif defined(_MSC_VER)
#define strncasecmp _strnicmp
#endif

int ethc_rand(uint8_t *bytes, size_t len) {
  FILE *fd;
  int r;

  if (bytes == NULL)
    return -1;

  fd = fopen("/dev/urandom", "r");
  if (fd < 0)
    return -1;

  r = fread(bytes, 1, len, fd);
  if (r <= 0)
    return -1;

  fclose(fd);
  return 1;
}

int ethc_strncasecmp(const char *s1, const char *s2, size_t len) {
  return strncasecmp(s1, s2, len);
}
