#include <stdint.h>
#include "nl_tcd_adc_work.h"
#include "playcontroller/lpc_lib.h"
#include "tcd/nl_tcd_adc_work.h"
#include "ipc/emphase_ipc.h"
#include "playcontroller/playcontroller-defs.h"
#include "playcontroller/playcontroller-converters.h"
#include "sys/nl_eeprom.h"
#include "sys/nl_stdlib.h"
#include "playcontroller/lpc_lib.h"

static BNDR_status_T status;
uint16_t             BNDR_GetStatus(void)
{
  return BNDR_statusToUint16(status);
}

// --------------
static uint32_t BNDR_lastBender = BIPOLAR_CENTER;
uint32_t        BNDR_GetLastBender(void)
{
  return BNDR_lastBender;
}

#define BNDR_LEGACY_DEFAULT (0)
static int legacyMode = BNDR_LEGACY_DEFAULT;
void       BNDR_SetLegacyMode(int const on)
{
  legacyMode        = (on != 0);
  status.legacyMode = legacyMode;
}

#define TABLE_SIZE (32)

// legacy
#define BENDER_DEADRANGE    20    // +/-1 % of +/-2047
#define BENDER_SMALL_THRESH 200   // +/-10 % of +/-2047, test range
#define BENDER_TEST_PERIOD  80    // 1 s = 80 * 12.5 ms
#define BENDER_RAMP_INC     8     // 400 / 8 = 50 steps from the threshold to zero (50 * 12.5 ms = 625 ms)
#define BENDER_FACTOR       4540  // 4540 / 4096 = 2047 / 1847 for saturation = 100 % at 90 % of the input range

static int32_t lastPitchbend = BIPOLAR_CENTER;
static int32_t lastRawPitchbend;
static int32_t pitchbendZero;

static uint16_t pbSignalIsSmall;
static uint16_t pbTestTime;
static uint16_t pbTestMode;
static uint16_t pbRampMode;
static int32_t  pbRamp;
static int32_t  pbRampInc;
static int16_t  allBenderTables[3][TABLE_SIZE + 1] = {};  // contains the bender curves
static int16_t *benderTable;
// legacy

static int16_t             benderTableX[TABLE_SIZE + 1];
static LIB_interpol_data_T BNDR_benderCurves = { TABLE_SIZE + 1, benderTableX, allBenderTables[1] };

/*****************************************************************************
* @brief	ADC_WORK_Select_BenderTable
* @param	0: soft, 1: normal, 2: hard
******************************************************************************/
void BNDR_Select_BenderTable(uint16_t const curve)
{
  if (curve > 2)
    return;
  benderTable                = allBenderTables[curve];
  BNDR_benderCurves.y_values = allBenderTables[curve];
}

static void Generate_BenderTable(uint16_t const curve)
{
  // separate processing of absolute values for positive and negative range

  float s;

  switch (curve)  // s defines the curve shape
  {
    case 0:     // soft
      s = 0.0;  // y = x
      break;
    case 1:     // normal
      s = 0.5;  // y = 0.5 * x + 0.5 * x^3
      break;
    case 2:     // hard
      s = 1.0;  // y = x^3
      break;
    default:
      return;
  }

  float x;

  for (int i = 0; i <= TABLE_SIZE; i++)
  {
    x                         = (float) i / (float) TABLE_SIZE;
    benderTableX[i]           = (uint16_t)(x * BIPOLAR_CENTER);
    allBenderTables[curve][i] = (uint16_t)(x * BIPOLAR_CENTER * ((1.0 - s) + s * x * x));
  }
}

/*****************************************************************************
* @brief  Process Bender
******************************************************************************/
static void SendBenderIfChanged(int32_t value)
{
  if (value != lastPitchbend)
  {
    ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_PITCHBEND, value);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_PITCHBEND, value);
    lastPitchbend = value;
  }
}

static void ProcessLegacyBender(int32_t value)
{
  int32_t valueToSend;

  value = value - pitchbendZero;  // -2048 ... 2047 (after initialization)

  if ((value > -BENDER_SMALL_THRESH) && (value < BENDER_SMALL_THRESH))  // small signals
  {
    if (!pbSignalIsSmall)  // entering the small-signal range
    {
      pbTestTime = 0;
      pbTestMode = 1;  // start testing
    }
    pbSignalIsSmall = 1;
  }
  else  // large signals
  {
    if (pbSignalIsSmall)  // leaving the small-signal range
      pbTestMode = 0;     // discard testing
    pbSignalIsSmall = 0;
  }

  if (pbTestMode)
  {
    if (pbTestTime < BENDER_TEST_PERIOD)
      pbTestTime++;
    else  // test time finished
    {
      pbTestMode = 0;
      pbRampMode = 1;  // start the ramp to zero

      if (value > 0)
      {
        pbRamp    = value;            // absolute amount of shifting
        pbRampInc = BENDER_RAMP_INC;  // positive increment, will shift the zero line up
      }
      else
      {
        pbRamp    = -value;            // absolute amount of shifting
        pbRampInc = -BENDER_RAMP_INC;  // negative increment, will shift the zero line down
      }
    }
  }

  if (pbRampMode)
  {
    pbRamp -= BENDER_RAMP_INC;  // determines the size of the ramp

    if (pbRamp <= 0)               // ramp has reached zero
      pbRampMode = 0;              // and is stopped
    else                           // the steps before the ramp reaches zero
      pitchbendZero += pbRampInc;  // shifting the zero line = negative feedback
  }

  if (value != lastRawPitchbend)
  {
    if (value > BENDER_DEADRANGE)  // is in the positive work range
    {
      valueToSend = value - BENDER_DEADRANGE;     // absolute amount
      valueToSend = valueToSend * BENDER_FACTOR;  // saturation factor

      if (valueToSend > 2047 * 4096)
        valueToSend = BIPOLAR_CENTER;  // clipping
      else
      {
        valueToSend    = valueToSend >> 12;                                                          // 0 ... 2047 (11 Bit)
        uint32_t fract = valueToSend & 0x3F;                                                         // lower 6 bits used for interpolation
        uint32_t index = valueToSend >> 6;                                                           // upper 5 bits (0...31) used as index in the table
        valueToSend    = (benderTable[index] * (64 - fract) + benderTable[index + 1] * fract) >> 6;  // (0...8000) * 64 / 64
      }

      valueToSend = BIPOLAR_CENTER + valueToSend;  // 8001 ... 16000
      SendBenderIfChanged(valueToSend);
    }
    else if (value < -BENDER_DEADRANGE)  // is in the negative work range
    {
      valueToSend = -BENDER_DEADRANGE - value;    // absolute amount
      valueToSend = valueToSend * BENDER_FACTOR;  // saturation factor
      if (valueToSend > 2047 * 4096)
        valueToSend = BIPOLAR_CENTER;  // clipping
      else
      {
        valueToSend    = valueToSend >> 12;                                                          // 0 ... 2047 (11 Bit)
        uint32_t fract = valueToSend & 0x3F;                                                         // lower 6 bits used for interpolation
        uint32_t index = valueToSend >> 6;                                                           // upper 5 bits (0...31) used as index in the table
        valueToSend    = (benderTable[index] * (64 - fract) + benderTable[index + 1] * fract) >> 6;  // (0...8000) * 64 / 64
      }
      valueToSend = BIPOLAR_CENTER - valueToSend;  // 7999 ... 0
      SendBenderIfChanged(valueToSend);
    }
    else  // is in the dead range
    {
      if ((lastRawPitchbend > BENDER_DEADRANGE) || (lastRawPitchbend < -BENDER_DEADRANGE))  // was outside of the dead range before
        SendBenderIfChanged(BIPOLAR_CENTER);
    }
    lastRawPitchbend = value;
  }
}
// legacy end

//
// -----------------------------------------------------------------------------------------------------------
//

#define ADC_RANGE  (4096u)
#define ADC_CENTER (2048u)

// ---------------- begin Value Buffer defs
#define VALBUF_SIZE (32ul)  // 2^N !!! Floating Average is used based on this size
#define VALBUF_MOD  (VALBUF_SIZE - 1)
typedef struct
{
  uint16_t values[VALBUF_SIZE];
  uint16_t index;
  uint16_t invalidCntr;
  uint32_t sum;
} ValueBuffer_T;

static void inline clearValueBuffer(ValueBuffer_T *const this, uint16_t const invalidCount, uint16_t const clearValue)
{
  this->index = 0;
  if (invalidCount < VALBUF_SIZE)
    this->invalidCntr = VALBUF_SIZE;
  else
    this->invalidCntr = invalidCount;
  for (int i = 0; i < VALBUF_SIZE; i++)
    this->values[i] = clearValue;
  this->sum = (uint32_t) clearValue * VALBUF_SIZE;
}

static inline uint16_t addInValueBuffer(ValueBuffer_T *const this, const uint16_t value)
{
  this->index = (this->index + 1) & VALBUF_MOD;
  if (this->invalidCntr)
    this->invalidCntr--;
  this->sum += (int) value - (int) this->values[this->index];
  this->values[this->index] = value;
  return value;
}

static inline int isValueBufferFilled(ValueBuffer_T *const this)
{
  return (this->invalidCntr == 0);
}

static inline void getValueBufferStats(ValueBuffer_T *const this, uint16_t *const pMin, uint16_t *const pMax, uint16_t *const pAvg, uint32_t *const pRmsSq)
{
  uint16_t min         = ADC_RANGE - 1;
  uint16_t max         = 0;
  uint16_t avg         = this->sum / VALBUF_SIZE;
  uint32_t avgUnscaled = this->sum;
  uint32_t rmsSq       = 0;
  for (int i = 0; i < VALBUF_SIZE; i++)
  {
    if (this->values[i] > max)
      max = this->values[i];
    if (this->values[i] < min)
      min = this->values[i];
    rmsSq += (VALBUF_SIZE * this->values[i] - avgUnscaled) * (VALBUF_SIZE * this->values[i] - avgUnscaled);
  }
  *pMin   = min;
  *pMax   = max;
  *pAvg   = avg;
  *pRmsSq = rmsSq / VALBUF_SIZE;
}

static ValueBuffer_T windowBuffer;

// ---------------- end Value Buffer defs

#define DEADZONE                (10u)    // when settled ignore adcValue changes (wrt center) below this -- kills noise
#define EXTREMA_BACKOFF         (30u)    // back-off for saturated adc values at end stops for safe 0% or 100% output
#define NOMINAL_LEFT_ENDSTOP    (33u)    // default left end stop safe adc value (saturated adc)
#define NOMINAL_RIGHT_ENDSTOP   (4000u)  // default right end stop safe adc value (saturated adc)
#define ENDSTOP_RANGE           (512u)   // range from either nominal end (saturated) where an end stop may be expected
#define AUTOZERO_RANGE          (512u)   // 2048 +- this adc value is the range where we will do autozeroing (512 = +-25%), to catch even heavily off-centered benders
#define HYSTERESIS_COMPENSATION (10)

// settling parameters
#define RMS_THRES_HIGH              (3000u)
#define RMS_THRES_MEDIUM            (700u)
#define RMS_THRES_LOW               (400u)
#define DELTA_THRES_HIGH            (20u)
#define DELTA_THRES_MEDIUM          (10u)
#define DELTA_THRES_LOW             (5u)
#define SETTLE_TIME_THRESHOLD_SHORT (400u)   // time in milliseconds values must be stable before transition from unsettled to settled
#define SETTLE_TIME_THRESHOLD_LONG  (1000u)  // time in milliseconds values must be stable before transition from unsettled to settled

#define SETTLE_TIME_THRESHOLD_SHORT_TS (SETTLE_TIME_THRESHOLD_SHORT * 10ul / 125ul)
#define SETTLE_TIME_THRESHOLD_LONG_TS  (SETTLE_TIME_THRESHOLD_LONG * 10ul / 125ul)

static const struct
{
  uint16_t const rmsThresH;
  uint16_t const rmsThresL;
  uint16_t const deltaThresH;
  uint16_t const deltaThresL;
  uint16_t const timeout;
} stettlingConstants[2] = {
  {
      .rmsThresH   = RMS_THRES_HIGH,
      .rmsThresL   = RMS_THRES_MEDIUM,
      .deltaThresH = DELTA_THRES_HIGH,
      .deltaThresL = DELTA_THRES_MEDIUM,
      .timeout     = SETTLE_TIME_THRESHOLD_SHORT_TS,
  },
  {
      .rmsThresH   = RMS_THRES_MEDIUM,
      .rmsThresL   = RMS_THRES_LOW,
      .deltaThresH = DELTA_THRES_MEDIUM,
      .deltaThresL = DELTA_THRES_LOW,
      .timeout     = SETTLE_TIME_THRESHOLD_LONG_TS,
  },
};

// --- Bender: ADC value to Output conversion (this is the general characteristic curve)
static int16_t             BNDR_adcToTcdX[4]   = { NOMINAL_LEFT_ENDSTOP, ADC_CENTER - DEADZONE, ADC_CENTER + DEADZONE, NOMINAL_RIGHT_ENDSTOP };
static int16_t             BNDR_adcToTcdY[4]   = { 0, 8000, 8000, 16000 };  // TCD output
static LIB_interpol_data_T BNDR_adcToTcd       = { 4, BNDR_adcToTcdX, BNDR_adcToTcdY };
static int16_t *const      pLeftEndStop        = &BNDR_adcToTcdX[0];
static int16_t *const      pLowerDeadzone      = &BNDR_adcToTcdX[1];
static int16_t *const      pUpperDeadzone      = &BNDR_adcToTcdX[2];
static int16_t *const      pRightEndStop       = &BNDR_adcToTcdX[3];
static uint16_t            settlingSensitivity = 0;  // 0=low, >=1, high

static uint16_t settledCandidateCntrCoarse;
static uint16_t settledCandidateCntrFine;
static int8_t   settledCoarse;
static int      settledFine;

static uint16_t currentCenter;
static int      goingSettled;

static void ProcessBender(int16_t adcValue)
{
  addInValueBuffer(&windowBuffer, adcValue);
  if (!isValueBufferFilled(&windowBuffer))
  {  // provide initial center target
    currentCenter = windowBuffer.sum / VALBUF_SIZE;
    return;
  }

  uint16_t min;
  uint16_t max;
  uint16_t avg;
  uint32_t rmsSq;

  getValueBufferStats(&windowBuffer, &min, &max, &avg, &rmsSq);

  // settling checks
  // coarse
  if (settledCoarse)
  {  // when settled wait for a single larger change to go unsettled
    settledCoarse = (rmsSq < stettlingConstants[0].rmsThresH) && ((max - min) < stettlingConstants[0].deltaThresH);
    if (!settledCoarse)
      settledCandidateCntrCoarse = 0;
  }
  else
  {  // when not settled wait for stable and really small changes to go settled again
    if ((rmsSq < stettlingConstants[0].rmsThresL) && ((max - min) < stettlingConstants[0].deltaThresL))
    {
      if (settledCandidateCntrCoarse < stettlingConstants[0].timeout)  // continuously stable ?
        settledCandidateCntrCoarse++;
      else
        settledCoarse = 1;
    }
  }
  // fine
  if (settledFine)
  {  // when settled wait for a single larger change to go unsettled
    settledFine = (rmsSq < stettlingConstants[1].rmsThresH) && ((max - min) < stettlingConstants[1].deltaThresH);
    if (!settledFine)
      settledCandidateCntrFine = 0;
  }
  else
  {  // when not settled wait for stable and really small changes to go settled again
    if ((rmsSq < stettlingConstants[1].rmsThresL) && ((max - min) < stettlingConstants[1].deltaThresL))
    {
      if (settledCandidateCntrFine < stettlingConstants[1].timeout)  // continuously stable ?
        settledCandidateCntrFine++;
      else
        settledFine = 1;
    }
  }

  int settled    = settlingSensitivity ? settledFine : settledCoarse;
  int tryZeroing = 0;

  // process adc values, center range first, then from right (high values) to left (low values)
  if ((adcValue >= (ADC_CENTER - AUTOZERO_RANGE)) && (adcValue <= (ADC_CENTER + AUTOZERO_RANGE)))
  {
    {  // inside auto-zero catch range
      switch (goingSettled)
      {
        case 0:  // wait until settled
          if (!settled)
            break;
          // determine and save new center target
          currentCenter = avg;
          goingSettled  = 1;
        case 1:  // wait until unsettled
          if (settled)
          {
            tryZeroing = 1;
            break;
          }
          goingSettled = 0;
          break;
      }
    }
  }
  else
  {
    goingSettled = 0;
    if (adcValue > (NOMINAL_RIGHT_ENDSTOP - ENDSTOP_RANGE))
    {  // close to typical right end stop (using coarse settling always)
      if (settledCoarse && ((*pRightEndStop) > avg - EXTREMA_BACKOFF))
      {  // new end stop is closer to center
        (*pRightEndStop)    = avg - EXTREMA_BACKOFF;
        status.rightEndStop = 1;
      }
    }
    else if (adcValue > (ADC_CENTER + AUTOZERO_RANGE))
    {  // above auto-zero range
      status.offZero |= settled;
    }
    else if (adcValue > (NOMINAL_LEFT_ENDSTOP + ENDSTOP_RANGE))
    {  // below auto-zero range
      status.offZero |= settled;
    }
    else
    {  // close to typical left end stop (using coarse settling always)
      if (settledCoarse && ((*pLeftEndStop) < avg + EXTREMA_BACKOFF))
      {  // new end stop is closer to center
        (*pLeftEndStop)    = avg + EXTREMA_BACKOFF;
        status.leftEndStop = 1;
      }
    }
  }

  status.zeroed = tryZeroing;
  status.everZeroed |= tryZeroing;

  if (tryZeroing)
  {
    currentCenter = avg;

    // move dead zones to new zero and expand them to full span
    if (*pLowerDeadzone + DEADZONE < currentCenter)
      (*pLowerDeadzone)++;
    else if (*pLowerDeadzone + DEADZONE > currentCenter)
      (*pLowerDeadzone)--;
    if (*pUpperDeadzone < currentCenter + DEADZONE)
      (*pUpperDeadzone)++;
    else if (*pUpperDeadzone > currentCenter + DEADZONE)
      (*pUpperDeadzone)--;
    status.zeroValue = currentCenter >> 2;
  }
  else
  {  // shrink deadzones
    if (*pLowerDeadzone < currentCenter)
      (*pLowerDeadzone)++;
    else if (*pLowerDeadzone > currentCenter)
      (*pLowerDeadzone)--;
    if (*pUpperDeadzone < currentCenter)
      (*pUpperDeadzone)++;
    else if (*pUpperDeadzone > currentCenter)
      (*pUpperDeadzone)--;
  }
  while (*pUpperDeadzone < *pLowerDeadzone)
  {
    ;
  }

  if (!status.everZeroed)  // couldn't ever find a valid zero position ==> no output
    return;

  int16_t tcdOutput = LIB_InterpolateValue(&BNDR_adcToTcd, adcValue);

  // apply user curves. Tables are not bipolar ==> split pos. and neg. processing
  if (tcdOutput >= BIPOLAR_CENTER)
  {
    tcdOutput = tcdOutput - BIPOLAR_CENTER;
    tcdOutput = +LIB_InterpolateValue(&BNDR_benderCurves, tcdOutput) + BIPOLAR_CENTER;
  }
  else
  {
    tcdOutput = BIPOLAR_CENTER - tcdOutput;
    tcdOutput = -LIB_InterpolateValue(&BNDR_benderCurves, tcdOutput) + BIPOLAR_CENTER;
  }

  if (tcdOutput != BNDR_lastBender)
  {
    BNDR_lastBender = tcdOutput;
    ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_PITCHBEND, tcdOutput);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_PITCHBEND, tcdOutput);
  }
}
// 0=insensitive, 1=sensitive
void BNDR_Select_BenderSensitivity(uint16_t const sensitivity)
{
  if (sensitivity == 0)
    settlingSensitivity = 0;
  else
    settlingSensitivity = 1;
}

void BNDR_Reset()
{
  // legacy
  lastRawPitchbend = ADC_RANGE;
  lastPitchbend    = BIPOLAR_CENTER;
  pitchbendZero    = ADC_CENTER;
  pbSignalIsSmall  = 0;
  pbTestTime       = 0;
  pbTestMode       = 0;
  pbRampMode       = 0;
  pbRamp           = 0;
  pbRampInc        = 0;
  // end legacy

  *pLeftEndStop              = NOMINAL_LEFT_ENDSTOP;
  *pLowerDeadzone            = ADC_CENTER - DEADZONE;
  *pUpperDeadzone            = ADC_CENTER + DEADZONE;
  *pRightEndStop             = NOMINAL_RIGHT_ENDSTOP;
  status                     = BNDR_uint16ToStatus(0);
  status.legacyMode          = legacyMode;
  settledCoarse              = 0;
  settledCandidateCntrCoarse = 0;
  settledFine                = 0;
  settledCandidateCntrFine   = 0;
  BNDR_lastBender            = BIPOLAR_CENTER;
  goingSettled               = 0;

  clearValueBuffer(&windowBuffer, 2 * VALBUF_SIZE, ADC_CENTER);
  ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_PITCHBEND, BIPOLAR_CENTER);
  ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_PITCHBEND, BIPOLAR_CENTER);
}

// -------------
void BNDR_ProcessBender(void)
{
  int16_t adcValue = IPC_ReadAdcBufferAveraged(IPC_ADC_PITCHBENDER);

  if (legacyMode)
    ProcessLegacyBender(adcValue);
  else
    ProcessBender(adcValue);
}

void BNDR_Init(void)
{
  Generate_BenderTable(0);
  Generate_BenderTable(1);
  Generate_BenderTable(2);
  BNDR_Select_BenderTable(1);
  BNDR_Select_BenderSensitivity(0);
  BNDR_Reset();
}
