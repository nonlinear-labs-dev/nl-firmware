#pragma once
#include <stdint.h>

extern uint32_t AT_lastAftertouch;

void      AT_Init(void);
uint16_t  AT_GetATAdcDataSize(void);
uint16_t *AT_GetATAdcData(void);
void      AT_Select_AftertouchTable(uint16_t const curve);
void      AT_ProcessAftertouch(void);
