#ifndef ETHC_MP_H
#define ETHC_MP_H

#include <tommath.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"

ETH_OP eth_mp_from_str(mp_int *dest, mp_int *scale, char *strint, int len);
ETH_OP eth_mp_to_str(char *dest, mp_int *mpint, int decimals);

#endif
