#include <stdint.h>
#include "tcd/nl_tcd_aftertouch.h"
#include "tcd/nl_tcd_interpol.h"
#include "tcd/nl_tcd_adc_work.h"
#include "tcd/nl_tcd_poly.h"
#include "ipc/emphase_ipc.h"
#include "playcontroller/playcontroller-defs.h"
#include "sys/nl_eeprom.h"
#include "sys/nl_stdlib.h"

uint32_t AT_lastAftertouch;

static int      AT_calibrationRun = 1;  // 0:off; 1:calibrate; 2:store
static uint16_t AT_eepromHandle   = 0;  // EEPROM access handle
static int      AT_updateEeprom   = 0;  // flag / step chain variable

// volatile ADC max values [61] is current single key #
// [61] is current single key #
// [62] is current force
static uint16_t AT_adcPerKey[63];

// --- ADC value to Force conversion --- this is the general characteristic curve
// ADC values (input, 0...4096)
static int16_t AT_adcToForceTableX[16] = { 0, 1196, 1992, 2530, 2816, 3002, 3202, 3301, 3368, 3412, 3474, 3512, 3541, 3557, 3570, 3600 };
// Force values (output, Milli-Newton units)
static int16_t AT_adcToForceTableY[16] = { 5300, 6000, 6500, 7000, 7500, 8000, 9000, 10000, 11000, 12000, 14000, 16000, 18000, 20000, 22000, 26000 };

static LIB_interpol_data_T AT_adcToForce = { 16, AT_adcToForceTableX, AT_adcToForceTableY };

// --- Key Calibration ---
typedef struct
{
  uint16_t keybedId;       // keybed serial #
  uint16_t adcTarget;      // adc target value for the test force
  uint16_t adcDefault;     // default when AT is active without any key pressed (typically the average of all the following values)
  uint16_t adcValues[61];  // adc values obtained for test force for all 61 keys
} AT_calibration_T;

// --- Default Calibration (based on many keybeds) ---
// clang-format off
static AT_calibration_T NL_EEPROM_ALIGN AT_adcCalibration =
{
  .keybedId   = 00001,
  .adcTarget  = 3290,  // input into adc-to-force charactestic to achieve 10N
  .adcDefault = 3465,
  {
    3019, 2980, 3200, 2999, 3256, 3231, 3087, 3275, 3119, 3263, 3133, 3295,
	3296, 3172, 3417, 3367, 3530, 3526, 3451, 3633, 3581, 3676, 3620, 3687,
	3675, 3607, 3673, 3630, 3706, 3683, 3562, 3622, 3522, 3635, 3561, 3608,
	3582, 3471, 3565, 3496, 3589, 3546, 3467, 3543, 3454, 3555, 3455, 3566,
	3567, 3517, 3610, 3558, 3618, 3609, 3530, 3542, 3576, 3621, 3533, 3593, 3570
  }
};
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

void AT_Init(void)
{
  SIZECHECK(AT_calibration_T, 32 * sizeof(uint32_t))

  AT_lastAftertouch = 0;
  AT_Select_AftertouchTable(1);

  AT_eepromHandle = NL_EEPROM_RegisterBlock(sizeof AT_adcCalibration, EEPROM_BLOCK_ALIGN_TO_PAGE);
  AT_calibration_T tmp;
  if (NL_EEPROM_ReadBlock(AT_eepromHandle, &tmp, EEPROM_READ_BOTH))
    memcpy(&AT_adcCalibration, &tmp, sizeof(AT_adcCalibration));
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

  AT_calibration_T tmp;

  memcpy(&AT_adcCalibration, &tmp, sizeof(AT_adcCalibration));
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

/*****************************************************************************
* @brief	ADC_WORK_Select_AftertouchTable -
* @param	0: soft, 1: normal, 2: hard
******************************************************************************/
void AT_Select_AftertouchTable(uint16_t const curve)
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
      break;
  }
}

//
// -----------------
void AT_ProcessAftertouch(void)
{
  int16_t adcValue;
  int16_t tcdOutput;
  int16_t adcToForceInput = 0;
  int16_t force           = 0;

  static int      state = 0;
  static int16_t  timer;
  static uint16_t savedAdcValue;

  static LIB_lowpass_data_T lpFilter = { .filtered = 0, .beta = 6, .fpShift = 7 };
  int16_t                   adcFiltered;

  adcValue = IPC_ReadAdcBufferAveraged(IPC_ADC_AFTERTOUCH);

  if (adcValue == 0)
  {
    tcdOutput        = 0;
    state            = 0;
    AT_adcPerKey[61] = -1;
  }
  else
  {
    int singleKey = POLY_GetSingleKey();  // -1 means : not a single key (none or more than one singleKey)
    if (singleKey != -1)
      AT_adcPerKey[61] = (AT_adcPerKey[61] & 0x8000) | singleKey;
    else
      AT_adcPerKey[61] = -1;

    // save current ADC adcValue in volatile table once it has settled
    if (AT_calibrationRun == 1)
    {
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

    // apply calibration
    int16_t calibrationPoint;
    if (singleKey != -1)
      calibrationPoint = AT_adcCalibration.adcValues[singleKey];
    else  // use average adcValue
      calibrationPoint = AT_adcCalibration.adcDefault;

    adcToForceInput = adcValue + (AT_adcCalibration.adcTarget - calibrationPoint);

    // convert to force and then to TCD
    force     = LIB_InterpolateValue(&AT_adcToForce, adcToForceInput);
    tcdOutput = LIB_InterpolateValue(AT_forceToTcd, force);
  }
  AT_adcPerKey[62] = force;

  if (tcdOutput != AT_lastAftertouch)
  {
    AT_lastAftertouch = tcdOutput;
    ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
  }

  checkCalibrationEepromUpdate();
}
