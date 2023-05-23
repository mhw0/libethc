#include <gmp.h>
#include <ethc/unit.h>

int eth_unit_convert(char **dest, const char *amount, const char *from, const char *to) {
  mpf_t j, k, l;
  char *buf, *fmt;

  if (dest == NULL || amount == NULL || from == NULL || to == NULL)
    return -1;

  if (mpf_init_set_str(j, amount, 10) == -1) {
    mpf_clear(j);
    return -1;
  }

  mpf_init_set_str(k, from, 10);
  mpf_init_set_str(l, to, 10);

  mpf_mul(j, j, k);
  mpf_div(j, j, l);

  fmt = mpf_integer_p(j) ? "%F.0f" : "%F.18f";
  if (gmp_asprintf(&buf, fmt, j) == -1) {
    mpf_clears(j, k, l, NULL);
    return -1;
  }

  mpf_clears(j, k, l, NULL);
  *dest = buf;
  return 1;
}
