#include <ctype.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

int eth_is_address(const char *addr) {
  ETHC_RETURN_IF_FALSE(addr != NULL, ETHC_FAIL);

  if (!eth_is_hex(addr, 42, 1))
    return ETHC_FAIL;

  return ETHC_SUCCESS;
}

int eth_is_checksum_address(const char *addr) {
  uint8_t addr2[42], keccak[32];
  size_t i;

  ETHC_RETURN_IF_FALSE(addr != NULL, ETHC_FAIL);

  if (!eth_is_address(addr))
    return ETHC_FAIL;

  for (i = 0; i < 42; i++)
    addr2[i] = tolower(addr[i]);

  if (!eth_keccak256(keccak, addr2 + 2, 40))
    return ETHC_FAIL;

  for (i = 0; i < 20; i++) {
    const char *addr_ptr = addr + (i * 2) + 2;
    uint8_t high_nibble = keccak[i] >> 4 & 0xf;
    uint8_t low_nibble = keccak[i] & 0x0f;

    if (high_nibble >= 8 && islower(*addr_ptr) ||
        high_nibble < 8 && isupper(*addr_ptr))
      return ETHC_FAIL;

    if (low_nibble >= 8 && islower(*(addr_ptr + 1)) ||
        low_nibble < 8 && isupper(*(addr_ptr + 1)))
      return ETHC_FAIL;
  }

  return ETHC_SUCCESS;
}

int eth_to_checksum_address(char *addr) {
  size_t i;
  uint8_t keccak[32];

  ETHC_RETURN_IF_FALSE(addr != NULL, ETHC_FAIL);

  if (!eth_is_address(addr))
    return ETHC_FAIL;

  if (!eth_keccak256(keccak, (uint8_t *)addr + 2, 40))
    return ETHC_FAIL;

  for (i = 0; i < 20; i++) {
    char *addr_ptr = addr + (i * 2) + 2;
    uint8_t high_nibble = keccak[i] >> 4 & 0xf;
    uint8_t low_nibble = keccak[i] & 0x0f;

    if (high_nibble >= 8)
      *addr_ptr = toupper(*addr_ptr);
    else
      *addr_ptr = tolower(*addr_ptr);

    if (low_nibble >= 8)
      *(addr_ptr + 1) = toupper(*(addr_ptr + 1));
    else
      *(addr_ptr + 1) = tolower(*(addr_ptr + 1));
  }

  return ETHC_SUCCESS;
}
