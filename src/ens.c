#include "ethc/ethc-common.h"
#include <string.h>
#include <stdlib.h>
#include <ethc/ens.h>
#include <ethc/keccak256.h>

ETH_OP eth_ens_namehash(uint8_t *dest, char *name) {
  char *token = NULL, *tokens[ETH_ENS_MAX_TOKENS], copied[ETH_ENS_MAX_COPIED_NAME_BUFFER_SIZE];
  int i = 0, j = 0;
  uint8_t state[64] = {
    // 32 elements are reserved for the node
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // remaining 32 for the hash of the label
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  if (dest == NULL || name == NULL)
    return ETH_ERR_INVALID_ARGS;

  // copy the name into fixed buffer
  strcpy(copied, name);

  // take the first token
  token = strtok(copied, ETH_ENS_DELIMITER);

  // walk through the tokens
  while (token != NULL) {
    tokens[i++] = token;
    token = strtok(NULL, ETH_ENS_DELIMITER);
  }

  // walk through again to calculate the hash (in reversed orderer)
  for (j = i - 1; j >= 0; j--) {
    // calculate keccak256 hash for the label
    eth_keccak256(&state[32], (uint8_t*)tokens[j], strlen(tokens[j]));

    // calculate the hash of the previous node + label hash
    eth_keccak256(state, state, 64);
  }

  // copy the first 32 bytes of the state to the destination
  strncpy((char*)dest, (char*)state, 32);

  return ETH_OK;
}
