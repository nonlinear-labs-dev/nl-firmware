#pragma once

#include <stdint.h>

// --- Aftertouch: ADC value to Force conversion (this is the general characteristic curve)
// ADC values (input, 0...4096)
static int16_t AT_adcToForceTableX[16] = { 0, 1196, 1992, 2530, 2816, 3002, 3202, 3301, 3368, 3412, 3474, 3512, 3541, 3557, 3570, 3600 };
// Force values (output, Milli-Newton units)
static int16_t AT_adcToForceTableY[16] = { 5300, 6000, 6500, 7000, 7500, 8000, 9000, 10000, 11000, 12000, 14000, 16000, 18000, 20000, 22000, 26000 };
