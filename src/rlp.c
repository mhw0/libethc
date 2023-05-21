#include <string.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>
#include <ethc/hex.h>
#include <ethc/address.h>

int eth_rlp_frame_init(struct ethc_rlp_frame **dest, uint8_t *bytes, size_t len) {
  struct ethc_rlp_frame *nframe;
  uint8_t *buf;
  size_t bufsize;

  nframe = (struct ethc_rlp_frame*)malloc(sizeof(struct ethc_rlp_frame));
  if (nframe == NULL)
    return -1;

  bufsize = bytes != NULL ? len : ETHC_RLP_FRAME_INITIAL_SIZE;
  buf = (uint8_t*)malloc(bufsize);
  if (buf == NULL) {
    free(nframe);
    return -1;
  }

  nframe->buf = buf;
  nframe->pframe = NULL;
  nframe->offset = 0;
  nframe->len = len;

  if (bytes != NULL)
    memcpy(nframe->buf, bytes, len);

  *dest = nframe;
  return 1;
}

int eth_rlp_init(struct eth_rlp *dest, int m) {
  struct ethc_rlp_frame *nframe;

  if (dest == NULL)
    return -1;

  if (eth_rlp_frame_init(&nframe, NULL, 0) <= 0)
    return -1;

  dest->cframe = nframe;
  dest->m = m;
  return 1;
}

int eth_rlp_array(struct eth_rlp *rlp) {
  struct ethc_rlp_frame *cframe, *nframe;
  uint8_t base;

  if (rlp == NULL)
    return -1;

  cframe = rlp->cframe;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (eth_rlp_frame_init(&nframe, NULL, 0) <= 0)
      return -1;

    nframe->pframe = cframe;
    rlp->cframe = nframe;
    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_len(rlp, NULL, &base) <= 0)
      return -1;

    if (base != 0xc0 && base != 0xf7)
      return -1;

    return 1;
  }

  return -1;
}

int eth_rlp_array_end(struct eth_rlp *rlp) {
  struct ethc_rlp_frame *cframe, *pframe;
  uint8_t base;

  if (rlp == NULL)
    return -1;

  cframe = rlp->cframe;
  pframe = cframe->pframe;

  if (rlp->m == ETH_RLP_ENCODE) {
    base = cframe->len <= 0x37 ? 0xc0 : 0xf7;

    rlp->cframe = pframe;

    if (eth_rlp_len(rlp, &(cframe->len), &base) <= 0)
      return -1;

    memcpy(&(pframe->buf[pframe->offset]), cframe->buf, cframe->len);
    pframe->offset += cframe->len;
    pframe->len += cframe->len;
    free(cframe->buf);
    free(cframe);
    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE)
    return 1;

  return -1;
}

int eth_rlp_len(struct eth_rlp *rlp, size_t *len, uint8_t *base) {
  struct ethc_rlp_frame *cframe;
  uint8_t head, bbase, llen;

  if (rlp == NULL)
    return -1;

  cframe = rlp->cframe;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (len == NULL || base == NULL)
      return -1;

    if (*len <= 0x37) {
      cframe->buf[cframe->offset++] = *base + *len;
      cframe->len++;
    } else if (*len <= 0xFF) {
      cframe->buf[cframe->offset++] = *base + 0x01;
      cframe->buf[cframe->offset++] = *len;
      cframe->len += 2;
    } else if (*len <= 0xFFFF) {
      cframe->buf[cframe->offset++] = *base + 0x37 + 0x02;
      cframe->buf[cframe->offset++] = (*len >> 0x08) & 0xFF;
      cframe->buf[cframe->offset++] = *len & 0xFF;
      cframe->len += 3;
    } else if (*len <= 0xFFFFFFFF) {
      cframe->buf[cframe->offset++] = *base + 0x37 + 0x04;
      cframe->buf[cframe->offset++] = (*len >> 0x18) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x10) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x08) & 0xFF;
      cframe->buf[cframe->offset++] = *len & 0xFF;
      cframe->len += 5;
    } else if (*len <= 0xFFFFFFFFFFFFFFFF) {
      cframe->buf[cframe->offset++] = *base + 0x37 + 0x08;
      cframe->buf[cframe->offset++] = (*len >> 0x38) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x30) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x28) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x20) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x18) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x10) & 0xFF;
      cframe->buf[cframe->offset++] = (*len >> 0x08) & 0xFF;
      cframe->buf[cframe->offset++] = *len & 0xFF;
      cframe->len += 9;
    } else {
      return -1;
    }

    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    head = cframe->buf[cframe->offset];

    if (head <= 0x7F) {
      llen = 1;
      bbase = 0;
    } else if (head <= 0xB7 || head <= 0xF7) {
      bbase = head <= 0xB7 ? 0x80 : 0xC0;
      llen = head - bbase;
      cframe->offset++;
    } else {
      bbase = head <= 0xBF ? 0xB8 : 0xF8;
      llen = head - bbase;

      if (llen <= 1) {
        llen = cframe->buf[cframe->offset++];
        cframe->offset += llen + 1;
      } else if (llen <= 2) {
        llen = cframe->buf[cframe->offset++] << 0x04;
        llen |= llen | cframe->buf[cframe->offset++];
        cframe->offset += llen + 2;
      } else if (*len <= 4) {
        llen = cframe->buf[cframe->offset++] << 0x18;
        llen |= cframe->buf[cframe->offset++] << 0x10;
        llen |= cframe->buf[cframe->offset++] << 0x08;
        llen |= cframe->buf[cframe->offset++];
        cframe->offset = llen + 4;
      } else if (*len <= 8) {
        llen = (size_t)cframe->buf[cframe->offset++] << 0x38;
        llen |= (size_t)cframe->buf[cframe->offset++] << 0x30;
        llen |= (size_t)cframe->buf[cframe->offset++] << 0x28;
        llen |= (size_t)cframe->buf[cframe->offset++] << 0x20;
        llen |= cframe->buf[cframe->offset++] << 0x18;
        llen |= cframe->buf[cframe->offset++] << 0x10;
        llen |= cframe->buf[cframe->offset++] << 0x08;
        llen |= cframe->buf[cframe->offset++];
        cframe->offset += 8;
      } else {
        return -1;
      }
    }

    if (len != NULL)
      *len = llen;

    if (base != NULL)
      *base = bbase;

    return 1;
  }

  return -1;
}


int eth_rlp_bytes(struct eth_rlp *rlp, uint8_t **bytes, size_t *len) {
  struct ethc_rlp_frame *cframe;
  uint8_t base, *buf;

  if (rlp == NULL || bytes == NULL)
    return -1;

  cframe = rlp->cframe;
  
  if (rlp->m == ETH_RLP_ENCODE) {
    if (*len == 1 && **bytes <= 0x7F) {
      cframe->buf[cframe->offset++] = **bytes;
      cframe->len++;
      return 1;
    }

    base = *len <= 0x37 ? 0x80 : 0xB7;
    if (eth_rlp_len(rlp, len, &base) <= 0)
      return -1;

    memcpy(&(cframe->buf[cframe->offset]), *bytes, *len);
    cframe->offset += *len;
    cframe->len += *len;
    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_len(rlp, len, &base) <= 0)
      return -1;

    if (base != 0x00 && base != 0x80)
      return -1;

    buf = (uint8_t*)malloc(sizeof(uint8_t) * (*len));
    if (buf == NULL)
      return -1;

    memcpy(buf, &(cframe->buf[cframe->offset]), *len);
    cframe->offset += *len;
    cframe->len += *len;
    *bytes = buf;
    return 1;
  }

  return -1;
}

int eth_rlp_hex(struct eth_rlp *rlp, char **hex, int *len) {
  uint8_t *buf;
  size_t hsize;
  int hlen;

  if (rlp == NULL || hex == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    /* TODO: not safe */
    hlen = len == NULL ? (int)strlen(*hex) : *len;

    if (eth_is_hex(*hex, hlen) <= 0)
      return -1;

    if ((hsize = eth_hex_to_bytes(&buf, *hex, hlen)) <= 0)
      return -1;

    if (eth_rlp_bytes(rlp, &buf, &hsize) <= 0) {
      free(buf);
      return -1;
    }

    free(buf);
    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(rlp, &buf, &hsize) <= 0)
      return -1;

    if ((hsize = (size_t)eth_hex_from_bytes(hex, buf, hsize)) <= 0)
      return -1;

    if (len != NULL)
      *len = hsize;

    free(buf);
    return 1;
  }

  return -1;
}

int eth_rlp_to_hex(char **dest, struct eth_rlp *src) {
  struct ethc_rlp_frame *cframe;
  char *buf;
  int hsize;

  if (dest == NULL || src == NULL)
    return -1;

  cframe = src->cframe;

  hsize = eth_hex_from_bytes(&buf, cframe->buf, cframe->len);
  if (hsize <= 0)
    return -1;

  *dest = buf;
  return hsize;
}

int eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len) {
  struct ethc_rlp_frame *nframe;
  uint8_t *buf, sbuf;

  if (dest == NULL || hex == NULL)
    return -1;

  if (len < 0)
    len = (int)strlen(hex); /* TODO: NOT SAFE */

  if ((sbuf = eth_hex_to_bytes(&buf, hex, len)) <= 0)
    return -1;

  if (eth_rlp_frame_init(&nframe, buf, sbuf) <= 0)
    return -1;

  dest->cframe = nframe;
  dest->m = ETH_RLP_DECODE;
  return 1;
}

int eth_rlp_free(struct eth_rlp *dest) {
  struct ethc_rlp_frame *cframe;
  if (dest == NULL)
    return -1;

  cframe = dest->cframe;

  free(cframe->buf);
  return 1;
}

int eth_rlp_uint8(struct eth_rlp *rlp, uint8_t *d) {
  uint8_t data[1], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = *d;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(rlp, &bytes, &blen) <= 0)
      return -1;

    *d = *bytes;
    free(bytes);
    return 1;
  }

  return -1;
}

int eth_rlp_uint16(struct eth_rlp *rlp, uint16_t *d) {
  uint8_t data[2], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = (*d >> 0x08) & 0xFF;
    bytes[blen++] = *d & 0xFF;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(rlp, &bytes, &blen) <= 0)
      return -1;

    *d = bytes[0] << 0x08;
    *d += bytes[1];

    free(bytes);
    return 1;
  }

  return -1;
}

#include <stdio.h>
int eth_rlp_uint32(struct eth_rlp *rlp, uint32_t *d) {
  uint8_t data[4], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = (*d >> 0x18) & 0xFF;
    bytes[blen++] = (*d >> 0x10) & 0xFF;
    bytes[blen++] = (*d >> 0x08) & 0xFF;
    bytes[blen++] = *d & 0xFF;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(rlp, &bytes, &blen) <= 0)
      return -1;

    *d = bytes[0] << 0x18;
    *d |= bytes[1] << 0x10;
    *d |= bytes[2] << 0x08;
    *d |= bytes[3];
    free(bytes);
    return 1;
  }

  return -1;
}

int eth_rlp_uint64(struct eth_rlp *rlp, uint64_t *d) {
  uint8_t data[8], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = (*d >> 0x38) & 0xFF;
    bytes[blen++] = (*d >> 0x30) & 0xFF;
    bytes[blen++] = (*d >> 0x28) & 0xFF;
    bytes[blen++] = (*d >> 0x20) & 0xFF;
    bytes[blen++] = (*d >> 0x18) & 0xFF;
    bytes[blen++] = (*d >> 0x10) & 0xFF;
    bytes[blen++] = (*d >> 0x08) & 0xFF;
    bytes[blen++] = *d & 0xFF;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if (eth_rlp_bytes(rlp, &bytes, &blen) <= 0)
      return -1;

    *d = (uint64_t) bytes[0] << 0x38;
    *d |= (uint64_t)bytes[1] << 0x30;
    *d |= (uint64_t)bytes[2] << 0x28;
    *d |= (uint64_t)bytes[3] << 0x20;
    *d |= bytes[4] << 0x18;
    *d |= bytes[5] << 0x10;
    *d |= bytes[6] << 0x8;
    *d |= bytes[7];
    free(bytes);
    return 1;
  }

  return -1;
}

int eth_rlp_address(struct eth_rlp *rlp, char **addr) {
  int hexlen;

  if (rlp == NULL || addr == NULL)
    return -1;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (eth_is_address(*addr) <= 0)
      return -1;

    if (strncmp(*addr, "0x", 2) == 0)
      *addr += 2;

    hexlen = 40;
    if (eth_rlp_hex(rlp, addr, &hexlen) <= 0)
      return -1;

    return 1;
  }

  if (rlp->m == ETH_RLP_DECODE)
    return eth_rlp_hex(rlp, addr, &hexlen);

  return -1;
}
