#include <ctype.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>
#include <string.h>

int eth_is_address(const char *addr, size_t len) {
  len = len == -1 ? strlen(addr) : len;

  if (!addr || len != 42 || !eth_is_hexstr(addr, len, 1))
    return 0;

  return 1;
}

int eth_is_checksum_address(const char *addr, size_t len) {
  uint8_t addr2[42], keccak[32];
  size_t i;

  len = len == -1 ? strlen(addr) : len;

  if (!eth_is_address(addr, len))
    return 0;

  for (i = 0; i < len; i++)
    addr2[i] = tolower(addr[i]);

  if (eth_keccak256(addr2 + 2, len - 2, keccak) == 0)
    return 0;

  for (i = 0; i < 20; i++) {
    const char *addr_ptr = addr + (i * 2) + 2;
    uint8_t keccak_high_nibble = keccak[i] >> 4 & 0xf;
    uint8_t keccak_low_nibble = keccak[i] & 0x0f;

    if (keccak_high_nibble >= 8 && islower(*addr_ptr) ||
        keccak_high_nibble < 8 && isupper(*addr_ptr))
      return 0;

    if (keccak_low_nibble >= 8 && islower(*(addr_ptr + 1)) ||
        keccak_low_nibble < 8 && isupper(*(addr_ptr + 1)))
      return 0;
  }

  return 1;
}
