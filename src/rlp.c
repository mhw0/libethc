#include <string.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>
#include <ethc/hex.h>
#include <ethc/address.h>
#include <ethc/internals.h>

int eth_rlp_frame(struct eth_rlp_frame **dest, uint8_t *bytes, size_t len) {
  struct eth_rlp_frame *f;
  uint8_t *buf;

  f = (struct eth_rlp_frame*)malloc(sizeof(struct eth_rlp_frame));
  if (f == NULL)
    return -1;

  len = bytes != NULL ? len : 1024;
  buf = (uint8_t*)malloc(len);
  if (buf == NULL)
    return -1;

  f->fp = buf;
  f->pf = NULL;
  f->fc = 0;
  f->fs = len;

  if (bytes != NULL)
    memcpy(f->fp, bytes, len);

  *dest = f;
  return 1;
}

int eth_rlp_init(struct eth_rlp *dest, int m) {
  struct eth_rlp_frame *f;
  uint8_t *buf;
  if (dest == NULL)
    return -1;

  if (eth_rlp_frame(&f, NULL, 0) <= 0)
    return -1;

  dest->cf = f;
  dest->m = m;
  return 1;
};

int eth_rlp_list(struct eth_rlp *dest) {
  struct eth_rlp_frame *sf;
  size_t len;
  int base;

  if (dest == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    if (eth_rlp_frame(&sf, NULL, 0) <= 0)
      return -1;

    sf->pf = dest->cf;
    dest->cf = sf;
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_len(dest, &len, (uint8_t*)&base) <= 0)
      return -1;

    if (base != 0xc0 && base != 0xf7)
      return -1;

    return 1;
  }

  return -1;
}

int eth_rlp_list_end(struct eth_rlp *dest) {
  struct eth_rlp_frame *cf, *sf;
  int base;

  if (dest == NULL || dest->cf == NULL)
    return -1;

  cf = dest->cf;

  if (dest->m == ETH_RLP_ENCODE) {
    if (cf->pf != NULL) {
      sf = cf;
      cf = (dest->cf = dest->cf->pf);

      base = 0xc0;
      if (eth_rlp_len(dest, &sf->fc, &base) <= 0)
        return -1;

      memcpy(cf->fp + cf->fc, sf->fp, sf->fc);
      cf->fc += sf->fc;
      free(sf->fp);
      free(sf);
      return 1;
    }
  }

  if (dest->m == ETH_RLP_DECODE)
    return 1;

  return -1;
}

int eth_rlp_len(struct eth_rlp *dest, size_t *len, uint8_t *base) {
  struct eth_rlp_frame *cf;
  int b;

  if (dest == NULL)
    return -1;

  cf = dest->cf;

  if (dest->m == ETH_RLP_ENCODE) {
    if (*len <= 0x37) {
      cf->fp[cf->fc++] = *base + *len;
    } else if (*len <= 0xff) { // u8
      cf->fp[cf->fc++] = *base + 0x01;
      cf->fp[cf->fc++] = *len;
    } else if (*len <= 0xffff) { // u16
      cf->fp[cf->fc++] = *base + 0x37 + 0x02;
      cf->fp[cf->fc++] = (*len >> 0x08) & 0xff;
      cf->fp[cf->fc++] = *len & 0xff;
    } else if (*len <= 0xffffffff) { // u32
      cf->fp[cf->fc++] = *base + 0x37 + 0x04;
      cf->fp[cf->fc++] = (*len >> 0x18) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x10) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x08) & 0xff;
      cf->fp[cf->fc++] = *len & 0xff;
    } else if (*len <= 0xffffffffffffffff) { // u64
      cf->fp[cf->fc++] = *base + 0x37 + 0x08;
      cf->fp[cf->fc++] = (*len >> 0x38) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x30) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x28) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x20) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x18) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x10) & 0xff;
      cf->fp[cf->fc++] = (*len >> 0x08) & 0xff;
      cf->fp[cf->fc++] = *len & 0xff;
    } else {
      return -1;
    }

    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    // TODO
    if (cf->fc >= cf->fs)
      return -1;

    b = cf->fp[cf->fc];

    if (b <= 0x7f) {
      *len = 1;
      return 1;
    }

    if (b <= 0xb7 || b <= 0xf7) {
      *base = b <= 0xb7 ? 0x80 : 0xc0;
      *len = b - *base;
      cf->fc += 1;
      return 1;
    }

    if (b <= 0xbf || b <= 0xff) {
      *base = b <= 0xbf ? 0xb8 : 0xf8;
      *len = b - *base;

      if (*len <= 1) {
        *len = cf->fp[cf->fc++];
        cf->fc += *len + 1;
      } else if (*len <= 2) {
        *len = cf->fp[cf->fc++] << 4;
        *len += *len | cf->fp[cf->fc++];
        cf->fc += *len + 2;
      } else if (*len <= 4) {
        *len = cf->fp[cf->fc++] << 24;
        *len += cf->fp[cf->fc++] << 16;
        *len += cf->fp[cf->fc++] << 8;
        *len += cf->fp[cf->fc++];
        cf->fc += *len + 4;
      } else if (*len <= 8) {
        *len = (size_t)cf->fp[cf->fc++] << 56;
        *len += (size_t)cf->fp[cf->fc++] << 48;
        *len += (size_t)cf->fp[cf->fc++] << 40;
        *len += (size_t)cf->fp[cf->fc++] << 32;
        *len += (size_t)cf->fp[cf->fc++] << 24;
        *len += (size_t)cf->fp[cf->fc++] << 16;
        *len += (size_t)cf->fp[cf->fc++] << 8;
        *len += (size_t)cf->fp[cf->fc++];
        cf->fc += *len + 8;
      } else {
        return -1;
      }

      return 1;
    }

  }

  return -1;
}

int eth_rlp_bytes(struct eth_rlp *dest, uint8_t **bytes, size_t *len) {
  struct eth_rlp_frame *cf;
  uint8_t base, *buf;

  if (dest == NULL || bytes == NULL)
    return -1;

  cf = dest->cf;
  
  if (dest->m == ETH_RLP_ENCODE) {
    if (*len == 1 && **bytes <= 0x7f) {
      cf->fp[cf->fc++] = **bytes;
      return 1;
    }

    base = 0x80;
    if (eth_rlp_len(dest, len, &base) <= 0)
      return -1;

    memcpy(cf->fp + cf->fc, *bytes, *len);
    cf->fc += *len;
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_len(dest, len, &base) <= 0)
      return -1;

    if (base != 0x00 && base != 0x80)
      return -1;

    if (*len > (cf->fs - cf->fc))
      return -1;

    buf = (uint8_t*)malloc(sizeof(uint8_t) * (*len));
    if (buf == NULL)
      return -1;

    memcpy(buf, cf->fp + cf->fc, *len);
    cf->fc += *len;
    *bytes = buf;
    return 1;
  }

  return -1;
}

int eth_rlp_hex(struct eth_rlp *dest, char **hex, int *len) {
  struct eth_rlp_frame *cf;
  uint8_t *buf;
  size_t hsize;

  if (dest == NULL || hex == NULL)
    return -1;

  cf = dest->cf;

  if (dest->m == ETH_RLP_ENCODE) {
    if (*len == -1)
      *len = (int)strlen(*hex);

    if (eth_is_hex(*hex, *len) <= 0)
      return -1;

    if ((hsize = eth_hex_to_bytes(&buf, *hex, *len)) <= 0)
      return -1;

    if (eth_rlp_bytes(dest, &buf, &hsize) <= 0) {
      free(buf);
      return -1;
    }

    free(buf);
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(dest, &buf, &hsize) <= 0)
      return -1;

    if ((hsize = (size_t)eth_hex_from_bytes(hex, buf, hsize)) <= 0)
      return -1;

    if (len != NULL)
      *len = hsize;

    free(buf);
    buf = NULL;
    return 1;
  }

  return -1;
}

int eth_rlp_to_hex(char **dest, struct eth_rlp *src) {
  struct eth_rlp_frame *cf;
  char *buf;
  int hsize;

  if (dest == NULL || src == NULL)
    return -1;

  cf = src->cf;

  if ((hsize = eth_hex_from_bytes(&buf, cf->fp, cf->fc)) <= 0)
    return -1;

  *dest = buf;
  return hsize;
}

int eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len) {
  struct eth_rlp_frame *f;
  uint8_t *buf, sbuf;

  if (dest == NULL || hex == NULL)
    return -1;

  if (len < 0)
    len = (int)strlen(hex);

  if ((sbuf = eth_hex_to_bytes(&buf, hex, len)) <= 0)
    return -1;

  if (eth_rlp_frame(&f, buf, sbuf) <= 0)
    return -1;

  dest->cf = f;
  return 1;
}

int eth_rlp_free(struct eth_rlp *dest) {
  if (dest == NULL)
    return -1;

  free(dest->cf->fp);
  dest->cf->fp = NULL;
  return 1;
}

int eth_rlp_uint8(struct eth_rlp *dest, uint8_t *d) {
  uint8_t *bytes;
  size_t blen = 0;

  if (dest == NULL || d == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    bytes = (uint8_t*)malloc(sizeof(uint8_t) * 1);
    if (bytes == NULL)
      return -1;

    bytes[blen++] = *d;
    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0) {
      free(bytes);
      return -1;
    }

    free(bytes);
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0)
      return -1;

    *d = *bytes;
    free(bytes);
    return 1;
  }

  return -1;
};

int eth_rlp_uint16(struct eth_rlp *dest, uint16_t *d) {
  uint8_t *bytes;
  size_t blen = 0;

  if (dest == NULL || d == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    bytes = (uint8_t*)malloc(sizeof(uint8_t) * 2);
    if (bytes == NULL)
      return -1;

    bytes[blen++] = (*d >> 0x08) & 0xff;
    bytes[blen++] = *d & 0xff;

    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0) {
      free(bytes);
      return -1;
    }

    free(bytes);
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0)
      return -1;

    *d = bytes[0] << 8;
    *d += bytes[1];

    free(bytes);
    return 1;
  }

  return -1;
};

int eth_rlp_uint32(struct eth_rlp *dest, uint32_t *d) {
  uint8_t *bytes;
  size_t blen = 0;

  if (dest == NULL || d == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    bytes = (uint8_t*)malloc(sizeof(uint8_t) * 4);
    if (bytes == NULL)
      return -1;

    bytes[blen++] = (*d >> 0x18) & 0xff;
    bytes[blen++] = (*d >> 0x10) & 0xff;
    bytes[blen++] = (*d >> 0x08) & 0xff;
    bytes[blen++] = *d & 0xff;

    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0) {
      free(bytes);
      return -1;
    }

    free(bytes);
    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0)
      return -1;

    *d = bytes[0] << 24;
    *d += bytes[1] << 16;
    *d += bytes[2] << 8;
    *d += bytes[3];
    free(bytes);
    return 1;
  }

  return -1;
};

int eth_rlp_uint64(struct eth_rlp *dest, uint64_t *d) {
  uint8_t *bytes;
  size_t blen = 0;

  if (dest == NULL || d == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    bytes = (uint8_t*)malloc(sizeof(uint8_t) * 8);
    if (bytes == NULL)
      return -1;

    bytes[blen++] = (*d >> 0x38) & 0xff;
    bytes[blen++] = (*d >> 0x30) & 0xff;
    bytes[blen++] = (*d >> 0x28) & 0xff;
    bytes[blen++] = (*d >> 0x20) & 0xff;
    bytes[blen++] = (*d >> 0x18) & 0xff;
    bytes[blen++] = (*d >> 0x10) & 0xff;
    bytes[blen++] = (*d >> 0x08) & 0xff;
    bytes[blen++] = *d & 0xff;
    return eth_rlp_bytes(dest, &bytes, &blen);
  }

  if (dest->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(dest, &bytes, &blen) <= 0)
      return -1;

    *d = (uint64_t)bytes[0] << 56;
    *d += (uint64_t)bytes[1] << 48;
    *d += (uint64_t)bytes[2] << 40;
    *d += (uint64_t)bytes[3] << 32;
    *d += (uint64_t)bytes[4] << 24;
    *d += (uint64_t)bytes[5] << 16;
    *d += (uint64_t)bytes[6] << 8;
    *d += (uint64_t)bytes[7];
    free(bytes);
    return 1;
  }

  return -1;
};

int eth_rlp_address(struct eth_rlp *dest, char **addr) {
  int hexlen;
  if (dest == NULL || addr == NULL)
    return -1;

  if (dest->m == ETH_RLP_ENCODE) {
    if (eth_is_address(*addr) <= 0)
      return -1;

    if (ethc_strncasecmp(*addr, "0x", 2) == 0)
      *addr += 2;

    hexlen = 40;
    if (eth_rlp_hex(dest, addr, &hexlen) <= 0)
      return -1;

    return 1;
  }

  if (dest->m == ETH_RLP_DECODE) {
    hexlen = 20;
    if (eth_rlp_hex(dest, addr, &hexlen) <= 0)
      return -1;

    return 1;
  }

  return -1;
}
