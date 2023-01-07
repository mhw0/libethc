#include <ethc/internals.h>
#include <stdio.h>

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
