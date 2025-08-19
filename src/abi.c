#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

#define ethc_abi_read8(dest, buf, offset) \
  dest = buf[offset + 31];

#define ethc_abi_read16(dest, buf, offset) \
  dest = buf[offset + 30] << 0x08; \
  dest |= buf[offset + 31];

#define ethc_abi_read32(dest, buf, offset) \
  dest = (uint64_t)buf[offset + 28] << 0x18; \
  dest |= buf[offset + 29] << 0x10; \
  dest |= buf[offset + 30] << 0x08; \
  dest |= buf[offset + 31];

#define ethc_abi_read64(dest, buf, offset) \
  dest = (uint64_t)buf[offset + 24] << 0x38; \
  dest |= (uint64_t)buf[offset + 25] << 0x30; \
  dest |= (uint64_t)buf[offset + 26] << 0x28; \
  dest |= (uint64_t)buf[offset + 27] << 0x20; \
  dest |= (uint64_t)buf[offset + 28] << 0x18; \
  dest |= buf[offset + 29] << 0x10; \
  dest |= buf[offset + 30] << 0x08; \
  dest |= buf[offset + 31];

#define ethc_abi_write8(buf, offset, src) \
  buf[offset + 31] = (src & 0xFF);

#define ethc_abi_write16(buf, offset, src) \
  buf[offset + 30] = (src >> 0x08) & 0xFF; \
  buf[offset + 31] = (src & 0xFF);

#define ethc_abi_write32(buf, offset, src) \
  buf[offset + 28] = (src >> 0x18) & 0xFF; \
  buf[offset + 29] = (src >> 0x10) & 0xFF; \
  buf[offset + 30] = (src >> 0x08) & 0xFF; \
  buf[offset + 31] = (src & 0xFF);

#define ethc_abi_write64(buf, offset, src) \
  buf[offset + 24] = (src >> 0x38) & 0xFF; \
  buf[offset + 25] = (src >> 0x30) & 0xFF; \
  buf[offset + 26] = (src >> 0x28) & 0xFF; \
  buf[offset + 27] = (src >> 0x20) & 0xFF; \
  buf[offset + 28] = (src >> 0x18) & 0xFF; \
  buf[offset + 29] = (src >> 0x10) & 0xFF; \
  buf[offset + 30] = (src >> 0x08) & 0xFF; \
  buf[offset + 31] = (src & 0xFF);

static void ethc_abi_stack_push(struct eth_abi *abi, struct ethc_abi_frame *frame) {
  abi->stack[++(abi->stack_top)] = frame;
}

static void ethc_abi_stack_peek(struct ethc_abi_frame **dest, struct eth_abi *abi) {
  *dest = abi->stack_top < 0 ? NULL : abi->stack[abi->stack_top];
}

static void ethc_abi_stack_pop(struct ethc_abi_frame **dest, struct eth_abi *abi) {
  *dest = abi->stack[(abi->stack_top)--];
}

static void ethc_abi_frame_alloc(struct ethc_abi_frame **dest, struct eth_abi *abi) {
  *dest = &(abi->pool[abi->pool_head++]);
}

ETH_OP eth_abi_init(struct eth_abi *abi, enum eth_abi_mode m) {
  struct ethc_abi_frame *nframe = NULL;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_frame_alloc(&nframe, abi);

  abi->stack_top = -1;
  abi->m = m;

  ethc_abi_stack_push(abi, nframe);

  return ETH_OK;
}

ETH_OP eth_abi_bool(struct eth_abi *abi, uint8_t *b) {
  uint8_t bint;

  if (abi == NULL || b == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (abi->m == ETH_ABI_ENCODE) {
    bint = *b == 0 ? 0 : 1;

    return eth_abi_uint8(abi, &bint);
  }

  if (abi->m == ETH_ABI_DECODE)
    return eth_abi_uint8(abi, b);

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint8(struct eth_abi *abi, uint8_t *d) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_write8(cframe->head, cframe->head_offset, *d);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read8(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint16(struct eth_abi *abi, uint16_t *d) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_write16(cframe->head, cframe->head_offset, *d);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read16(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint32(struct eth_abi *abi, uint32_t *d) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_write32(cframe->head, cframe->head_offset, *d);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read32(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint64(struct eth_abi *abi, uint64_t *d) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_write64(cframe->head, cframe->head_offset, *d);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read64(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int8(struct eth_abi *abi, int8_t *d) {
  struct ethc_abi_frame *cframe = NULL;
  int fill = 0;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000 ? 0xFF : 0x00;

    ethc_abi_write8(cframe->head, cframe->head_offset, *d);

    memset(&(cframe->head[cframe->head_offset]), fill, ETH_ABI_WORD_SIZE - 1);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    cframe->tail_offset += ETH_ABI_WORD_SIZE - 1;

    *d = cframe->tail[cframe->tail_offset++];

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int16(struct eth_abi *abi, int16_t *d) {
  struct ethc_abi_frame *cframe = NULL;
  int fill = 0;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000 ? 0xFF : 0x00;

    ethc_abi_write16(cframe->head, cframe->head_offset, *d);

    memset(&(cframe->head[cframe->head_offset]), fill, ETH_ABI_WORD_SIZE - 2);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read16(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int32(struct eth_abi *abi, int32_t *d) {
  struct ethc_abi_frame *cframe = NULL;
  int fill = 0;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x80000000 ? 0xFF : 0x00;

    ethc_abi_write32(cframe->head, cframe->head_offset, *d);

    memset(&(cframe->head[cframe->head_offset]), fill, ETH_ABI_WORD_SIZE - 4);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read32(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int64(struct eth_abi *abi, int64_t *d) {
  struct ethc_abi_frame *cframe = NULL;
  int fill = 0;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000000000000000 ? 0xFF : 0x00;

    ethc_abi_write64(cframe->head, cframe->head_offset, *d);

    memset(&(cframe->head[cframe->head_offset]), fill, ETH_ABI_WORD_SIZE - 8);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read64(*d, cframe->tail, cframe->tail_offset);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_mpint(struct eth_abi *abi, mp_int *mpint) {
  struct ethc_abi_frame *cframe = NULL;
  uint8_t buf[32] = {0}, size = 0;
  mp_int mask, tmpint;
  ETH_OP op;

  if (abi == NULL || mpint == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    if (mp_init(&tmpint) != MP_OKAY)
      return ETH_ERR_MP;

    if (mp_copy(mpint, &tmpint) != MP_OKAY)
      return ETH_ERR_MP;

    // if the mpint is a negative number
    if (mp_cmp_d(&tmpint, 0) == MP_LT) {
      if (mp_init(&mask) != MP_OKAY) {
        mp_clear(&tmpint);
        return ETH_ERR_MP;
      }

      if (mp_2expt(&mask, 256) != MP_OKAY) {
        mp_clear_multi(&tmpint, &mask, NULL);
        return ETH_ERR_MP;
      }

      if (mp_add(&tmpint, &mask, &tmpint) != MP_OKAY) {
        mp_clear_multi(&tmpint, &mask, NULL);
        return ETH_ERR_MP;
      }
    }

    // do not accept integers that consume more than 32 bytes
    // TODO(mhw0): clear the pow256 if this fails
    size = mp_ubin_size(&tmpint);
    if (size == 0 || size > 32) {
      mp_clear(&tmpint);
      return ETH_ERR_INVALID_ARGS;
    }

    // TODO(mhw0): clear the pow256 if this fails
    if (mp_to_ubin(&tmpint, buf + (32 - size), 32, NULL) != MP_OKAY) {
      mp_clear(&tmpint);
      return ETH_ERR_MP;
    }

    // TODO(mhw0): clear the pow256 if this fails
    mp_clear(&tmpint);
    return eth_abi_bytes32(abi, buf);
  }

  if (abi->m == ETH_ABI_DECODE) {
    if ((op = eth_abi_bytes32(abi, buf)) != ETH_OK)
      return op;

    if (mp_from_ubin(mpint, buf, 32) != MP_OKAY)
      return ETH_ERR_MP;

    // if the most significant bit is set (is negative)
    if (buf[0] & 0x80) {
      if (mp_init(&mask) != MP_OKAY)
        return ETH_ERR_MP;

      if (mp_2expt(&mask, 256) != MP_OKAY) {
        mp_clear(&mask);
        return ETH_ERR_MP;
      }

      if (mp_sub(mpint, &mask, mpint) != MP_OKAY) {
        mp_clear(&mask);
        return ETH_ERR_MP;
      }

      mp_clear(&mask);
      return ETH_OK;
    }

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_OK;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframe->head[cframe->head_offset]), bytes, 8);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframe->tail[cframe->tail_offset]), 8);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_OK;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframe->head[cframe->head_offset]), bytes, 16);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframe->tail[cframe->tail_offset]), 16);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_frame *cframe = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(cframe->head[cframe->head_offset]), bytes, 32);

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(cframe->tail[cframe->tail_offset]), 32);

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes(struct eth_abi *abi, uint8_t *bytes, size_t *len) {
  struct ethc_abi_frame *cframe = NULL, *pframe = NULL;
  size_t offset = 0, byteslen = 0;

  if (abi == NULL || bytes == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);
  ethc_abi_stack_pop(&pframe, abi);
  
  if (abi->m == ETH_ABI_ENCODE) {
    pframe->patches[pframe->patch_offset].head_offset = pframe->head_offset;

    ethc_abi_write32(pframe->head, pframe->head_offset, 0);
    pframe->head_offset += ETH_ABI_WORD_SIZE;

    pframe->patches[pframe->patch_offset].tail_offset = pframe->tail_offset;
    pframe->patch_offset++;

    ethc_abi_write64(cframe->tail, cframe->tail_offset, *len);
    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    memcpy(cframe->tail + cframe->tail_offset, bytes, *len);

    offset = *len % ETH_ABI_WORD_SIZE
      ? *len + (ETH_ABI_WORD_SIZE - (*len % ETH_ABI_WORD_SIZE))
      : *len;

    cframe->tail_offset += offset;

    ethc_abi_stack_push(abi, pframe);

    return ETH_OK;
  }
  
  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_read64(offset, cframe->tail, cframe->tail_offset);
    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    ethc_abi_read64(byteslen, cframe->tail, offset);
    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    memcpy(bytes, cframe->tail + cframe->tail_offset, byteslen);

    return ETH_OK;
  }
  
  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_address(struct eth_abi *abi, char *addr) {
  struct ethc_abi_frame *cframe = NULL;
  size_t i = 0;

  if (abi == NULL || addr == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    if (strncmp(addr, "0x", 2) == 0)
      addr += 2;

    for (i = 0; i < 20; i++)
      sscanf(addr + 2 * i, "%2hhx", &(cframe->head[cframe->head_offset + 12 + i]));

    cframe->head_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    for (i = 0; i < 20; i++)
      snprintf(addr + (i * 2), 3, "%02x", cframe->tail[cframe->tail_offset + 12 + i]);

    addr[40] = '\0';

    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

static void ethc_abi_frame_backpatch(struct ethc_abi_frame *frame) {
  struct ethc_abi_patch *patch = NULL;
  size_t i = 0, offset = 0;

  for (i = 0; i < frame->patch_offset; i++) {
    patch = &frame->patches[i];
    offset = frame->head_offset + patch->tail_offset;
    ethc_abi_write64(frame->head, patch->head_offset, offset);
  }
}

ETH_OP eth_abi_array(struct eth_abi *abi, size_t *len) {
  struct ethc_abi_frame *cframe = NULL, *nframe = NULL;
  size_t offset = 0, arrlen = 0;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_frame_alloc(&nframe, abi);

    ethc_abi_stack_push(abi, nframe);

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_frame_alloc(&nframe, abi);

    ethc_abi_read64(offset, cframe->tail, cframe->tail_offset);
    cframe->tail_offset += ETH_ABI_WORD_SIZE;

    ethc_abi_read64(arrlen, cframe->tail, offset);

    nframe->tail_size = cframe->tail_size - (offset + ETH_ABI_WORD_SIZE);
    memcpy(nframe->tail, cframe->tail + offset + ETH_ABI_WORD_SIZE, nframe->tail_size);

    if (len != NULL)
      *len = arrlen;

    ethc_abi_stack_push(abi, nframe);

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_array_end(struct eth_abi *abi) {
  struct ethc_abi_frame *pframe = NULL, *cframe = NULL;
  size_t len = 0;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_pop(&cframe, abi);
  ethc_abi_stack_peek(&pframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_frame_backpatch(cframe);

    len = cframe->head_offset / ETH_ABI_WORD_SIZE;

    pframe->patches[pframe->patch_offset].head_offset = pframe->head_offset;

    ethc_abi_write32(pframe->head, pframe->head_offset, 0);
    pframe->head_offset += ETH_ABI_WORD_SIZE;

    pframe->patches[pframe->patch_offset].tail_offset = pframe->tail_offset;
    pframe->patch_offset++;

    ethc_abi_write32(pframe->tail, pframe->tail_offset, len);
    pframe->tail_offset += ETH_ABI_WORD_SIZE;

    memcpy(pframe->tail + pframe->tail_offset, cframe->head, cframe->head_offset);
    pframe->tail_offset += cframe->head_offset;

    memcpy(pframe->tail + pframe->tail_offset, cframe->tail, cframe->tail_offset);
    pframe->tail_offset += cframe->tail_offset;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_call(struct eth_abi *abi, char **func, int *len) {
  struct ethc_abi_frame *cframe = NULL;
  uint8_t keccak[32];
  int funclen = 0;

  if (abi == NULL || func == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (len == NULL)
    funclen = strlen(*func);

  ethc_abi_stack_peek(&cframe, abi);

  if (abi->m == ETH_ABI_ENCODE) {
    if (eth_keccak256(keccak, (uint8_t*)*func, funclen) < 0)
      return ETH_ERR_INVALID_ARGS;

    memcpy(abi->func, keccak, ETH_ABI_FUNCTION_SELECTOR_SIZE);
    abi->func_set = 1;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    if (abi->func_set == 0) {
      *func = NULL;
      return ETH_OK;
    }

    snprintf(*func, 9, "%02x%02x%02x%02x", abi->func[0], abi->func[1], abi->func[2], abi->func[3]);

    if (len != NULL)
      *len = ETH_ABI_FUNCTION_SELECTOR_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}


ETH_OP eth_abi_call_end(struct eth_abi *abi) {
  (void)abi;

  return ETH_OK;
}

ETH_OP eth_abi_from_hex(struct eth_abi *abi, char *hex, int len) {
  struct ethc_abi_frame *nframe = NULL;
  uint8_t *buf = NULL;

  if (abi == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  if (len == -1)
    len = strlen(hex);

  if ((len = eth_hex_to_bytes(&buf, hex, len)) < 0)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_frame_alloc(&nframe, abi);

  if (len % ETH_ABI_WORD_SIZE == ETH_ABI_FUNCTION_SELECTOR_SIZE) {
    memcpy(abi->func, buf, ETH_ABI_FUNCTION_SELECTOR_SIZE);
    abi->func_set = 1;

    memcpy(nframe->tail, buf + ETH_ABI_FUNCTION_SELECTOR_SIZE, len - ETH_ABI_FUNCTION_SELECTOR_SIZE);
  } else {
    memcpy(nframe->tail, buf, len);
  }

  nframe->tail_size = len;

  ethc_abi_stack_push(abi, nframe);

  abi->m = ETH_ABI_DECODE;

  free(buf);

  return ETH_OK;
}

ETH_OP eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len) {
  struct ethc_abi_frame *cframe = NULL;
  uint8_t tmp[ETH_ABI_WORD_SIZE * 128];
  int hexlen = 0, offset = 0;

  if (abi == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_stack_peek(&cframe, abi);

  ethc_abi_frame_backpatch(cframe);

  if (abi->func_set) {
    memcpy(tmp, abi->func, ETH_ABI_FUNCTION_SELECTOR_SIZE);
    offset += ETH_ABI_FUNCTION_SELECTOR_SIZE;
  }

  memcpy(tmp + offset, cframe->head, cframe->head_offset);
  offset += cframe->head_offset;

  memcpy(tmp + offset, cframe->tail, cframe->tail_offset);
  offset += cframe->tail_offset;

  if ((hexlen = eth_hex_from_bytes(hex, tmp, offset)) < 0)
    return ETH_ERR_INVALID_ARGS;

  if (len != NULL)
    *len = hexlen;

  return ETH_OK;
}
