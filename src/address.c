#include <ctype.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

int eth_is_address(const char *addr) {
  if (!addr || !eth_is_hexstr(addr, 42, 1))
    return 0;

  return 1;
}

int eth_is_checksum_address(const char *addr) {
  uint8_t addr2[42], keccak[32];
  size_t i;

  if (!eth_is_address(addr))
    return 0;

  for (i = 0; i < 42; i++)
    addr2[i] = tolower(addr[i]);

  if (eth_keccak256(addr2 + 2, 40, keccak) == 0)
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

int eth_to_checksum_address(char *addr) {
  size_t i;
  uint8_t keccak[32];

  if(!addr || !eth_is_address(addr))
    return 0;

  if(eth_keccak256((uint8_t*)addr + 2, 40, keccak) == 0)
    return 0;

  for(i = 0; i < 20; i++) {
    char *addr_ptr = addr + (i * 2) + 2;
    uint8_t keccak_high_nibble = keccak[i] >> 4 & 0xf;
    uint8_t keccak_low_nibble = keccak[i] & 0x0f;

    if(keccak_high_nibble >= 8)
      *addr_ptr = toupper(*addr_ptr);
    else
      *addr_ptr = tolower(*addr_ptr);

    if(keccak_low_nibble >= 8)
      *(addr_ptr + 1) = toupper(*(addr_ptr + 1));
    else
      *(addr_ptr + 1) = tolower(*(addr_ptr + 1));
  }

  return 1;
}
