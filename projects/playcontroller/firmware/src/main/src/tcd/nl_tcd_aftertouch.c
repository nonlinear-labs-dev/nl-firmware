#include <stdint.h>
#include "tcd/nl_tcd_aftertouch.h"
#include "playcontroller/lpc_lib.h"
#include "tcd/nl_tcd_adc_work.h"
#include "tcd/nl_tcd_poly.h"
#include "ipc/emphase_ipc.h"
#include "playcontroller/playcontroller-defs.h"
#include "playcontroller/playcontroller-converters.h"
#include "playcontroller/playcontroller-data.h"
#include "sys/nl_eeprom.h"
#include "sys/nl_stdlib.h"
#include "playcontroller/lpc_lib.h"

#define AT_RAMP_TIME  (3000ul)                 // ramp time in ms (maximum)
#define AT_RAMP_COUNT (2 * AT_RAMP_TIME / 25)  // ramp count, based on a 12.5ms time-slice, make sure it is not zero

// --------------
static uint32_t AT_lastAftertouch;
uint32_t        AT_GetLastAftertouch()
{
  return AT_lastAftertouch;
}
#define AT_LEGACY_DEFAULT (1)
static int legacyMode = AT_LEGACY_DEFAULT;
void       AT_SetLegacyMode(int const on)
{
  legacyMode = (on != 0);
}

// --------------
static int      collectATtestData = 0;  // 0:off; 1:on
static uint16_t AT_eepromHandle   = 0;  // EEPROM access handle
static int      AT_updateEeprom   = 0;  // flag / step chain variable

// volatile ADC max values [61] is current single key #
// [61] is current single key #
// [62] is current force
// [63] is current calibration point
static uint16_t AT_adcPerKey[64];

// --- Aftertouch: ADC value to Force conversion (this is the general characteristic curve)
// X and Y data is global (defined in playcontroller-defs.h)
static LIB_interpol_data_T AT_adcToForce = { 16, AT_adcToForceTableX, AT_adcToForceTableY };

// --- Default Calibration (based on many keybeds) ---
// clang-format off
#warning ToDo: insert useful default aftertouch calibration data
#define ATCAL (3300)
static AT_calibration_T NL_EEPROM_ALIGN AT_adcCalibration =
{
  .keybedId   = 0000,   // =0 --> turn on legacy mode, do not apply any calibration
  .adcTarget  = ATCAL,  // input into adc-to-force characteristic to achieve 10N
  .adcDefault = ATCAL,
  {
	ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL,
	ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL,
	ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL,
	ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL,
	ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL, ATCAL,
  }
};
/* TEST CODE
static AT_calibration_T NL_EEPROM_ALIGN AT_adcCalibration =
{
  .keybedId   = 00001,
  .adcTarget  = 3290,  // input into adc-to-force characteristic to achieve 10N
  .adcDefault = 3465,
  {
    3019, 2980, 3200, 2999, 3256, 3231, 3087, 3275, 3119, 3263, 3133, 3295,
	3296, 3172, 3417, 3367, 3530, 3526, 3451, 3633, 3581, 3676, 3620, 3687,
	3675, 3607, 3673, 3630, 3706, 3683, 3562, 3622, 3522, 3635, 3561, 3608,
	3582, 3471, 3565, 3496, 3589, 3546, 3467, 3543, 3454, 3555, 3455, 3566,
	3567, 3517, 3610, 3558, 3618, 3609, 3530, 3542, 3576, 3621, 3533, 3593, 3570
  }
};
*/
// clang-format on

// --- Force to TDC range conversion tables ---

// Soft
static int16_t             AT_forceToTcdX_Soft[3] = { 5300, 6300, 7500 };  // forces
static int16_t             AT_forceToTcdY_Soft[3] = { 0, 3000, 16000 };    // TCD output
static LIB_interpol_data_T AT_forceToTcd_Soft     = { 3, AT_forceToTcdX_Soft, AT_forceToTcdY_Soft };

// Normal
static int16_t             AT_forceToTcdX_Normal[3] = { 6000, 7100, 9000 };  // forces
static int16_t             AT_forceToTcdY_Normal[3] = { 0, 4000, 16000 };    // TCD output
static LIB_interpol_data_T AT_forceToTcd_Normal     = { 3, AT_forceToTcdX_Normal, AT_forceToTcdY_Normal };

// Hard
static int16_t             AT_forceToTcdX_Hard[2] = { 6700, 11000 };  // forces
static int16_t             AT_forceToTcdY_Hard[2] = { 0, 16000 };     // TCD output
static LIB_interpol_data_T AT_forceToTcd_Hard     = { 2, AT_forceToTcdX_Hard, AT_forceToTcdY_Hard };

static LIB_interpol_data_T *AT_forceToTcd = &AT_forceToTcd_Normal;

// -------------

uint16_t AT_GetATAdcDataSize(void)
{
  return sizeof(AT_adcPerKey) / sizeof(uint16_t);
}

uint16_t *AT_GetATAdcData(void)
{
  return AT_adcPerKey;
}
void AT_SetCollectTestData(uint16_t const flag)
{
  collectATtestData = (flag != 0);
}

/*****************************************************************************
* @brief	AT_SetAftertouchCalibration -
* @param	length: # of words in data array
* @param	data: array containing calibration data sets for aftertouch
******************************************************************************/
void AT_SetAftertouchCalibration(uint16_t const length, uint16_t *data)
{
  if (length != (sizeof(AT_calibration_T) / sizeof(uint16_t)))
    return;

  memcpy(&AT_adcCalibration, data, sizeof(AT_adcCalibration));
  if (AT_updateEeprom == 0)  // already updating ?
    AT_updateEeprom = 1;     // no, start step chain
}

//--------

static void checkCalibrationEepromUpdate(void)
{
  static AT_calibration_T tmp;
  if (AT_updateEeprom && !NL_EEPROM_Busy())
  {
    switch (AT_updateEeprom)  // read, compare and burn sequencer, to split cpu load
    {
      // read main EEPROM
      case 1:
        if (NL_EEPROM_ReadBlock(AT_eepromHandle, &tmp, EEPROM_READ_MAIN))  // read OK ?
          AT_updateEeprom = 3;                                             // then check if equal data
        else
          AT_updateEeprom = 2;  // read failed, check shadow
        break;
      // read shadow EEPROM
      case 2:
        if (NL_EEPROM_ReadBlock(AT_eepromHandle, &tmp, EEPROM_READ_SHADOW))  // read OK ?
          AT_updateEeprom = 3;                                               // then check if equal data
        else
          AT_updateEeprom = 4;  // read failed, force write
        break;
      // compare data
      case 3:
        if (memcmp32(&AT_adcCalibration, &tmp, sizeof(tmp) / 4) == 0)  // data is the same ?
          AT_updateEeprom = 0;                                         // yes, done
        else
          AT_updateEeprom = 4;  // no, force write
        break;
      // write data
      case 4:
        NL_EEPROM_StartWriteBlock(AT_eepromHandle, &AT_adcCalibration);
        AT_updateEeprom = 0;  // done
        break;
    }
  }
}

//
// -----------------
static void collectCalibrationData(int16_t const adcValue)
{

  static int      state = 0;
  static int16_t  timer;
  static uint16_t savedAdcValue;

  static LIB_lowpass_data_T lpFilter = { .filtered = 0, .beta = 6, .fpShift = 7 };
  int16_t                   adcFiltered;

  if (collectATtestData == 0)
    return;

  int singleKey = POLY_getSingleKey();  // one and only one single key
  if (singleKey != -1)
    AT_adcPerKey[61] = (AT_adcPerKey[61] & 0x8000) | singleKey;
  else
    AT_adcPerKey[61] = -1;

  if (adcValue == 0)
  {
    state = 0;
    return;
  }

  // save current ADC adcValue in volatile table once it has settled
  if (singleKey != -1)
  {
    switch (state)
    {
      case 0:  // key newly pressed
        state         = 1;
        savedAdcValue = lpFilter.filtered = adcValue;
        timer                             = 0;
        break;
      case 1:  // wait until values settles around [-1; +2]
        adcFiltered             = LIB_LowPass(&lpFilter, adcValue);
        AT_adcPerKey[singleKey] = adcFiltered;  // for monitoring
        AT_adcPerKey[61] &= 0x7FFF;
        if ((adcFiltered >= savedAdcValue - 1) && (adcFiltered <= savedAdcValue + 2))
        {  // still within range
          timer++;
          if (timer > 80)  // stable for 1 seconds
          {
            AT_adcPerKey[singleKey] = savedAdcValue;  // that's our max
            AT_adcPerKey[61] |= 0x8000;
            state = 99;
            break;
          }
        }
        else
        {  // outside limits
          timer         = 0;
          savedAdcValue = adcFiltered;  // new range
        }
        break;
      case 99:  // wait until key is released
        break;
    }
  }
}

// -------------
static void ProcessAftertouch(int16_t adcValue)
{
  int16_t tcdOutput;
  int16_t adcToForceInput;
  int16_t force;

  int            focussedKey;
  static int     lastFocussedKey = -1;
  static int16_t calibrationTarget;
  static int16_t calibrationPoint;
  static int     dynamicOffset = 0;

  do
  {
    if (POLY_SilentMaskedKeys() && POLY_OnlyMaskedKeysPressed())
    {  // mute AT output for masked keys
      adcValue    = 0;
      focussedKey = lastFocussedKey;
      break;
    }
    focussedKey = POLY_getNewestKey();  // -1 means : no (unmasked) key pressed
    if (focussedKey != lastFocussedKey)
    {
      if (focussedKey == -1)
      {
        calibrationTarget = calibrationPoint = AT_adcCalibration.adcDefault;
        break;
      }

      if (lastFocussedKey == -1)
      {  // newly pressed single key, so start right from there
        calibrationTarget = calibrationPoint = AT_adcCalibration.adcValues[focussedKey];
        break;
      }

      // focussed key has changed
      calibrationTarget = AT_adcCalibration.adcValues[focussedKey];
      if (adcValue == 0)  // if AT is not active then no need to ramp to new target
        calibrationPoint = calibrationTarget;
      else
      {
        dynamicOffset = abs(calibrationTarget - calibrationPoint) / AT_RAMP_COUNT;  // ramp to new target
        if (dynamicOffset == 0)
          dynamicOffset = 1;
      }
    }
  } while (0);
  lastFocussedKey = focussedKey;

  // ramp to new calibration point if required
  if (dynamicOffset)
  {
    if (calibrationPoint < calibrationTarget)
    {
      calibrationPoint += dynamicOffset;
      if (calibrationPoint >= calibrationTarget)
      {
        calibrationPoint = calibrationTarget;
        dynamicOffset    = 0;
      }
    }
    else if (calibrationPoint > calibrationTarget)
    {
      calibrationPoint -= dynamicOffset;
      if (calibrationPoint <= calibrationTarget)
      {
        calibrationPoint = calibrationTarget;
        dynamicOffset    = 0;
      }
    }
  }

  if (adcValue == 0)
  {
    tcdOutput = 0;
    force     = 0;
  }
  else
  {
    // apply calibration

    adcToForceInput = adcValue + (AT_adcCalibration.adcTarget - calibrationPoint);

    // convert to force and then to TCD
    force     = LIB_InterpolateValue(&AT_adcToForce, adcToForceInput);
    tcdOutput = LIB_InterpolateValue(AT_forceToTcd, force);
  }

  AT_adcPerKey[62] = force;
  AT_adcPerKey[63] = calibrationPoint;

  if (tcdOutput != AT_lastAftertouch)
  {
    AT_lastAftertouch = tcdOutput;
    ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
  }

  checkCalibrationEepromUpdate();
}

// -------------

/*****************************************************************************
* @brief	ADC_WORK_Select_AftertouchTable -
* @param	0: soft, 1: normal, 2: hard
******************************************************************************/
static uint32_t  allAftertouchTables[3][33] = {};  // contains the chosen aftertouch curve
static uint32_t *aftertouchTable;
void             AT_Select_AftertouchTable(uint16_t const curve)
{
  switch (curve)
  {
    case 0:  // soft
      AT_forceToTcd = &AT_forceToTcd_Soft;
      break;
    case 1:  // normal
      AT_forceToTcd = &AT_forceToTcd_Normal;
      break;
    case 2:  // hard
      AT_forceToTcd = &AT_forceToTcd_Hard;
      break;
    default:
      return;
  }

  // Legacy
  aftertouchTable = allAftertouchTables[curve];
}

// Legacy
static void Generate_AftertouchTable(uint16_t const curve)
{
  float range = 16000.0;  // full TCD range

  float s;

  switch (curve)  // s defines the curve shape
  {
    case 0:     // soft
      s = 0.0;  // y = x
      break;
    case 1:     // normal
      s = 0.7;  // y = 0.3 * x + 0.7 * x^6
      break;
    case 2:      // hard
      s = 0.95;  // y = 0.05 * x + 0.95 * x^6
      break;
    default:
      return;
  }

  uint32_t i_max = 32;
  uint32_t i;
  float    x;

  for (i = 0; i <= i_max; i++)
  {
    x = (float) i / (float) i_max;

    allAftertouchTables[curve][i] = (uint32_t)(range * x * ((1.0 - s) + s * x * x * x * x * x));
  }
}

void AT_Init(void)
{
  SIZECHECK(AT_calibration_T, 32 * sizeof(uint32_t));

  AT_lastAftertouch = 0;
  collectATtestData = 0;  // 0:off; 1:on
  AT_eepromHandle   = 0;  // EEPROM access handle
  AT_updateEeprom   = 0;  // flag / step chain variable
  legacyMode        = AT_LEGACY_DEFAULT;

  // Legacy
  Generate_AftertouchTable(0);
  Generate_AftertouchTable(1);
  Generate_AftertouchTable(2);

  AT_Select_AftertouchTable(1);

  AT_eepromHandle = NL_EEPROM_RegisterBlock(sizeof AT_adcCalibration, EEPROM_BLOCK_ALIGN_TO_PAGE);
  AT_calibration_T tmp;
  if (NL_EEPROM_ReadBlock(AT_eepromHandle, &tmp, EEPROM_READ_BOTH))
    memcpy(&AT_adcCalibration, &tmp, sizeof(AT_adcCalibration));
}

// --------------------------------------------------------
#define AT_DEADRANGE 30    // 0.73 % of 0 ... 4095
#define AT_FACTOR    5080  // 5080 / 4096 for saturation = 100 % at 81 % of the input range
static void ProcessLegacyAftertouch(int16_t const adcValue)
{
  int32_t value = adcValue;
  int32_t valueToSend;

  static int32_t lastAftertouch;

  value = IPC_ReadAdcBufferAveraged(IPC_ADC_AFTERTOUCH);

  if (value != lastAftertouch)
  {
    if (value > AT_DEADRANGE)  // outside of the dead range
    {
      valueToSend = value - AT_DEADRANGE;

      valueToSend = value * AT_FACTOR;  // saturation factor

      if (valueToSend > 4095 * 4096)
      {
        valueToSend = 16000;
      }
      else
      {
        valueToSend = valueToSend >> 12;  // 0 ... 4095

        uint32_t fract = valueToSend & 0x7F;                                                                  // lower 7 bits used for interpolation
        uint32_t index = valueToSend >> 7;                                                                    // upper 5 bits (0...31) used as index in the table
        valueToSend    = (aftertouchTable[index] * (128 - fract) + aftertouchTable[index + 1] * fract) >> 7;  // (0...16000) * 128 / 128
      }

      ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, valueToSend);
      ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, valueToSend);
      AT_lastAftertouch = valueToSend;
    }
    else  // inside of the dead range
    {
      if (lastAftertouch > AT_DEADRANGE)  // was outside of the dead range before   /// define
      {
        ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, 0);
        ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, 0);
        AT_lastAftertouch = 0;
      }
    }

    lastAftertouch = value;
  }
}

// -------------
void AT_ProcessAftertouch(void)
{
  int16_t adcValue = IPC_ReadAdcBufferAveraged(IPC_ADC_AFTERTOUCH);

  collectCalibrationData(adcValue);

  if (legacyMode || (AT_adcCalibration.keybedId == 0))
    ProcessLegacyAftertouch(adcValue);
  else
    ProcessAftertouch(adcValue);
}

// -------------
uint16_t AT_GetStatus(void)
{
  AT_status_T status;
  status.legacyMode = legacyMode != 0;
  status.calibrated = AT_adcCalibration.keybedId != 0;
  status.maskedKeys = POLY_maskedKeyBF != 0;
  status.silentKeys = POLY_SilentMaskedKeys() != 0;
  return AT_statusToUint16(status);
}
