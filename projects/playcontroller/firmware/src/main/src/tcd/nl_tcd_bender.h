#pragma once
#include <stdint.h>

void     BNDR_Init(void);
void     BNDR_SetLegacyMode(int const on);
uint16_t BNDR_GetStatus(void);
uint32_t BNDR_GetLastBender();
void     BNDR_Select_BenderTable(uint16_t const curve);
void     BNDR_Reset();
void     BNDR_ProcessBender(void);
