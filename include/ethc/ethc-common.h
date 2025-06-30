#ifndef ETHC_COMMON_H
#define ETHC_COMMON_H

#ifdef _WIN32
#define ETHC_EXPORT __declspec(dllexport)
#elif __GNUC__
#define ETHC_EXPORT __attribute__((visibility("default")))
#else
#define ETHC_EXPORT
#endif

enum eth_op {
  ETH_OK               = 0,
  ETH_ERR_BUFFER_ALLOC = -1,
  ETH_ERR_INVALID_ARGS = -2
};

#define ETH_OP enum eth_op

#endif
