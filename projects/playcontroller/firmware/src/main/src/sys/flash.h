#pragma once
#include <stdint.h>

void FLASH_Init(void);
int  flashMemory(uint32_t const* const buf, uint32_t len, uint8_t const bank);

typedef struct
{
  uint32_t data[4];
} iapUniqueId_t;

int iapGetUniqueId(iapUniqueId_t* pId);  // no need to run FLASH_Init() for this.
