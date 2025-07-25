#include <ethc/unit.h>
#include <ethc/unit.h>
#include <string.h>

ETH_OP eth_unit_convert(mp_int *dest, int *decimals, mp_int *amount, mp_int *scale, const char *from, const char *to) {
  mp_int fromint, toint, tmpint;

  if (dest == NULL || amount == NULL || from == NULL || to == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (mp_init_multi(&fromint, &toint, &tmpint, NULL) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  if (mp_read_radix(&fromint, from, 10) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  if (mp_read_radix(&toint, to, 10) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  // if "from unit" is less than "to unit", scale the numberator by 10^18
  if (mp_cmp(&fromint, &toint) == MP_LT) {
    if (mp_read_radix(&tmpint, "1000000000000000000", 10) != MP_OKAY)
      return ETH_ERR_UNKNOWN;

    if (mp_mul(amount, &tmpint, amount) != MP_OKAY)
      return ETH_ERR_UNKNOWN;

    *decimals = 18;
  } else {
    *decimals = 0;
  }

  // calculate the K = (amount * from multiplier)
  if (mp_mul(amount, &fromint, dest) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  // calculate the L = (to multiplier * scale factor)
  if (mp_mul(scale, &toint, &tmpint) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  // calculate K / L
  if (mp_div(dest, &tmpint, dest, NULL) != MP_OKAY)
    return ETH_ERR_UNKNOWN;

  mp_clear_multi(&fromint, &toint, &tmpint, NULL);

  return ETH_OK;
}
