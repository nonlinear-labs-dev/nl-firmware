#include <stdint.h>
#include "cmsis/LPC43xx.h"
#include "cmsis/lpc43xx_cgu.h"
#include "flash.h"

static uint32_t commandParam[5];
static uint32_t statusResult[5];

#define IAP_LOCATION *(volatile uint32_t *) (0x10400100);
typedef void (*IAP_t)(uint32_t[], uint32_t[]);
static IAP_t iapEntry;

#define IAP_CMD_SUCCESS (0)

void FLASH_Init(void)
{
  iapEntry = (IAP_t) IAP_LOCATION;
  CGU_UpdateClock();
}

static void iapInit(void)
{
  commandParam[0] = 49;  // CMD : Init
  iapEntry(commandParam, statusResult);
}

static int iapPrepSectorsForWrite(uint32_t const startSector, uint32_t const endSector, uint32_t const bank)
{
  commandParam[0] = 50;  // CMD : Prepare
  commandParam[1] = startSector;
  commandParam[2] = endSector;
  commandParam[3] = bank;
  iapEntry(commandParam, statusResult);
  return statusResult[0] == IAP_CMD_SUCCESS;
}

static int iapCopyRamToFlash(uint32_t const *const src, uint32_t const *const dest, uint32_t const bytes)
{
  commandParam[0] = 51;  // CMD : Write
  commandParam[1] = (uint32_t) dest;
  commandParam[2] = (uint32_t) src;
  commandParam[3] = bytes;
  commandParam[4] = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000;
  iapEntry(commandParam, statusResult);
  return statusResult[0] == IAP_CMD_SUCCESS;
}

static int iapEraseSectors(uint32_t const startSector, uint32_t const endSector, uint32_t const bank)
{
  commandParam[0] = 52;  // CMD : Erase
  commandParam[1] = startSector;
  commandParam[2] = endSector;
  commandParam[3] = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000;
  commandParam[4] = bank;
  iapEntry(commandParam, statusResult);
  return statusResult[0] == IAP_CMD_SUCCESS;
}

int iapGetUniqueId(iapUniqueId_t *pId)
{
  iapEntry        = (IAP_t) IAP_LOCATION;
  commandParam[0] = 58;  // CMD : Read device unique ID
  iapEntry(commandParam, statusResult);
  if ((statusResult[0] == IAP_CMD_SUCCESS) && (pId != NULL))
  {
    pId->data[0] = statusResult[1];
    pId->data[1] = statusResult[2];
    pId->data[2] = statusResult[3];
    pId->data[3] = statusResult[4];
    return 1;
  }
  return 0;
}

// buf : data, on a word boundary
// len : length of data in bytes
// bank: 0/1 (Bank A / Bank B)
// returns 0 on success, else the step # where it failed
int flashMemory(uint32_t const *buf, uint32_t len, uint8_t bank)
{
  static uint32_t *const flashBankAdr[2] = { (uint32_t *) 0x1A000000, (uint32_t *) 0x1B000000 };
  uint32_t               flashBuffer[1024];  // 4kB buffer, residing in the stack to save data segment space

  __disable_irq();
  iapInit();

  // align len to a 4-byte multiple and make it a word count
  while ((len & 3) != 0)
    len++;
  len >>= 2;

  if (len == 0)
    return 1;

  bank               = (bank != 0);
  uint32_t  sector   = 0;
  uint32_t *flashAdr = flashBankAdr[bank];

  // erase flash (40kBytes, 5 sectors 8kBytes each)
  if (!(iapPrepSectorsForWrite(0, 4, bank) && iapEraseSectors(0, 4, bank)))
    return 2;

  while (len > 0)
  {
    // copy a at most 4kBytes chunk into flashBuffer
    uint32_t toCopy = len;
    if (toCopy > 1024)
      toCopy = 1024;
    for (uint32_t i = 0; i < toCopy; i++, buf++)
      flashBuffer[i] = *buf;

    // zero any remaining unused space
    for (uint32_t i = toCopy; i < 1024; i++)
      flashBuffer[i] = 0;

    // prepare affected sector. We need to increment the sector number after every
    // second 4kB block of data as a sector is 8kB, the right-shift affords this
    if (!iapPrepSectorsForWrite(sector >> 1, sector >> 1, bank))
      return 3;
    sector++;

    // flash chunk
    if (!iapCopyRamToFlash(flashBuffer, flashAdr, 4096))
      return 4;

    flashAdr += 1024;  // next 4kB start adr
    len -= toCopy;
  }
  return 0;
}
