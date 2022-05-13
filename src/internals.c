#include <ethc/internals.h>
#include <stdio.h>

int ethc_rand(uint8_t *bytes, size_t len) {
  FILE *fd;
  int r;
  ETHC_RETURN_IF_FALSE(bytes != NULL, ETHC_FAIL);

  fd = fopen("/dev/urandom", "r");
  ETHC_RETURN_IF_FALSE(fd >= 0, ETHC_FAIL);

  r = fread(bytes, 1, len, fd);
  ETHC_RETURN_IF_FALSE(r > 0, ETHC_FAIL);

  fclose(fd);
  return ETHC_SUCCESS;
}
