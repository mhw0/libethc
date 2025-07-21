#include <stdio.h>
#include <string.h>
#include <ethc/abi.h>
#include <ethc/address.h>
#include <ethc/hex.h>
#include <ethc/keccak256.h>

#define ethc_abi_buffer_padded_read64(dest, buffer, offset) \
  dest = (uint64_t)buffer->rawbuf[offset + 24] << 0x38; \
  dest |= (uint64_t)buffer->rawbuf[offset + 25] << 0x30; \
  dest |= (uint64_t)buffer->rawbuf[offset + 26] << 0x28; \
  dest |= (uint64_t)buffer->rawbuf[offset + 27] << 0x20; \
  dest |= (uint64_t)buffer->rawbuf[offset + 28] << 0x18; \
  dest |= buffer->rawbuf[offset + 29] << 0x10; \
  dest |= buffer->rawbuf[offset + 30] << 0x08; \
  dest |= buffer->rawbuf[offset + 31];

#define ethc_abi_buffer_padded_read32(dest, buffer, offset) \
  dest = (uint64_t)buffer->rawbuf[offset + 28] << 0x18; \
  dest |= buffer->rawbuf[offset + 29] << 0x10; \
  dest |= buffer->rawbuf[offset + 30] << 0x08; \
  dest |= buffer->rawbuf[offset + 31];

#define ethc_abi_buffer_padded_read16(dest, buffer, offset) \
  dest = buffer->rawbuf[offset + 30] << 0x08; \
  dest |= buffer->rawbuf[offset + 31];

#define ethc_abi_buffer_padded_read8(dest, buffer, offset) \
  dest = buffer->rawbuf[offset + 31];

#define ethc_abi_buffer_padded_write64(buf, src, offset) \
  buf->rawbuf[offset + 24] = (src >> 0x38) & 0xFF; \
  buf->rawbuf[offset + 25] = (src >> 0x30) & 0xFF; \
  buf->rawbuf[offset + 26] = (src >> 0x28) & 0xFF; \
  buf->rawbuf[offset + 27] = (src >> 0x20) & 0xFF; \
  buf->rawbuf[offset + 28] = (src >> 0x18) & 0xFF; \
  buf->rawbuf[offset + 29] = (src >> 0x10) & 0xFF; \
  buf->rawbuf[offset + 30] = (src >> 0x08) & 0xFF; \
  buf->rawbuf[offset + 31] = (src & 0xFF);

#define ethc_abi_buffer_padded_write32(buf, src, offset) \
  buf->rawbuf[offset + 28] = (src >> 0x18) & 0xFF; \
  buf->rawbuf[offset + 29] = (src >> 0x10) & 0xFF; \
  buf->rawbuf[offset + 30] = (src >> 0x08) & 0xFF; \
  buf->rawbuf[offset + 31] = (src & 0xFF);

#define ethc_abi_buffer_padded_write16(buffer, src, offset) \
  buffer->rawbuf[offset + 30] = (src >> 0x08) & 0xFF; \
  buffer->rawbuf[offset + 31] = (src & 0xFF);

#define ethc_abi_buffer_padded_write8(buf, src, offset) \
  buf->rawbuf[offset + 31] = (src & 0xFF);

void ethc_abi_type_stack_push(struct ethc_abi_dynamic_type_stack *stack, struct ethc_abi_dynamic_type *buf) {
  stack->stack[stack->head++] = buf;
}

void ethc_abi_type_stack_peek(struct ethc_abi_dynamic_type **dest, struct ethc_abi_dynamic_type_stack *stack) {
  *dest = stack->stack[stack->head - 1];
}

void ethc_abi_type_stack_pop(struct ethc_abi_dynamic_type **dest, struct ethc_abi_dynamic_type_stack *stack) {
  *dest = stack->stack[--stack->head];
}

void ethc_abi_buffer_alloc(struct ethc_abi_dynamic_type **dest, struct eth_abi *abi) {
  *dest = &(abi->typepool[abi->head++]);
}

ETH_OP eth_abi_init(struct eth_abi *abi, enum eth_abi_mode m) {
  struct ethc_abi_dynamic_type *ntype;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  abi->m = m;

  ethc_abi_buffer_alloc(&ntype, abi);
  ethc_abi_type_stack_push(&abi->stack, ntype);

  return ETH_OK;
}

ETH_OP eth_abi_array(struct eth_abi *abi, size_t *len) {
  struct ethc_abi_dynamic_type *ctype = NULL, *ntype = NULL;
  size_t typeoffset = 0, typelen = 0, nsize = 0;

  (void)len;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    // allocate buffer from the pool
    ethc_abi_buffer_alloc(&ntype, abi);

    // push the allocated buffer to "nested stack"
    ethc_abi_type_stack_push(&abi->stack, ntype);

    // push the array to types list
    abi->types[abi->typelen++] = ntype;

    // leave word for the length of the array
    ntype->offset += ETH_ABI_WORD_SIZE;

    // store the starting position of the array in the parent buffer
    ntype->ploc = ctype->offset;

    // leave space for the location of the array in the parent buffer
    ctype->offset += ETH_ABI_WORD_SIZE;

    // store the parent buffer
    ntype->ptype = ctype;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    // allocate new type
    ethc_abi_buffer_alloc(&ntype, abi);

    // read the offset of the type
    ethc_abi_buffer_padded_read64(typeoffset, ctype, ctype->offset);
    ctype->offset += ETH_ABI_WORD_SIZE;

    // read the length of the type
    ethc_abi_buffer_padded_read64(typelen, ctype, typeoffset);
    typeoffset += ETH_ABI_WORD_SIZE;

    // calculate how many bytes we should copy to the new type
    nsize = ctype->size - typeoffset;
    ntype->size = nsize;

    // copy the content to the new type
    memcpy(ntype->rawbuf + ntype->offset, ctype->rawbuf + typeoffset, nsize);

    // enter the type
    ethc_abi_type_stack_push(&abi->stack, ntype);

    // return the length of the array
    if (len != NULL)
      *len = typelen;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_array_end(struct eth_abi *abi) {
  struct ethc_abi_dynamic_type *ctype = NULL, *ptype = NULL;
  size_t arrlen = 0;

  if (abi == NULL)
    return ETH_ERR_INVALID_ARGS;

  // get the current buffer from the stack
  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    // calculate the array length (remove one word allocated for the length)
    arrlen = ((ctype->offset - ETH_ABI_WORD_SIZE) / ETH_ABI_WORD_SIZE);

    // encode the length of the array to the current buffer
    ethc_abi_buffer_padded_write64(ctype, arrlen, 0);

    ethc_abi_type_stack_pop(&ptype, &abi->stack);
    ethc_abi_type_stack_peek(&ptype, &abi->stack);

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_type_stack_pop(&ptype, &abi->stack);
    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}


void ethc_abi_backpatch(struct eth_abi *abi, struct ethc_abi_dynamic_type *buf) {
  struct ethc_abi_dynamic_type *type = NULL, *ptype = NULL;
  size_t i = 0, patchloc = 0, dyloc = 0;

  (void)buf;
  (void)type;

  for (i = 0; i < abi->typelen; i++) {
    type = abi->types[i];
    ptype = type->ptype;

    // save the absolute position of the current buffer
    type->absloc = buf->offset;

    // calculate the location of where we should write its offset to
    patchloc = ptype->absloc + type->ploc;

    // calculate the distance between the parent buffer offset and the current buffer
    dyloc = i == 0
        ? (buf->offset - ptype->absloc)
        : (buf->offset - ptype->absloc) - ETH_ABI_WORD_SIZE;

    // normalize the dynamic type location
    dyloc = dyloc % ETH_ABI_WORD_SIZE != 0
        ? dyloc - ETH_ABI_FUNCTION_SELECTOR_SIZE
        : dyloc;

    // copy the contents of the buffer to given buffer
    memcpy(buf->rawbuf + buf->offset, type->rawbuf, type->offset);
    buf->offset += type->offset;

    // backpatch the offset of this buffer
    ethc_abi_buffer_padded_write64(buf, dyloc, patchloc);
  }
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

ETH_OP eth_abi_address(struct eth_abi *abi, char **addr) {
  struct ethc_abi_dynamic_type *ctype;
  uint8_t *tmp;

  if (abi == NULL || addr == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    if (eth_is_hex(*addr, 42) <= 0)
      return ETH_ERR_INVALID_ARGS;

    if (eth_hex_to_bytes(&tmp, *addr, 42) < 0)
      return ETH_ERR_INVALID_ARGS;

    memcpy(&(ctype->rawbuf[ctype->offset + 12]), tmp, 20);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    tmp = &(ctype->rawbuf[ctype->offset + 12]);

    if (eth_hex_from_bytes(addr, tmp, 20) < 0)
      return ETH_ERR_INVALID_ARGS;

    ctype->offset += 32;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_call(struct eth_abi *abi, char **func, int *len) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  uint8_t keccak[32];
  int funclen = 0;

  if (abi == NULL || func == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    if (len == NULL)
      funclen = strlen(*func);

    if (eth_keccak256(keccak, (uint8_t*)*func, funclen) < 0)
      return ETH_ERR_INVALID_ARGS;

    memcpy(&(ctype->rawbuf[ctype->offset]), keccak, ETH_ABI_FUNCTION_SELECTOR_SIZE);

    ctype->offset += ETH_ABI_FUNCTION_SELECTOR_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    keccak[0] = *(ctype->rawbuf + ctype->offset++);
    keccak[1] = *(ctype->rawbuf + ctype->offset++);
    keccak[2] = *(ctype->rawbuf + ctype->offset++);
    keccak[3] = *(ctype->rawbuf + ctype->offset++);

    snprintf(*func, 9, "%02x%02x%02x%02x", keccak[0], keccak[1], keccak[2], keccak[3]);

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

ETH_OP eth_abi_uint8(struct eth_abi *abi, uint8_t *d) {
  struct ethc_abi_dynamic_type *ctype = NULL;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buffer_padded_write8(ctype, *d, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read8(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint16(struct eth_abi *abi, uint16_t *d) {
  struct ethc_abi_dynamic_type *ctype;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buffer_padded_write16(ctype, *d, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read16(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint32(struct eth_abi *abi, uint32_t *d) {
  struct ethc_abi_dynamic_type *ctype;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buffer_padded_write32(ctype, *d, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read32(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_uint64(struct eth_abi *abi, uint64_t *d) {
  struct ethc_abi_dynamic_type *ctype;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    ethc_abi_buffer_padded_write64(ctype, *d, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read64(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int8(struct eth_abi *abi, int8_t *d) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  int fill;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000 ? 0xFF : 0x00;

    ethc_abi_buffer_padded_write8(ctype, *d, ctype->offset);

    memset(&(ctype->rawbuf[ctype->offset]), fill, ETH_ABI_WORD_SIZE - 1);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ctype->offset += ETH_ABI_WORD_SIZE - 1;

    *d = ctype->rawbuf[ctype->offset++];

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int16(struct eth_abi *abi, int16_t *d) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  int fill;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000 ? 0xFF : 0x00;

    ethc_abi_buffer_padded_write16(ctype, *d, ctype->offset);

    memset(&(ctype->rawbuf[ctype->offset]), fill, ETH_ABI_WORD_SIZE - 2);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read16(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int32(struct eth_abi *abi, int32_t *d) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  int fill;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x80000000 ? 0xFF : 0x00;

    ethc_abi_buffer_padded_write32(ctype, *d, ctype->offset);

    memset(&(ctype->rawbuf[ctype->offset]), fill, ETH_ABI_WORD_SIZE - 4);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read32(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_int64(struct eth_abi *abi, int64_t *d) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  int fill;

  if (abi == NULL || d == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    fill = *d & 0x8000000000000000 ? 0xFF : 0x00;

    ethc_abi_buffer_padded_write64(ctype, *d, ctype->offset);

    memset(&(ctype->rawbuf[ctype->offset]), fill, ETH_ABI_WORD_SIZE - 8);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    ethc_abi_buffer_padded_read64(*d, ctype, ctype->offset);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes8(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_dynamic_type *ctype = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_OK;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(ctype->rawbuf[ctype->offset]), bytes, 8);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(ctype->rawbuf[ctype->offset]), 8);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes16(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_dynamic_type *ctype = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_OK;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(ctype->rawbuf[ctype->offset]), bytes, 16);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(ctype->rawbuf[ctype->offset]), 16);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes32(struct eth_abi *abi, uint8_t *bytes) {
  struct ethc_abi_dynamic_type *ctype = NULL;

  if (abi == NULL || bytes == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  if (abi->m == ETH_ABI_ENCODE) {
    memcpy(&(ctype->rawbuf[ctype->offset]), bytes, 32);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  if (abi->m == ETH_ABI_DECODE) {
    memcpy(bytes, &(ctype->rawbuf[ctype->offset]), 32);

    ctype->offset += ETH_ABI_WORD_SIZE;

    return ETH_OK;
  }

  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_bytes(struct eth_abi *abi, uint8_t *bytes, size_t *len) {
  struct ethc_abi_dynamic_type *ctype = NULL, *ntype = NULL, *buf = NULL;
  size_t dyoffset = 0, typelen = 0;
  // uint8_t *buf;

  if (abi == NULL || bytes == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);
  
  if (abi->m == ETH_ABI_ENCODE) {
    // allocate buffer for the bytes type
    ethc_abi_buffer_alloc(&ntype, abi);

    // write the length of the buffer
    ethc_abi_buffer_padded_write64(ntype, *len, ntype->offset);
    ntype->offset += ETH_ABI_WORD_SIZE;

    // copy the contents of the argument to the new buffer
    memcpy(ntype->rawbuf + ntype->offset, bytes, *len);

    // make the arbitrary length 32-byte aligned (16->32, 33->64)
    dyoffset = *len % ETH_ABI_WORD_SIZE
      ? *len + (ETH_ABI_WORD_SIZE - (*len % ETH_ABI_WORD_SIZE))
      : *len;

    // save the 32-byte aligned offset
    ntype->offset += dyoffset;

    // store the starting position of the array in the parent buffer
    ntype->ploc = ctype->offset;

    // leave space for the location of the array in the current buffer
    ctype->offset += ETH_ABI_WORD_SIZE;

    // save the parent buffer of the new buffer
    ntype->ptype = ctype;

    // add the bytes to dynamic types list
    abi->types[abi->typelen++] = ntype;

    return ETH_OK;
  }
  
  if (abi->m == ETH_ABI_DECODE) {
    // allocate buffer
    ethc_abi_buffer_alloc(&buf, abi);

    // read the location of the buffer
    ethc_abi_buffer_padded_read64(dyoffset, ctype, ctype->offset);
    ctype->offset += ETH_ABI_WORD_SIZE;

    // read the length of the bytes
    ethc_abi_buffer_padded_read64(typelen, ctype, dyoffset);

    // copy the content of the bytes
    memcpy(bytes, ctype->rawbuf + ctype->offset + dyoffset, typelen);

    // return the length
    if (len != NULL)
      *len = typelen;

    return ETH_OK;
  }
  
  return ETH_ERR_INVALID_ARGS;
}

ETH_OP eth_abi_from_hex(struct eth_abi *abi, char *hex, int len) {
  struct ethc_abi_dynamic_type *ntype = NULL;
  uint8_t *buf = NULL;

  if (abi == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_buffer_alloc(&ntype, abi);

  if (len == -1)
    len = strlen(hex);

  if ((len = eth_hex_to_bytes(&buf, hex, len)) < 0)
    return ETH_ERR_INVALID_ARGS;

  memcpy(ntype->rawbuf, buf, len);
  ntype->size = len;

  // enter the type
  ethc_abi_type_stack_push(&abi->stack, ntype);

  abi->m = ETH_ABI_DECODE;

  return ETH_OK;
}

ETH_OP eth_abi_to_hex(struct eth_abi *abi, char **hex, size_t *len) {
  struct ethc_abi_dynamic_type *ctype = NULL;
  int hexlen;

  if (abi == NULL || hex == NULL)
    return ETH_ERR_INVALID_ARGS;

  ethc_abi_type_stack_peek(&ctype, &abi->stack);

  ethc_abi_backpatch(abi, ctype);

  if ((hexlen = eth_hex_from_bytes(hex, ctype->rawbuf, ctype->offset)) < 0)
    return ETH_ERR_INVALID_ARGS;

  *len = hexlen;

  return ETH_OK;
}
