#include <string.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>
#include <ethc/hex.h>
#include <ethc/address.h>
#include <stdio.h>

/**
 * Implementation details:
 *
 * RLP: [["a"], ["b", "c", "d", ["e", "f", ["g"]]]]
 *
 * Each opening square bracket represents one RLP buffer.
 * When a new array is opened, a new RLP buffer is allocated
 * with a pointer to its parent buffer. Then, it is set as the
 * current buffer and further data encodings (like eth_rlp_uint8,
 * eth_rlp_address etc.) are stored in this buffer:
 *
 * buffer[1]: <- parent is NULL
 *   buffer[2]: <- parent is 1
 *     "a"
 *   buffer[3]: <- parent is 1
 *     "b"
 *     "c"
 *     "d"
 *     buffer[4]: <- parent is 3
 *       "e"
 *       "f"
 *       buffer[5]: <- parent is 4
 *         "g"
 *
 * Once bracket is closed, the content of the current buffer is merged
 * into the parent buffer (with corresponding prefix), then flushed after
 * which the parent buffer becomes the current buffer.
 */


ETH_OP ethc_rlp_buffer_init(struct ethc_rlp_buffer **dest, uint8_t *rawbuf, size_t len) {
  struct ethc_rlp_buffer *rlpbuf;

  rlpbuf = (struct ethc_rlp_buffer*)malloc(sizeof(struct ethc_rlp_buffer));
  if (rlpbuf == NULL)
    return ETH_ERR_BUFFER_ALLOC;

  // if the raw buffer is not provided, allocate
  // space to store rlp buffer data
  if (rawbuf == NULL) {
    len = ETHC_RLP_BUFFER_INIT_SIZE;
    rawbuf = (uint8_t*)malloc(len);
    if (rawbuf == NULL)
      return ETH_ERR_BUFFER_ALLOC;
  }

  rlpbuf->rawbuf = rawbuf;
  rlpbuf->pbuf = NULL;
  rlpbuf->offset = 0;
  rlpbuf->len = len;

  *dest = rlpbuf;

  return ETH_OK;
}

ETH_OP eth_rlp_init(struct eth_rlp *dest, enum eth_rlp_mode m) {
  struct ethc_rlp_buffer *cbuf;
  ETH_OP op;

  if (dest == NULL)
    return ETH_ERR_INVALID_ARGS;

  if ((op = ethc_rlp_buffer_init(&cbuf, NULL, 0)) != ETH_OK)
    return op;

  dest->cbuf = cbuf;
  dest->m = m;

  return ETH_OK;
}

ETH_OP eth_rlp_array(struct eth_rlp *rlp) {
  struct ethc_rlp_buffer *pcbuf = NULL, *cbuf = NULL;
  uint8_t base;
  ETH_OP op;

  if (rlp == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    pcbuf = rlp->cbuf;

    if ((op = ethc_rlp_buffer_init(&cbuf, NULL, 0)) != ETH_OK)
      return op;

    rlp->cbuf = cbuf;
    cbuf->pbuf = pcbuf;

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_len(rlp, NULL, &base)) != ETH_OK)
      return op;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_array_end(struct eth_rlp *rlp) {
  struct ethc_rlp_buffer *cbuf, *pcbuf;
  uint8_t base;
  ETH_OP op;

  if (rlp == NULL)
    return ETH_ERR_INVALID_ARGS;

  pcbuf = rlp->cbuf;
  cbuf = rlp->cbuf->pbuf;

  if (rlp->m == ETH_RLP_ENCODE) {
    // array is closing, parent buffer becomes the current buffer
    rlp->cbuf = cbuf;

    // base for the length
    base = pcbuf->offset <= 0x37 ? 0xc0 : 0xf7;

    // encode the length of the previous current buffer into the actual current buffer
    if ((op = eth_rlp_len(rlp, &(pcbuf->offset), &base)) != ETH_OK)
      return op;

    // copy contents of the previous current buffer into the current buffer
    memcpy(&(cbuf->rawbuf[cbuf->offset]), pcbuf->rawbuf, pcbuf->offset);

    // move the offset of the 
    cbuf->offset += pcbuf->offset;

    // free the previous current buffer
    free(pcbuf->rawbuf);
    free(pcbuf);

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE)
    return ETH_OK;

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_len(struct eth_rlp *rlp, size_t *len, uint8_t *base) {
  struct ethc_rlp_buffer *cbuf;
  uint8_t head, bbase, llen;

  if (rlp == NULL)
    return ETH_ERR_INVALID_ARGS;

  cbuf = rlp->cbuf;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (len == NULL || base == NULL)
      return ETH_ERR_INVALID_ARGS;

    if (*len <= 0x37) {
      cbuf->rawbuf[cbuf->offset++] = *base + *len;
      cbuf->len++;
    } else if (*len <= 0xFF) {
      cbuf->rawbuf[cbuf->offset++] = *base + 0x01;
      cbuf->rawbuf[cbuf->offset++] = *len;
      cbuf->len += 2;
    } else if (*len <= 0xFFFF) {
      cbuf->rawbuf[cbuf->offset++] = *base + 0x02;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x08) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = *len & 0xFF;
      cbuf->len += 3;
    } else if (*len <= 0xFFFFFFFF) {
      cbuf->rawbuf[cbuf->offset++] = *base + 0x04;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x18) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x10) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x08) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = *len & 0xFF;
      cbuf->len += 5;
    } else if (*len <= 0xFFFFFFFFFFFFFFFF) {
      cbuf->rawbuf[cbuf->offset++] = *base + 0x08;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x38) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x30) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x28) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x20) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x18) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x10) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = (*len >> 0x08) & 0xFF;
      cbuf->rawbuf[cbuf->offset++] = *len & 0xFF;
      cbuf->len += 9;
    } else {
      return ETH_ERR_INVALID_ARGS;
    }

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    head = cbuf->rawbuf[cbuf->offset];

    if (head <= 0x7F) {
      llen = 1;
      bbase = 0;
    } else if (head <= 0xB7 || (head >= 0xC0 && head <= 0xF7)) {
      bbase = head <= 0xB7 ? 0x80 : 0xC0;
      llen = head - bbase;
      cbuf->offset++;
    } else {
      bbase = head <= 0xBF ? 0xB8 : 0xF8;
      llen = head - bbase;

      if (llen <= 1) {
        cbuf->offset += llen + 1;
        llen = cbuf->rawbuf[cbuf->offset++];
      } else if (llen <= 2) {
        cbuf->offset += llen + 2;
        llen = cbuf->rawbuf[cbuf->offset++] << 0x04;
        llen |= llen | cbuf->rawbuf[cbuf->offset++];
      } else if (*len <= 4) {
        cbuf->offset = llen + 4;
        llen = cbuf->rawbuf[cbuf->offset++] << 0x18;
        llen |= cbuf->rawbuf[cbuf->offset++] << 0x10;
        llen |= cbuf->rawbuf[cbuf->offset++] << 0x08;
        llen |= cbuf->rawbuf[cbuf->offset++];
      } else if (*len <= 8) {
        cbuf->offset += 8;
        llen = (size_t)cbuf->rawbuf[cbuf->offset++] << 0x38;
        llen |= (size_t)cbuf->rawbuf[cbuf->offset++] << 0x30;
        llen |= (size_t)cbuf->rawbuf[cbuf->offset++] << 0x28;
        llen |= (size_t)cbuf->rawbuf[cbuf->offset++] << 0x20;
        llen |= cbuf->rawbuf[cbuf->offset++] << 0x18;
        llen |= cbuf->rawbuf[cbuf->offset++] << 0x10;
        llen |= cbuf->rawbuf[cbuf->offset++] << 0x08;
        llen |= cbuf->rawbuf[cbuf->offset++];
      } else {
        return ETH_ERR_INVALID_ARGS;
      }
    }

    if (len != NULL)
      *len = llen;

    if (base != NULL)
      *base = bbase;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_bytes(struct eth_rlp *rlp, uint8_t **bytes, size_t *len) {
  ETH_OP op;
  struct ethc_rlp_buffer *cbuf;
  uint8_t base, *buf;

  if (rlp == NULL || bytes == NULL)
    return ETH_ERR_INVALID_ARGS;

  cbuf = rlp->cbuf;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (*len == 1 && **bytes <= 0x7F) {
      if (**bytes == 0x00)
        cbuf->rawbuf[cbuf->offset++] = 0x80;
      else
        cbuf->rawbuf[cbuf->offset++] = **bytes;

      cbuf->len++;
      return ETH_OK;
    }

    base = *len <= 0x37 ? 0x80 : 0xB7;
    if ((op = eth_rlp_len(rlp, len, &base)) != ETH_OK)
      return op;

    memcpy(&(cbuf->rawbuf[cbuf->offset]), *bytes, *len);
    cbuf->offset += *len;
    cbuf->len += *len;

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_len(rlp, len, &base)) != ETH_OK)
      return op;

    buf = (uint8_t*)malloc(sizeof(uint8_t) * (*len));
    if (buf == NULL)
      return ETH_ERR_BUFFER_ALLOC;

    memcpy(buf, &(cbuf->rawbuf[cbuf->offset]), *len);
    cbuf->offset += *len;
    cbuf->len += *len;
    *bytes = buf;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_hex(struct eth_rlp *rlp, char **hex, int *len) {
  uint8_t *buf;
  size_t hsize;
  int hlen;
  ETH_OP op;

  if (rlp == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    /* TODO: not safe */
    hlen = len == NULL ? (int)strlen(*hex) : *len;

    if (eth_is_hex(*hex, hlen) <= 0)
      return ETH_ERR_INVALID_ARGS;

    if ((hsize = eth_hex_to_bytes(&buf, *hex, hlen)) <= 0)
      return ETH_ERR_INVALID_ARGS;

    if ((op = eth_rlp_bytes(rlp, &buf, &hsize)) != ETH_OK) {
      free(buf);
      return ETH_ERR_INVALID_ARGS;
    }

    free(buf);

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_bytes(rlp, &buf, &hsize)) != ETH_OK)
      return op;

    if (hsize == 0) {
      *hex = "0";
    } else if ((hsize = (size_t)eth_hex_from_bytes(hex, buf, hsize)) <= 0) {
      return ETH_ERR_INVALID_ARGS;
    }

    if (len != NULL)
      *len = hsize;

    free(buf);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_uint8(struct eth_rlp *rlp, uint8_t *d) {
  ETH_OP op;
  uint8_t data[1], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = *d;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_bytes(rlp, &bytes, &blen)) != ETH_OK)
      return op;

    if (blen == 0) {
      *d = 0;
    } else {
      *d = *bytes;
    }

    free(bytes);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_uint16(struct eth_rlp *rlp, uint16_t *d) {
  ETH_OP op;
  uint8_t data[2], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = (*d >> 0x08) & 0xFF;
    bytes[blen++] = *d & 0xFF;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_bytes(rlp, &bytes, &blen)) != ETH_OK)
      return op;

    if (blen == 0) {
      *d = 0;
    } else {
      *d = bytes[0] << 0x08;
      *d += bytes[1];
    }

    free(bytes);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_uint32(struct eth_rlp *rlp, uint32_t *d) {
  ETH_OP op;
  uint8_t data[4], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    bytes[blen++] = (*d >> 0x18) & 0xFF;
    bytes[blen++] = (*d >> 0x10) & 0xFF;
    bytes[blen++] = (*d >> 0x08) & 0xFF;
    bytes[blen++] = *d & 0xFF;

    return eth_rlp_bytes(rlp, &bytes, &blen);
  }

  if (rlp->m == ETH_RLP_DECODE) {
    if ((op = eth_rlp_bytes(rlp, &bytes, &blen)) != ETH_OK)
      return op;

    if (blen == 0) {
      *d = 0;
    } else {
      *d = bytes[0] << 0x18;
      *d |= bytes[1] << 0x10;
      *d |= bytes[2] << 0x08;
      *d |= bytes[3];
    }

    free(bytes);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_uint64(struct eth_rlp *rlp, uint64_t *d) {
  ETH_OP op;
  uint8_t data[8], *bytes = data;
  size_t blen = 0;

  if (rlp == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

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
    if ((op = eth_rlp_bytes(rlp, &bytes, &blen)) != ETH_OK)
      return op;

    if (blen == 0) {
      *d = 0;
    } else {
      *d = (uint64_t) bytes[0] << 0x38;
      *d |= (uint64_t)bytes[1] << 0x30;
      *d |= (uint64_t)bytes[2] << 0x28;
      *d |= (uint64_t)bytes[3] << 0x20;
      *d |= bytes[4] << 0x18;
      *d |= bytes[5] << 0x10;
      *d |= bytes[6] << 0x8;
      *d |= bytes[7];
    }

    free(bytes);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_uint(struct eth_rlp *rlp, uint64_t *d) {
  ETH_OP op;
  size_t offset, len;
  uint8_t base;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (*d <= 0xff) {
      return eth_rlp_uint8(rlp, (uint8_t*)d);
    } else if (*d <= 0xffff) {
      return eth_rlp_uint16(rlp, (uint16_t*)d);
    } else if (*d <= 0xffffffff) {
      return eth_rlp_uint32(rlp, (uint32_t*)d);
    } else if (*d <= 0xffffffffffffffff) {
      return eth_rlp_uint64(rlp, d);
    }

    return ETH_ERR_INVALID_ARGS;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    offset = rlp->cbuf->offset;

    if ((op = eth_rlp_len(rlp, &len, &base)) != ETH_OK)
      return op;

    rlp->cbuf->offset = offset;

    if (len <= 1)
      return eth_rlp_uint8(rlp, (uint8_t*)d);
    else if (len <= 2)
      return eth_rlp_uint16(rlp, (uint16_t*)d);
    else if (len <= 4)
      return eth_rlp_uint32(rlp, (uint32_t*)d);
    else if (len <= 8)
      return eth_rlp_uint64(rlp, (uint64_t*)d);

    return ETH_ERR_INVALID_ARGS;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_address(struct eth_rlp *rlp, char **addr) {
  ETH_OP op;
  int hexlen;

  if (rlp == NULL || addr == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (rlp->m == ETH_RLP_ENCODE) {
    if (strlen(*addr) == 0) {
      uint8_t empty_address = 0x00;
      uint8_t *empty_address_ptr = &empty_address;
      size_t size = 1;
      return eth_rlp_bytes(rlp, &empty_address_ptr, &size);
    }

    if (eth_is_address(*addr) <= 0)
      return ETH_ERR_INVALID_ARGS;

    if (strncmp(*addr, "0x", 2) == 0)
      *addr += 2;

    hexlen = 40;
    if ((op = eth_rlp_hex(rlp, addr, &hexlen)) != ETH_OK)
      return op;

    return ETH_OK;
  }

  if (rlp->m == ETH_RLP_DECODE) {
    uint8_t *buf;
    size_t hsize;

    if ((op = eth_rlp_bytes(rlp, &buf, &hsize)) != ETH_OK)
      return op;

    if (hsize == 0 && buf[0] == 0x0) {
      *addr = strdup("");
      free(buf);
      return ETH_OK;
    }

    if ((hsize = (size_t)eth_hex_from_bytes(addr, buf, hsize)) <= 0) {
      free(buf);
      return ETH_ERR_INVALID_ARGS;
    }

    free(buf);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_rlp_to_hex(char **dest, struct eth_rlp *src) {
  struct ethc_rlp_buffer *cbuf;
  char *buf;
  int hsize;

  if (dest == NULL || src == NULL)
    return ETH_ERR_INVALID_ARGS;

  cbuf = src->cbuf;

  hsize = eth_hex_from_bytes(&buf, (uint8_t*)cbuf->rawbuf, cbuf->offset);
  if (hsize <= 0)
    return ETH_ERR_INVALID_ARGS;

  *dest = buf;

  return ETH_OK;
}

ETH_OP eth_rlp_to_bytes(uint8_t **bytes, size_t *len, struct eth_rlp *src) {
  struct ethc_rlp_buffer *cbuf;
  uint8_t *buf;

  if (bytes == NULL || len == NULL || src == NULL)
    return ETH_ERR_INVALID_ARGS;

  cbuf = src->cbuf;

  buf = (uint8_t*)malloc(cbuf->len);
  if (buf == NULL)
    return ETH_ERR_BUFFER_ALLOC;

  memcpy(buf, cbuf->rawbuf, cbuf->offset);

  *bytes = buf;
  *len = cbuf->offset;

  return ETH_OK;
}

ETH_OP eth_rlp_from_hex(struct eth_rlp *dest, char *hex, int len) {
  ETH_OP op;
  struct ethc_rlp_buffer *nbuf;
  uint8_t *buf;
  int buflen;

  if (dest == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (len < 0)
    len = (int)strlen(hex); /* TODO: NOT SAFE */

  if ((buflen = eth_hex_to_bytes((uint8_t**)&buf, hex, len)) <= 0)
    return ETH_ERR_INVALID_ARGS;

  if ((op = ethc_rlp_buffer_init(&nbuf, buf, buflen)) != ETH_OK)
    return op;

  dest->cbuf = nbuf;
  dest->m = ETH_RLP_DECODE;

  return ETH_OK;
}

ETH_OP eth_rlp_free(struct eth_rlp *dest) {
  if (dest == NULL)
    return ETH_ERR_INVALID_ARGS;

  free(dest->cbuf->rawbuf);

  return ETH_OK;
}
