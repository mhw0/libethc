#include <stdlib.h>
#include <string.h>
#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

#define ethc_abi_buf_pr64(dest, framebuf, offset) \
  dest = (uint64_t)framebuf->buf[offset + 24] << 0x38; \
  dest |= (uint64_t)framebuf->buf[offset + 25] << 0x30; \
  dest |= (uint64_t)framebuf->buf[offset + 26] << 0x28; \
  dest |= (uint64_t)framebuf->buf[offset + 27] << 0x20; \
  dest |= (uint64_t)framebuf->buf[offset + 28] << 0x18; \
  dest |= framebuf->buf[offset + 29] << 0x10; \
  dest |= framebuf->buf[offset + 30] << 0x08; \
  dest |= framebuf->buf[offset + 31];

#define ethc_abi_buf_pr32(dest, framebuf, offset) \
  dest = (uint64_t)framebuf->buf[offset + 28] << 0x18; \
  dest |= framebuf->buf[offset + 29] << 0x10; \
  dest |= framebuf->buf[offset + 30] << 0x08; \
  dest |= framebuf->buf[offset + 31];

#define ethc_abi_buf_pr16(dest, framebuf, offset) \
  dest = framebuf->buf[offset + 30] << 0x08; \
  dest |= framebuf->buf[offset + 31];

#define ethc_abi_buf_pr8(dest, framebuf, offset) \
  dest = framebuf->buf[offset + 31];

#define ethc_abi_buf_pw64(framebuf, src, offset) \
  framebuf->buf[offset + 24] = (src >> 0x38) & 0xFF; \
  framebuf->buf[offset + 25] = (src >> 0x30) & 0xFF; \
  framebuf->buf[offset + 26] = (src >> 0x28) & 0xFF; \
  framebuf->buf[offset + 27] = (src >> 0x20) & 0xFF; \
  framebuf->buf[offset + 28] = (src >> 0x18) & 0xFF; \
  framebuf->buf[offset + 29] = (src >> 0x10) & 0xFF; \
  framebuf->buf[offset + 30] = (src >> 0x08) & 0xFF; \
  framebuf->buf[offset + 31] = (src & 0xFF);

#define ethc_abi_buf_pw32(framebuf, src, offset) \
  framebuf->buf[offset + 28] = (src >> 0x18) & 0xFF; \
  framebuf->buf[offset + 29] = (src >> 0x10) & 0xFF; \
  framebuf->buf[offset + 30] = (src >> 0x08) & 0xFF; \
  framebuf->buf[offset + 31] = (src & 0xFF);

#define ethc_abi_buf_pw16(framebuf, src, offset) \
  framebuf->buf[offset + 30] = (src >> 0x08) & 0xFF; \
  framebuf->buf[offset + 31] = (src & 0xFF);

#define ethc_abi_buf_pw8(framebuf, src, offset) \
  framebuf->buf[offset + 31] = (src & 0xFF);

int ethc_abi_buf_init(struct ethc_abi_buf **dest, size_t size) {
  struct ethc_abi_buf *bbuf;
  uint8_t *buf;

  bbuf = (struct ethc_abi_buf*)malloc(sizeof(struct ethc_abi_buf));
  if (bbuf == NULL)
    return -1;

  buf = calloc(size, sizeof(uint8_t));
  if (buf == NULL) {
    free(bbuf);
    return -1;
  }
  
  bbuf->buf = buf;
  bbuf->len = 0;
  bbuf->offset = 0;
  *dest = bbuf;
  return 1;
}

int ethc_abi_frame_init(struct ethc_abi_frame **frame) {
  struct ethc_abi_frame *nframe;
  struct ethc_abi_buf *fbuf;

  nframe = (struct ethc_abi_frame*)malloc(sizeof(struct ethc_abi_frame));
  if (nframe == NULL)
    return -1;

  if (ethc_abi_buf_init(&fbuf, ETHC_ABI_BUFFER_INITIAL_SIZE) < 0)
    return -1;

  nframe->buf = fbuf;
  nframe->pframe = NULL;
  nframe->dybuflen = 0;
  nframe->len = 0;
  *frame = nframe;
  return 1;
}

int ethc_abi_frame_backpatch(struct ethc_abi_frame *frame) {
  struct ethc_abi_buf *framebuf, *dybuf;
  uint64_t dyoffset, i;

  framebuf = frame->buf;

  for (i = 0; i < frame->dybuflen; i++) {
    dybuf = frame->dybufs[i];

    dyoffset = frame->pframe == NULL
      ? framebuf->offset
      : framebuf->offset - ETH_ABI_WORD_SIZE;

    /* backpatch the offset for the dynamic buffer (can up to 2**64) */
    ethc_abi_buf_pw64(framebuf, dyoffset, dybuf->doffset);

    memcpy(&(framebuf->buf[framebuf->offset]), dybuf->buf, dybuf->len);
    framebuf->len += dybuf->len;
    framebuf->offset += dybuf->len;

    free(dybuf);
  }

  frame->dybuflen = 0;
  return 1;
}

int eth_abi_init(struct eth_abi *abi, int m) {
  struct ethc_abi_frame *nframe;

  if (abi == NULL)
    return -1;

  if (ethc_abi_frame_init(&nframe) < 0)
    return -1;

  abi->m = m;
  abi->cframe = nframe;
  return 1;
}

int eth_abi_free(struct eth_abi *abi) {
  if (abi == NULL)
    return -1;

  if (abi->cframe != NULL) {
    struct ethc_abi_frame *frame = abi->cframe;
    while (frame) {
      int i;
      struct ethc_abi_frame *curr = frame;

      if (frame->buf != NULL) {
        if (frame->buf->buf != NULL) {
          free(frame->buf->buf);
          frame->buf->buf = NULL;
        }
        free(frame->buf);
        frame->buf = NULL;
      }

      for (i = 0; i < frame->dybuflen; i++) {
        struct ethc_abi_buf *dybuf = frame->dybufs[i];
        if (dybuf != NULL) {
          if (dybuf->buf != NULL) {
            free(dybuf->buf);
            dybuf->buf = NULL;
          }
          free(dybuf);
          dybuf = NULL;
        }
      }

      frame = frame->pframe;
      free(curr);
      curr = NULL;
    }
  }
  return 1;
}

int eth_abi_array(struct eth_abi *abi, uint64_t *len) {
  struct ethc_abi_frame *cframe, *nframe;
  struct ethc_abi_buf *cframebuf, *nframebuf;
  uint64_t dyoffset, framelen;

  if (abi == NULL)
    return -1;

  cframe = abi->cframe;
  cframebuf = cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    if (ethc_abi_frame_init(&nframe) < 0)
      return -1;

    nframebuf = nframe->buf;
    nframebuf->doffset = cframebuf->offset;

    /* write offset */
    memset(cframebuf->buf + cframebuf->offset, 0, ETH_ABI_WORD_SIZE);
    cframebuf->len += ETH_ABI_WORD_SIZE;
    cframebuf->offset += ETH_ABI_WORD_SIZE;

    /* write length */
    memset(nframebuf->buf + nframebuf->offset, 0, ETH_ABI_WORD_SIZE);
    nframebuf->len += ETH_ABI_WORD_SIZE;
    nframebuf->offset += ETH_ABI_WORD_SIZE;

    cframe->dybufs[cframe->dybuflen++] = nframe->buf;
    nframe->pframe = cframe;
    abi->cframe = nframe;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr64(dyoffset, cframebuf, cframebuf->offset);
    dyoffset = cframebuf->offset % 32 != 0 ? dyoffset + 4 : dyoffset;
    ethc_abi_buf_pr64(framelen, cframebuf, dyoffset);

    nframebuf = (struct ethc_abi_buf*)malloc(sizeof(struct ethc_abi_buf));
    if (nframebuf == NULL)
      return -1;
    
    nframe = (struct ethc_abi_frame*)malloc(sizeof(struct ethc_abi_frame));
    if (nframe == NULL) {
      free(nframebuf);
      return -1;
    }

    if (len != NULL)
      *len = framelen;

    nframebuf->buf = &(cframebuf->buf[dyoffset + 32]);
    nframebuf->len = framelen * 32;
    nframebuf->offset = 0;
    nframebuf->doffset = cframebuf->offset;

    nframe->pframe = cframe;
    nframe->buf = nframebuf;
    nframe->len = framelen;
    
    abi->cframe = nframe;
    cframebuf->offset += 32;
    return 1;
  }

  return -1;
}

int eth_abi_array_end(struct eth_abi *abi) {
  struct ethc_abi_buf *cframebuf;
  struct ethc_abi_frame *cframe;
  uint64_t asize;

  cframe = abi->cframe;
  cframebuf = cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    asize = cframebuf->len / ETH_ABI_WORD_SIZE - 1;

    /* backpatch the length */
    ethc_abi_buf_pw64(cframebuf, asize, 0);

    ethc_abi_frame_backpatch(abi->cframe);
    abi->cframe = abi->cframe->pframe;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    abi->cframe = abi->cframe->pframe;
    return 1;
  }

  return -1;
}

int eth_abi_bool(struct eth_abi *abi, uint8_t *b) {
  uint8_t u8bool;

  if (abi == NULL || b == NULL)
    return -1;

  if (abi->m == ETH_ABI_ENCODE) {
    u8bool = *b == 0 ? 0 : 1;
    return eth_abi_uint8(abi, &u8bool);
  }

  if (abi->m == ETH_ABI_DECODE)
    return eth_abi_uint8(abi, b);

  return -1;
}

int eth_abi_address(struct eth_abi *abi, char **addr) {
  struct ethc_abi_frame *cframe;
  struct ethc_abi_buf *cframebuf;
  uint8_t *tmp;

  cframe = abi->cframe;
  cframebuf = cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    if (eth_is_hex(*addr, 42) <= 0)
      return -1;

    if (eth_hex_to_bytes(&tmp, *addr, 42) < 0)
      return -1;

    memcpy(&(cframebuf->buf[cframebuf->offset + 12]), tmp, 20);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    tmp = &(cframebuf->buf[cframebuf->offset + 12]);

    if (eth_hex_from_bytes(addr, tmp, 20) < 0)
      return -1;

    cframebuf->offset += 32;

    return 1;
  }
  return -1;
}

int eth_abi_call(struct eth_abi *abi, char **fn, int *len) {
  struct ethc_abi_buf *cframebuf;
  uint8_t keccak[32];
  int fnlen;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    if (len == NULL)
      fnlen = strlen(*fn);

    if (eth_keccak256(keccak, (uint8_t*)*fn, fnlen) < 0)
      return -1;

    memcpy(&(cframebuf->buf[cframebuf->offset]), keccak, 4);
    cframebuf->buf += 4;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    fnlen = eth_hex_from_bytes(fn, &(cframebuf->buf[cframebuf->offset]), 4);
    if (fnlen < 0)
      return -1;

    if (len != NULL)
      *len = fnlen;
    cframebuf->offset += 4;
    return 1;
  }

  return -1;
}

int eth_abi_call_end(struct eth_abi *abi) {
  struct ethc_abi_frame *cframe;
  struct ethc_abi_buf *cframebuf;

  cframe = abi->cframe;
  cframebuf = cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_frame_backpatch(cframe);
    cframebuf->buf -= 4;
    cframebuf->len += 4;
    cframebuf->offset += 4;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE)
    return 1;

  return -1;
}

int eth_abi_uint8(struct eth_abi *abi, uint8_t *d) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buf_pw8(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr8(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_uint16(struct eth_abi *abi, uint16_t *d) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buf_pw16(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr16(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_uint32(struct eth_abi *abi, uint32_t *d) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buf_pw32(cframebuf, *d, cframebuf->offset);
    cframebuf->offset +=  ETH_ABI_WORD_SIZE;
    cframebuf->len +=  ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr32(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_uint64(struct eth_abi *abi, uint64_t *d) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buf_pw64(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr64(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_int8(struct eth_abi *abi, int8_t *d) {
  struct ethc_abi_buf *cframebuf;
  int f;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    f = *d & 0x8000 ? 0xFF : 0x00;
    memset(&(cframebuf->buf[cframebuf->offset]), f, ETH_ABI_WORD_SIZE - 1);
    cframebuf->offset += ETH_ABI_WORD_SIZE - 1;
    cframebuf->buf[cframebuf->offset++] = *d;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    cframebuf->offset += ETH_ABI_WORD_SIZE - 1;
    *d = cframebuf->buf[cframebuf->offset++];
    return 1;
  }

  return -1;
}

int eth_abi_int16(struct eth_abi *abi, int16_t *d) {
  struct ethc_abi_buf *cframebuf;
  int f;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    f = *d & 0x8000 ? 0xFF : 0x00;
    memset(&(cframebuf->buf[cframebuf->offset]), f, ETH_ABI_WORD_SIZE - 2);
    ethc_abi_buf_pw16(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr16(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_int32(struct eth_abi *abi, int32_t *d) {
  struct ethc_abi_buf *cframebuf;
  int f;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    f = *d & 0x80000000 ? 0xFF : 0x00;
    memset(&(cframebuf->buf[cframebuf->offset]), f, ETH_ABI_WORD_SIZE - 4);
    ethc_abi_buf_pw32(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr32(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_int64(struct eth_abi *abi, int64_t *d) {
  struct ethc_abi_buf *cframebuf;
  int f;

  if (abi == NULL || d == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    f = *d & 0x8000000000000000 ? 0xFF : 0x00;
    memset(&(cframebuf->buf[cframebuf->offset]), f, ETH_ABI_WORD_SIZE - 8);
    ethc_abi_buf_pw64(cframebuf, *d, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buf_pr64(*d, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE ;
    return 1;
  }

  return -1;
}

int eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || bytes == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframebuf->buf[cframebuf->offset]), bytes, 8);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframebuf->buf[cframebuf->offset]), 8);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || bytes == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframebuf->buf[cframebuf->offset]), bytes, 16);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframebuf->buf[cframebuf->offset]), 16);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || bytes == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframebuf->buf[cframebuf->offset]), bytes, 32);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframebuf->buf[cframebuf->offset]), 32);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_bytes64(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_buf *cframebuf;

  if (abi == NULL || bytes == NULL)
    return -1;

  cframebuf = abi->cframe->buf;

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframebuf->buf[cframebuf->offset]), bytes, 64);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;
    return 1;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframebuf->buf[cframebuf->offset]), 64);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    return 1;
  }

  return -1;
}

int eth_abi_mpint(struct eth_abi *abi, mpz_t mpz) {
  uint8_t bytes[32] = {0};
  size_t size;
  mpz_t mpztmp, mpzmask;

  if (abi->m == ETH_ABI_ENCODE) {
    mpz_init_set_str(mpzmask, "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", 0);

    /* keep only 256 bits (one word) */
    mpz_init(mpztmp);
    mpz_and(mpztmp, mpz, mpzmask);

    size = mpz_sizeinbase(mpztmp, 16);
    size = (size % 2 == 0 ? size : size + 1) / 2;
    mpz_export((bytes + 32) - size, NULL, 1, sizeof(uint8_t), 0, 0, mpztmp);
    mpz_clears(mpztmp, mpzmask, NULL);

    return eth_abi_bytes32(abi, bytes);
  }

  if (abi->m == ETH_ABI_DECODE) {
    if (eth_abi_bytes32(abi, bytes) == 0)
      return 0;

    mpz_import(mpz, 32, 1, sizeof(uint8_t), 0, 0, bytes);
    return 1;
  }

  return 0;
}

int eth_abi_bytes(struct eth_abi *abi, uint8_t **bytes, size_t *len) {
  struct ethc_abi_frame *cframe;
  struct ethc_abi_buf *cframebuf, *dybuf;
  uint64_t dyoffset, blen;
  uint8_t *buf;
  size_t bsize;
  
  cframe = abi->cframe;
  cframebuf = cframe->buf;
  
  if (abi->m == ETH_ABI_ENCODE) {
    /* make the arbitrary length 32-byte aligned (16->32, 33->64) */
    bsize = *len % ETH_ABI_WORD_SIZE
      ? *len + (ETH_ABI_WORD_SIZE - (*len % ETH_ABI_WORD_SIZE))
      : *len;

    if (ethc_abi_buf_init(&dybuf, ETH_ABI_WORD_SIZE + bsize) < 0)
      return -1;

    /* store the declaration offset for the dynamic buffer */
    dybuf->doffset = cframebuf->offset;

    memset(&(cframebuf->buf[cframebuf->offset]), 0, ETH_ABI_WORD_SIZE);
    cframebuf->offset += ETH_ABI_WORD_SIZE;
    cframebuf->len += ETH_ABI_WORD_SIZE;

    /* write the length */
    ethc_abi_buf_pw64(dybuf, *len, 0);
    dybuf->offset += ETH_ABI_WORD_SIZE;
    dybuf->len += ETH_ABI_WORD_SIZE;

    memcpy(&(dybuf->buf[dybuf->offset]), *bytes, *len);
    dybuf->offset += bsize;
    dybuf->len += bsize;

    cframe->dybufs[cframe->dybuflen++] = dybuf;
    return 1;
  }
  
  if (abi->m == ETH_ABI_DECODE) {
    /* read the offset */
    ethc_abi_buf_pr64(dyoffset, cframebuf, cframebuf->offset);
    cframebuf->offset += ETH_ABI_WORD_SIZE;

    /* read the length */
    ethc_abi_buf_pr64(blen, cframebuf, dyoffset);

    buf = (uint8_t*)malloc(blen);
    if (buf == NULL)
      return -1;

    memcpy(buf, &(cframebuf->buf[cframebuf->offset + dyoffset]), blen);
    *bytes = buf;

    if (len != NULL)
      *len = blen;
    return 1;
  }
  
  return -1;
}

int eth_abi_from_hex(struct eth_abi *abi, char *hex, int len) {
  struct ethc_abi_frame *nframe;

  if (abi == NULL || abi == NULL)
    return -1;

  if (ethc_abi_frame_init(&nframe) < 0)
    return -1;

  if ((len = eth_hex_to_bytes(&(nframe->buf->buf), hex, len)) < 0)
    return -1;

  nframe->pframe = NULL;
  nframe->buf->len = len;
  abi->m = ETH_ABI_DECODE;
  abi->cframe = nframe;
  return 1;
}

int eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len) {
  struct ethc_abi_buf *cframebuf;
  int hexlen;

  cframebuf = abi->cframe->buf;

  if (abi == NULL || hex == NULL || len == NULL)
    return -1;

  ethc_abi_frame_backpatch(abi->cframe);

  if ((hexlen = eth_hex_from_bytes(hex, cframebuf->buf, cframebuf->len)) < 0)
    return -1;

  *len = hexlen;
  return 1;
}
