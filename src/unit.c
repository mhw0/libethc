#include <ethc/unit.h>
#include <gmp.h>

char *eth_unit_convert(const char *amount, enum eth_unit from,
                       enum eth_unit to) {
  mpf_t j, k, l;
  char *buff, *fmt;

  if (amount == NULL)
    return NULL;

  if (mpf_init_set_str(j, amount, 10) == -1) {
    mpf_clear(j);
    return NULL;
  }

  mpf_init_set_str(k, ETH_UNIT_AMOUNTS[from], 10);
  mpf_init_set_str(l, ETH_UNIT_AMOUNTS[to], 10);

  mpf_mul(j, j, k);
  mpf_div(j, j, l);

  fmt = mpf_integer_p(j) ? "%F.0f" : "%F.18f";
  if (gmp_asprintf(&buff, fmt, j) == -1) {
    mpf_clears(j, k, l, NULL);
    return NULL;
  }

  mpf_clears(j, k, l, NULL);
  return buff;
}
