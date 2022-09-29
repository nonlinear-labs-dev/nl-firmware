#pragma once
#include <stdint.h>

void     BNDR_Init(void);
void     BNDR_SetLegacyMode(uint16_t const on);
uint16_t BNDR_GetStatus(void);
uint32_t BNDR_GetLastBender(void);
void     BNDR_Select_BenderTable(uint16_t const curve);
void     BNDR_Select_BenderSensitivity(uint16_t const sensitivity);
void     BNDR_Reset(uint16_t const legacyMode);
void     BNDR_ProcessBender(void);
