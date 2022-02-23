#ifndef ETHC_UNIT_H
#define ETHC_UNIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"

#define ETH_UNIT_MAP(XX)                                                       \
  XX(WEI, "1")                                                                 \
  XX(KWEI, "1000")                                                             \
  XX(BABBAGE, "1000")                                                          \
  XX(FEMTOETHER, "1000")                                                       \
  XX(MWEI, "1000000")                                                          \
  XX(LOVELACE, "1000000")                                                      \
  XX(PICOETHER, "1000000")                                                     \
  XX(GWEI, "1000000000")                                                       \
  XX(SHANNON, "1000000000")                                                    \
  XX(NANOETHER, "1000000000")                                                  \
  XX(NANO, "1000000000")                                                       \
  XX(SZABO, "1000000000000")                                                   \
  XX(MICROETHER, "1000000000000")                                              \
  XX(MICRO, "1000000000000")                                                   \
  XX(FINNEY, "1000000000000000")                                               \
  XX(MILLIETHER, "1000000000000000")                                           \
  XX(MILLI, "1000000000000000")                                                \
  XX(ETHER, "1000000000000000000")                                             \
  XX(KETHER, "1000000000000000000000")                                         \
  XX(GRAND, "1000000000000000000000")                                          \
  XX(METHER, "1000000000000000000000000")                                      \
  XX(GETHER, "1000000000000000000000000000")                                   \
  XX(TETHER, "1000000000000000000000000000000")

enum eth_unit {
#define XX(UNIT, _) ETH_UNIT_##UNIT,
  ETH_UNIT_MAP(XX)
#undef XX
};

static const char *ETH_UNIT_AMOUNTS[] = {
#define XX(_, AMOUNT) AMOUNT,
  ETH_UNIT_MAP(XX)
#undef XX
};

ETHC_EXPORT char *eth_unit_convert(const char *amount, enum eth_unit from,
                                   enum eth_unit to);

#ifdef __cplusplus
}
#endif

#endif
