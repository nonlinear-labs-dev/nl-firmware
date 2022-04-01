/******************************************************************************/
/** @file		nl_stdlib.h
    @date		2020-05-07
    @version	0
    @author		KSTR
    @brief		some common functions
*******************************************************************************/
#pragma once

#include <stdint.h>
#include <stddef.h>

static inline void *memset(void *const pMem, uint8_t const c, size_t n)
{
  uint8_t *p = (uint8_t *) pMem;
  while (n--)
    *(p++) = c;
  return pMem;
}

static inline void *memcpy(void *dest, void *src, size_t n)
{
  uint8_t *p = (uint8_t *) dest;
  while (n--)
    *(p++) = *((uint8_t *) src++);
  return dest;
}

static inline int memcmp32(void *data1, void *data2, uint16_t count)
{
  while (count--)
  {
    if (*(int32_t *) (data1++) == *(int32_t *) (data2++))
      continue;
    return (*(int32_t *) (data1++) < *(int32_t *) (data2++) ? -1 : 1);
  }
  return 0;
}

static inline int16_t abs(int16_t const x)
{
  return (x >= 0) ? x : -x;
}

#pragma GCC diagnostic error "-Wdiv-by-zero"
#define SIZECHECK(item, size)         \
  {                                   \
    if (1 / (sizeof(item) == (size))) \
      ;                               \
  }
