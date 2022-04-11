#pragma once
#include <stdint.h>

void      AT_Init(void);
void      AT_SetLegacyMode(int const on);
uint32_t  AT_GetLastAftertouch();
uint16_t  AT_GetATAdcDataSize(void);
uint16_t *AT_GetATAdcData(void);
void      AT_Select_AftertouchTable(uint16_t const curve);
void      AT_ProcessAftertouch(void);
void      AT_SetAftertouchCalibration(uint16_t const length, uint16_t *data);
uint16_t  AT_GetStatus(void);
