#include <ctype.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>
#include <ethc/internals.h>

int eth_is_address(const char *addr) {
  if (addr == NULL)
    return -1;

  if (ethc_strncasecmp(addr, "0x", 2) == 0)
    return eth_is_hex(addr, 42);

  return eth_is_hex(addr, 40);
}

int eth_is_checksum_address(const char *addr) {
  uint8_t lcaddr[40], keccak[32], i;

  if (addr == NULL)
    return -1;

  if (ethc_strncasecmp(addr, "0x", 2) == 0)
    addr += 2;

  if (eth_is_hex(addr, 40) <= 0)
    return -1;

  for (i = 0; i < 40; i++)
    lcaddr[i] = tolower(addr[i]);

  if (eth_keccak256(keccak, lcaddr, 40) <= 0)
    return -1;

  for (i = 0; i < 20; i++) {
    const char *addrptr = addr + (i * 2);
    uint8_t hnib = (keccak[i] >> 0x04) & 0xf;
    uint8_t lnib = keccak[i] & 0x0f;

    if ((hnib >= 8 && islower(*addrptr)) || (hnib < 8 && isupper(*addrptr)))
      return 0;

    if ((lnib >= 8 && islower(*(addrptr + 1))) || (lnib < 8 && isupper(*(addrptr + 1))))
      return 0;
  }

  return 1;
}

int eth_to_checksum_address(char *addr) {
  uint8_t keccak[32];
  int i;

  if (addr == NULL)
    return -1;

  if (ethc_strncasecmp(addr, "0x", 2) == 0)
    addr += 2;

  if (eth_is_hex(addr, 40) <= 0)
    return -1;

  if (eth_keccak256(keccak, (uint8_t*)addr, 40) != 1)
    return -1;

  for (i = 0; i < 20; i++) {
    char *addrptr = addr + (i * 2);
    uint8_t hnib = keccak[i] >> 4 & 0xf;
    uint8_t lnib = keccak[i] & 0x0f;

    if (hnib >= 8)
      *addrptr = toupper(*addrptr);
    else
      *addrptr = tolower(*addrptr);

    if (lnib >= 8)
      *(addrptr + 1) = toupper(*(addrptr  + 1));
    else
      *(addrptr + 1) = tolower(*(addrptr + 1));
  }

  return 1;
}
