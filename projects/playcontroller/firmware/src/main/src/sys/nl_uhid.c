#include "nl_uhid.h"

static iapUniqueId_t uhid = {};
uint64_t             NL_uhid64;

void UHID_Init(void)
{
  NL_uhid64 = 0;
  if (!iapGetUniqueId(&uhid))
    return;
  NL_uhid64 = (uint64_t)(uhid.data[0] ^ uhid.data[2]) | (uint64_t)(uhid.data[1] ^ uhid.data[3]) << 32;
}
