/*
 * nl_tcd_adc_work.c
 *
 *  Created on: 13.03.2015
 *      Author: ssc
 *  Last change on : 2020-05-10 KSTR
 */

#include <math.h>
#include "nl_tcd_adc_work.h"
#include "nl_tcd_msg.h"
#include "ipc/emphase_ipc.h"
#include "ipc/emphase_ipc.h"
#include "spibb/nl_bb_msg.h"
#include "nl_tcd_interpol.h"
#include "ehc/nl_ehc_ctrl.h"
#include "drv/nl_dbg.h"
#include "sys/nl_eeprom.h"
#include "sys/nl_stdlib.h"
#include "playcontroller/playcontroller-defs.h"
#include "tcd/nl_tcd_poly.h"
#include "io/pins.h"

#define BIPOLAR_CENTER      8000  // bipolar "0" in unsigned 0...16000 notation
#define BENDER_DEADRANGE    20    // +/-1 % of +/-2047
#define BENDER_SMALL_THRESH 200   // +/-10 % of +/-2047, test range
#define BENDER_TEST_PERIOD  80    // 1 s = 80 * 12.5 ms
#define BENDER_RAMP_INC     8     // 400 / 8 = 50 steps from the threshold to zero (50 * 12.5 ms = 625 ms)
#define BENDER_FACTOR       4540  // 4540 / 4096 = 2047 / 1847 for saturation = 100 % at 90 % of the input range

#define AT_DEADRANGE 30    // 0.73 % of 0 ... 4095
#define AT_FACTOR    5080  // 5080 / 4096 for saturation = 100 % at 81 % of the input range

static int      sendUiValues = 0;
static uint16_t uiSendValue[NUM_HW_SOURCES];
static uint16_t aeSendValue[NUM_HW_REAL_SOURCES];
static int      pollDataRequest = 0;

static int32_t lastPitchbend = BIPOLAR_CENTER;
static int32_t lastRawPitchbend;
static int32_t pitchbendZero;

static uint16_t  pbSignalIsSmall;
static uint16_t  pbTestTime;
static uint16_t  pbTestMode;
static uint16_t  pbRampMode;
static int32_t   pbRamp;
static int32_t   pbRampInc;
static uint32_t  allBenderTables[3][33] = {};  // contains the bender curves
static uint32_t *benderTable;

static uint32_t lastAftertouch;
static int      AT_calibrationRun = 1;  // 0:off; 1:calibrate; 2:store
//static uint16_t AT_eepromHandle   = 0;  // EEPROM access handle
//static int      AT_updateEeprom   = 0;  // flag / step chain variable

//========= ribbons ========
static uint16_t rib_eepromHandle = 0;  // EEPROM access handle
static int      rib_updateEeprom = 0;  // flag / step chain variable

typedef struct  // __attribute__((packed))
{
  int16_t threshold;
  int16_t __attribute__((packed)) tableX[33];
  int16_t __attribute__((packed)) tableY[33];
} RibbonCalibrationData_T;

// clang-format off
static RibbonCalibrationData_T NL_EEPROM_ALIGN ribbonCalibrationData[2] = {
// initialized with default data derived from a variety of ribbons
  { .threshold = 122,
    .tableX    = { 172, 282, 411, 533, 656, 772, 887, 1003, 1119, 1236, 1353, 1469, 1584, 1699, 1813, 1925, 2037,
		   2149, 2261, 2373, 2485, 2600, 2716, 2836, 2957, 3082, 3209, 3342, 3476, 3620, 3765, 3916, 4040 },
    .tableY    = { 0, 712, 1198, 1684, 2170, 2655, 3141, 3627, 4113, 4599, 5085, 5571, 6057, 6542, 7028, 7514, 8000,
                   8500, 8993, 9494, 9994, 10495, 10995, 11496, 11996, 12497, 12997, 13498, 13998, 14499, 14999, 15500, 16000 }
  },
  { .threshold = 122,
    .tableX    = { 172, 282, 411, 533, 656, 772, 887, 1003, 1119, 1236, 1353, 1469, 1584, 1699, 1813, 1925, 2037,
		   2149, 2261, 2373, 2485, 2600, 2716, 2836, 2957, 3082, 3209, 3342, 3476, 3620, 3765, 3916, 4040 },
    .tableY    = { 0, 712, 1198, 1684, 2170, 2655, 3141, 3627, 4113, 4599, 5085, 5571, 6057, 6542, 7028, 7514, 8000,
                   8500, 8993, 9494, 9994, 10495, 10995, 11496, 11996, 12497, 12997, 13498, 13998, 14499, 14999, 15500, 16000 }
  }
};

static const LIB_interpol_data_T ribbonCalibration[2] =
{
  {
	.points   = 33,
	.x_values = &ribbonCalibrationData[0].tableX[0],
	.y_values = &ribbonCalibrationData[0].tableY[0]
  },
  {
	.points   = 33,
	.x_values = &ribbonCalibrationData[1].tableX[0],
	.y_values = &ribbonCalibrationData[1].tableY[0]
  }
};
// clang-format on

// type for working variables
typedef struct
{
  int32_t              last;           ///< last raw value (0...4095)
  uint16_t             touch;          ///< flag for "touch begins"
  uint16_t             behavior;       ///< bit field: Bit 0:return/#non-return, Bit 1:relative/#absolute
  int32_t              relFactor;      ///< scale factor for relative movement
  uint16_t             isEditControl;  ///< flag for usage as "Edit Control Slider"
  uint16_t             editBehavior;   ///< flag absolute/#relative
  uint16_t             incBase;        ///< base value for relative increment
  int32_t              output;         ///< processed output value (0...16000)
  LIB_interpol_data_T *calibration;    ///< pointer to calibration data to be used
  uint16_t             ipcId;          ///< ID to fetch raw data from M0 kernel
  uint16_t             hwSourceId;     ///< ID for BB and TCD message
} Ribbon_Data_T;

// working variables
static Ribbon_Data_T ribbon[2];  // two ribbons
#define RIB1 0
#define RIB2 1

// global control
static uint16_t suspend                  = 1;  // will be set to zero by ADC_WORK_Init()
static uint16_t send_raw_sensor_messages = 0;  // sends raw sensor values every 12.5ms when set (!= 0)

static int16_t SetThreshold(int32_t val)
{  // set threshold to 80%
  val *= 8;
  val /= 10;
  return val;
}

// helper functions
int memcmp32(void *data1, void *data2, uint16_t count)
{
  while (count--)
  {
    if (*(int32_t *) (data1++) == *(int32_t *) (data2++))
      continue;
    return (*(int32_t *) (data1++) < *(int32_t *) (data2++) ? -1 : 1);
  }
  return 0;
}

/*****************************************************************************
* @brief	ADC_WORK_Select_BenderTable
* @param	0: soft, 1: normal, 2: hard
******************************************************************************/
void ADC_WORK_Select_BenderTable(uint16_t const curve)
{
  if (curve > 2)
    return;
  benderTable = allBenderTables[curve];
}

void Generate_BenderTable(uint16_t const curve)
{
  float_t range = BIPOLAR_CENTER;  // separate processing of absolute values for positive and negative range

  float_t s;

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

  uint32_t i_max = 32;
  uint32_t i;
  float    x;

  for (i = 0; i <= i_max; i++)
  {
    x = (float) i / (float) i_max;

    allBenderTables[curve][i] = (uint32_t)(range * x * ((1.0 - s) + s * x * x));
  }
}

void ClearHWValuesForUI(void)
{
  for (int i = 0; i < NUM_HW_SOURCES; i++)
    uiSendValue[i] = 0;
}

void ClearHWValuesForAE(void)
{
  for (int i = 0; i < NUM_HW_REAL_SOURCES; i++)
    aeSendValue[i] = 0;
}

/*****************************************************************************
* @brief	ADC_WORK_Init -
******************************************************************************/
void ADC_WORK_Init1(void)
{
  lastRawPitchbend = 0;
  lastPitchbend    = 0;
  pitchbendZero    = 2048;

  pbSignalIsSmall = 0;
  pbTestTime      = 0;
  pbTestMode      = 0;
  pbRampMode      = 0;
  pbRamp          = 0;
  pbRampInc       = 0;

  Generate_BenderTable(0);
  Generate_BenderTable(1);
  Generate_BenderTable(2);
  ADC_WORK_Select_BenderTable(1);

  lastAftertouch = 0;
  ADC_WORK_Select_AftertouchTable(1);

  // initialize ribbon data
  for (int i = 0; i <= 1; i++)
  {
    ribbon[i].last          = 0;
    ribbon[i].touch         = 0;
    ribbon[i].behavior      = 0;    // Abs, Non-Return
    ribbon[i].relFactor     = 256;  // == 1.0
    ribbon[i].isEditControl = 0;
    ribbon[i].editBehavior  = 0;
    ribbon[i].incBase       = 0;
    ribbon[i].output        = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    ribbon[i].calibration = &ribbonCalibration[i];
#pragma GCC diagnostic pop
    ribbonCalibrationData[i].threshold = SetThreshold(ribbonCalibrationData[i].threshold);
    ribbon[i].ipcId                    = (i == 0 ? IPC_ADC_RIBBON1 : IPC_ADC_RIBBON2);
    ribbon[i].hwSourceId               = (i == 0 ? HW_SOURCE_ID_RIBBON_1 : HW_SOURCE_ID_RIBBON_2);
  }

  NL_EHC_InitControllers();

  rib_eepromHandle = NL_EEPROM_RegisterBlock(sizeof ribbonCalibrationData, EEPROM_BLOCK_ALIGN_TO_PAGE);
  // AT_eepromHandle  = NL_EEPROM_RegisterBlock(sizeof ribbonCalibrationData, EEPROM_BLOCK_ALIGN_TO_PAGE);

  RibbonCalibrationData_T tmp[2];
  if (NL_EEPROM_ReadBlock(rib_eepromHandle, tmp, EEPROM_READ_BOTH))
    memcpy(ribbonCalibrationData, tmp, sizeof(ribbonCalibrationData));

  ClearHWValuesForUI();
  ClearHWValuesForAE();
}

void ADC_WORK_Init2(void)
{
  suspend = 0;
}

/*****************************************************************************
* @brief	SendEditMessageToBB - when using a ribbon as an edit control,
* 			the messages are sent by calls to this function
* @param	value: ribbon position (0 ... 16000)
* @param	inc: position increment (theoretically -16000 ... 16000)
******************************************************************************/
static void SendEditMessageToBB(uint16_t const which, uint16_t const value, int16_t inc)
{
  if (ribbon[which].editBehavior)  // 1: the ribbon sends the absolute value
  {
    BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_EDIT_CONTROL, ribbon[which].hwSourceId, value);
    BB_MSG_SendTheBuffer();  // sends the value as an Edit Control message; results being displayed on the upper Ribbon
  }
  else  // 0: the ribbon sends the increment
  {
    if (inc == 0)
      return;
    // limiting it to a 14-bit range like with other controls
    if (inc < -8000)
      inc = -8000;
    if (inc > +8000)
      inc = +8000;
    // sends the increment as an Edit Control message; results being displayed on the Ribbon
    BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_EDIT_CONTROL, ribbon[which].hwSourceId,
                            inc > 0 ? inc : 0x8000 | -inc);
    BB_MSG_SendTheBuffer();
  }
}
/// "return"-Behaviour braucht eigene Funktion, die mit einem Timer-Task aufgerufen wird
/// mit Abfrage, ob das Ribbon losgelassen wurde. Sprungartiges Zurücksetzen.

void ADC_WORK_WriteHWValueForUI(uint16_t const hwSourceId, uint16_t const value)
{
  uiSendValue[hwSourceId] = value + 1;  // makes sure it isn't zero (assumes that 0xFFFF is never used as input)
}

void ADC_WORK_WriteHWValueForAE(uint16_t const hwSourceId, uint16_t const value)
{
  aeSendValue[hwSourceId] = value + 1;  // makes sure it isn't zero (assumes that 0xFFFF is never used as input)
}

void ADC_WORK_SendUIMessages(void)  // is called as a regular COOS task
{
  uint32_t i;
  uint32_t send = 0;

  for (i = 0; i < NUM_HW_SOURCES; i++)
  {
    if (uiSendValue[i])
    {
      if (sendUiValues)
      {
        if (BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_PARAMETER, i, uiSendValue[i] - 1) > -1)
        {                      //
          uiSendValue[i] = 0;  // clear value, including update flag
          send           = 1;
        }  // else: sending failed, try again later
      }
      else
        uiSendValue[i] = 0;  // clear value, including update flag
    }
  }
  if (send)
    BB_MSG_SendTheBuffer();
}

void ADC_WORK_EnableSendUIMessages(uint16_t const flag)
{
  sendUiValues = (flag != 0);
}

static void SendAEMessages(void)
{
  static const enum HW_SOURCE_IDS PRIORITY_TABLE[NUM_HW_REAL_SOURCES] = {
    HW_SOURCE_ID_PEDAL_8,  // update EHCs first
    HW_SOURCE_ID_PEDAL_7,
    HW_SOURCE_ID_PEDAL_6,
    HW_SOURCE_ID_PEDAL_5,
    HW_SOURCE_ID_PEDAL_4,
    HW_SOURCE_ID_PEDAL_3,
    HW_SOURCE_ID_PEDAL_2,
    HW_SOURCE_ID_PEDAL_1,
    HW_SOURCE_ID_AFTERTOUCH,  // update AT and PB last, so that they override a macro ...
    HW_SOURCE_ID_PITCHBEND,   // ... control shared with with pedals quickly, to reduce glitching
    HW_SOURCE_ID_RIBBON_1,    // update ribbons last, so that they override a macro ...
    HW_SOURCE_ID_RIBBON_2,    // ... control shared with with pedals quickly, to reduce glitching
  };

  uint16_t j, i;
  for (j = 0; j < NUM_HW_REAL_SOURCES; j++)
  {
    i = PRIORITY_TABLE[j];  // sort update by priority
    if (aeSendValue[i])
    {
      MSG_HWSourceUpdate(i, aeSendValue[i] - 1);
      aeSendValue[i] = 0;  // clear value, including update flag
    }
  }
}

void ADC_WORK_PollRequestHWValues(void)
{
  pollDataRequest = 1;
}

static void PollHWValues(void)
{
  MSG_SendAEDevelopperCmd(AE_PROTOCOL_CMD_POLL_DATA_START);

  ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_PITCHBEND, lastPitchbend);
  ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_PITCHBEND, lastPitchbend);

  ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, lastAftertouch);
  ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, lastAftertouch);

  NL_EHC_PollControllers();

  SendAEMessages();
  MSG_SendAEDevelopperCmd(AE_PROTOCOL_CMD_POLL_DATA_STOP);
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbon1Behaviour
* @param	0: Abs + Non-Return, 1: Abs + Return, 2: Rel + Non-Return, 3: Rel + Return
******************************************************************************/
void ADC_WORK_SetRibbon1Behaviour(uint16_t const behaviour)
{
  ribbon[RIB1].behavior = behaviour;
  if (ribbon[RIB1].isEditControl == 0)  // initialization if working as a HW source
  {
    if ((ribbon[RIB1].behavior == 1) || (ribbon[RIB1].behavior == 3))  // "return" behavior
    {
      ribbon[RIB1].output = BIPOLAR_CENTER;
      ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_RIBBON_1, BIPOLAR_CENTER);
    }
  }
}

uint32_t ADC_WORK_GetRibbon1Behaviour(void)
{
  if (ribbon[RIB1].behavior & 0x01)
    return RETURN_TO_CENTER;
  else
    return NON_RETURN;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbonRelFactor -
* @param
******************************************************************************/
void ADC_WORK_SetRibbonRelFactor(uint16_t const factor)
{
  ribbon[RIB1].relFactor = factor;
  ribbon[RIB2].relFactor = factor;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbon2Behaviour -
* @param	0: Abs + Non-Return, 1: Abs + Return, 2: Rel + Non-Return, 3: Rel + Return
******************************************************************************/
void ADC_WORK_SetRibbon2Behaviour(uint16_t const behaviour)
{
  ribbon[RIB2].behavior = behaviour;
  if (ribbon[RIB2].isEditControl == 0)  // initialization if working as a HW source
  {
    if ((ribbon[RIB2].behavior == 1) || (ribbon[RIB2].behavior == 3))  // "return" behaviour
    {
      ribbon[RIB2].output = BIPOLAR_CENTER;
      ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_RIBBON_2, BIPOLAR_CENTER);
    }
  }
}

uint32_t ADC_WORK_GetRibbon2Behaviour(void)
{
  if (ribbon[RIB2].behavior & 0x01)
    return RETURN_TO_CENTER;
  else
    return NON_RETURN;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbon1EditMode -
* @param	0: Play, 1: (Parameter) Edit
******************************************************************************/
void ADC_WORK_SetRibbon1EditMode(uint16_t const mode)
{
  ribbon[RIB1].isEditControl = mode;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbon1EditBehaviour -
* @param	0: Relative + Non-Return, 1: Absolute + Non-Return
******************************************************************************/
void ADC_WORK_SetRibbon1EditBehaviour(uint16_t const behaviour)
{
  ribbon[RIB1].editBehavior = behaviour;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbon1/2OutputValue
* @param	0..16000 output value
******************************************************************************/
void ADC_WORK_SetRibbon1OutputValue(uint16_t const value)
{
  if (ribbon[RIB1].touch != 2 && !ribbon[RIB1].isEditControl)
    ribbon[RIB1].output = value;
}

void ADC_WORK_SetRibbon2OutputValue(uint16_t const value)
{
  if (ribbon[RIB2].touch != 2 && !ribbon[RIB2].isEditControl)
    ribbon[RIB2].output = value;
}

/*****************************************************************************
* @brief	ADC_WORK_SetRibbonCalibration -
* @param	length: # of words in data array
* @param	data: array containing calibration data sets for the ribbons
******************************************************************************/
void ADC_WORK_SetRibbonCalibration(uint16_t const length, uint16_t *data)
{
  if (length != (34 + 33 + 34 + 33))  // data must contain X (34 point) and Y (33 points) sets,  for each ribbon
    return;

  RibbonCalibrationData_T tmp[2];
  int                     i, j;

  for (i = 0; i < 2; i++)
  {  // al ribbons
    tmp[i].threshold = (int32_t) *data++;
    for (j = 0; j < 33; j++)
      tmp[i].tableX[j] = (int32_t) *data++;
    for (j = 0; j < 33; j++)
      tmp[i].tableY[j] = (int32_t) *data++;

    // Integrity checks
    if (tmp[i].threshold < 100
        || tmp[i].threshold > tmp[i].tableX[0])
      return;  // threshold typically is > 140, and must be <= first X entry
    tmp[i].threshold = SetThreshold(tmp[i].threshold);

    for (j = 0; j < 33; j++)
    {                             // all 33 data points
      if (tmp[i].tableX[j] < 100  // lowest raw value on touch is sure > 100 (140 typ.)
          || tmp[i].tableX[j] > 4095
          || tmp[i].tableX[j] < 100  // lowest raw value on touch is sure > 100 (140 typ.)
          || tmp[i].tableX[j] > 4095)
        return;  // x-values are not in valid range for raw values
      if (tmp[i].tableY[j] < 0
          || tmp[i].tableY[j] > 16000
          || tmp[i].tableY[j] < 0
          || tmp[i].tableY[j] > 16000)
        return;  // y-values are not in valid range for output values
      if (j > 0)
      {
        if (tmp[i].tableX[j] <= tmp[i].tableX[j - 1]
            || tmp[i].tableY[j] <= tmp[i].tableY[j - 1])
          return;  // x- or y-values are not monotonically rising
      }
    }
  }

  memcpy(ribbonCalibrationData, tmp, sizeof(ribbonCalibrationData));
  if (rib_updateEeprom == 0)  // already updating ?
    rib_updateEeprom = 1;     // no, start step chain
}

static void checkCalibrationEepromUpdate(void)
{
  static RibbonCalibrationData_T tmp[2];
  if (rib_updateEeprom && !NL_EEPROM_Busy())
  {
    switch (rib_updateEeprom)  // read, compare and burn sequencer, to split cpu load
    {
      // read main EEPROM
      case 1:
        if (NL_EEPROM_ReadBlock(rib_eepromHandle, tmp, EEPROM_READ_MAIN))  // read OK ?
          rib_updateEeprom = 3;                                            // then check if equal data
        else
          rib_updateEeprom = 2;  // read failed, check shadow
        break;
      // read shadow EEPROM
      case 2:
        if (NL_EEPROM_ReadBlock(rib_eepromHandle, tmp, EEPROM_READ_SHADOW))  // read OK ?
          rib_updateEeprom = 3;                                              // then check if equal data
        else
          rib_updateEeprom = 4;  // read failed, force write
        break;
      // compare data
      case 3:
        if (memcmp32(ribbonCalibrationData, tmp, sizeof(tmp) / 4) == 0)  // data is the same ?
          rib_updateEeprom = 0;                                          // yes, done
        else
          rib_updateEeprom = 4;  // no, force write
        break;
      // write data
      case 4:
        NL_EEPROM_StartWriteBlock(rib_eepromHandle, ribbonCalibrationData);
        rib_updateEeprom = 0;  // done
        break;
    }
  }
}

/*****************************************************************************
* @brief  Process Ribbons, array'd style, new Linearization/Calibration
******************************************************************************/
static void ProcessRibbons(void)
{
  for (int i = 0; i <= 1; i++)
  {
    int16_t input = IPC_ReadAdcBuffer(ribbon[i].ipcId);
    int     potentialRelease;
    int     position;
    int     inc;

    if (!ribbon[i].touch)
    {  // wait for initial touch
      if (input > ribbonCalibrationData[i].threshold)
      {  // but we won't use any data yet for output because it might not have settled fully
        ribbon[i].touch = 1;
        ribbon[i].last  = input;  // save initial input
      }
      continue;  // quit and process next ribbon
    }
    else
    {
      if (input <= ribbonCalibrationData[i].threshold  // hard below threshold, or
          || input + 150 < ribbon[i].last)             // very fast value drop, more than to be expected by fastest user action
      {
        ribbon[i].touch = 0;  // wait for new touch
        if (!ribbon[i].isEditControl && (ribbon[i].behavior & 0b01))
        {                             // play control with "return" behavior
          position = BIPOLAR_CENTER;  // center (zero) value
          goto Output;
        }
        continue;  // quit and process next ribbon
      }
      potentialRelease = (input + 4 <= ribbon[i].last);  // a drop more than just a few values
    }

    // normal processing starts here
    // touch has settled, readout continuous values

    if (potentialRelease)  // then use previous output to avoid sending drooping final value right before a release
      position = LIB_InterpolateValue(ribbon[i].calibration, ribbon[i].last);
    else
      position = LIB_InterpolateValue(ribbon[i].calibration, input);
    ribbon[i].last = input;

    if (ribbon[i].touch == 1)  // touch begins ?
    {
      ribbon[i].touch = 2;  // mark "stable data" phase
      inc             = 0;  // start increment is zero
    }
    else
      inc = position - ribbon[i].incBase;
    ribbon[i].incBase = position;

    if (inc > 18 || inc < -14)           // apply scale factor only if there is enough delta, this eliminates noise ...
      inc *= ribbon[i].relFactor / 256;  // ... and allows for fine adjustment even with large gains
                                         // the asymmetry accounts for the skew in value flow introduced by "potentialRelease"
    if (ribbon[i].isEditControl)
    {
      SendEditMessageToBB(i, position, inc);
      continue;  // do NOT update ribbon[i].output
    }
    // play control
    if (ribbon[i].behavior & 0b10)  // relative
    {
      ribbon[i].output += inc;  // bug atm : preset, or edit buffer, does not send the initial value of the ribbon as displayed
      position = ribbon[i].output;
      if (position < 0)
        position = 0;
      if (position > 16000)
        position = 16000;
    }

  Output:
    ADC_WORK_WriteHWValueForAE(ribbon[i].hwSourceId, position);
    ADC_WORK_WriteHWValueForUI(ribbon[i].hwSourceId, position);
    ribbon[i].output = position;

  }  // for all ribbons

  checkCalibrationEepromUpdate();
}

void ADC_WORK_SetRawSensorMessages(uint16_t const flag)
{
  send_raw_sensor_messages = (flag != 0);
}

void ADC_WORK_Suspend(void)
{
  suspend = 1;
}

void ADC_WORK_Resume(void)
{
  suspend = 0;
}

/*****************************************************************************
* @brief  Process Aftertouch
******************************************************************************/
// volatile ADC max values [61] is current single key #
// [61] is current single key #
// [62] is current force
static uint16_t AT_adcPerKey[63];

uint16_t ADC_WORK_GetATAdcDataSize(void)
{
  return sizeof(AT_adcPerKey) / sizeof(uint16_t);
}

uint16_t *ADC_WORK_GetATAdcData(void)
{
  return AT_adcPerKey;
}

// --- ADC value to Force conversion ---
// ADC values (input, 0...4096)
static int16_t AT_adcToForceTableX[16] = { 0, 1196, 1992, 2530, 2816, 3002, 3202, 3301, 3368, 3412, 3474, 3512, 3541, 3557, 3570, 3600 };
// Force values (output, 1/100 Newton units)
static int16_t AT_adcToForceTableY[16] = { 5300, 6000, 6500, 7000, 7500, 8000, 9000, 10000, 11000, 12000, 14000, 16000, 18000, 20000, 22000, 26000 };

static LIB_interpol_data_T AT_adcToForce = { 16, AT_adcToForceTableX, AT_adcToForceTableY };

// clang-format off
// --- Key Calibration ---
// ADC value calibration data
// [0..60] : per key raw adc reading value for test force
// [61]    : average of these values (can be computed, of course, unless there were some discarded values)
// [62]    : target adc input value for the averaged characteristic for the used test force (10N)

/* uneven keybed */
static int16_t AT_adcCalibration[63] =
{
  2995, 3018, 3141, 3028, 3191, 3204, 3124, 3224, 3128, 3216, 3150, 3275,
  3271, 3199, 3381, 3371, 3496, 3481, 3489, 3610, 3585, 3663, 3626, 3666,
  3659, 3601, 3659, 3646, 3683, 3662, 3595, 3578, 3539, 3621, 3570, 3567,
  3541, 3498, 3552, 3519, 3576, 3522, 3471, 3514, 3481, 3533, 3469, 3545,
  3547, 3528, 3575, 3554, 3596, 3591, 3535, 3498, 3581, 3587, 3534, 3564, 3531,
  3465, 3290
};
// clang-format off

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

/*****************************************************************************
* @brief	ADC_WORK_Select_AftertouchTable -
* @param	0: soft, 1: normal, 2: hard
******************************************************************************/
void ADC_WORK_Select_AftertouchTable(uint16_t const curve)
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
static void ProcessAftertouch(void)
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
    tcdOutput = 0;
    state     = 0;
  }
  else
  {
    int singleKey    = POLY_GetSingleKey();  // -1 means : not a single key (none or more than one singleKey)
    AT_adcPerKey[61] = (AT_adcPerKey[61] & 0x8000) | singleKey;

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
      calibrationPoint = AT_adcCalibration[singleKey];
    else  // use average adcValue
      calibrationPoint = AT_adcCalibration[61];

    adcToForceInput = adcValue + (AT_adcCalibration[62] - calibrationPoint);

    // convert to force and then to TCD
    force     = LIB_InterpolateValue(&AT_adcToForce, adcToForceInput);
    tcdOutput = LIB_InterpolateValue(AT_forceToTcd, force);
  }
  AT_adcPerKey[62] = force;

  if (tcdOutput != lastAftertouch)
  {
    lastAftertouch = tcdOutput;
    ADC_WORK_WriteHWValueForAE(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_AFTERTOUCH, tcdOutput);
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

static void ProcessBender(void)
{
  int32_t value;
  int32_t valueToSend;

  value = IPC_ReadAdcBufferAveraged(IPC_ADC_PITCHBENDER);  // 0 ... 4095

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
    {
      pbTestMode = 0;  // discard testing
    }

    pbSignalIsSmall = 0;
  }

  if (pbTestMode)
  {
    if (pbTestTime < BENDER_TEST_PERIOD)
    {
      pbTestTime++;
    }
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

    if (pbRamp <= 0)  // ramp has reached zero
    {
      pbRampMode = 0;  // and is stopped
    }
    else  // the steps before the ramp reaches zero
    {
      pitchbendZero += pbRampInc;  // shifting the zero line = negative feedback
    }
  }

  if (value != lastRawPitchbend)
  {
    if (value > BENDER_DEADRANGE)  // is in the positive work range
    {
      valueToSend = value - BENDER_DEADRANGE;  // absolute amount

      valueToSend = valueToSend * BENDER_FACTOR;  // saturation factor

      if (valueToSend > 2047 * 4096)
      {
        valueToSend = BIPOLAR_CENTER;  // clipping
      }
      else
      {
        valueToSend = valueToSend >> 12;  // 0 ... 2047 (11 Bit)

        uint32_t fract = valueToSend & 0x3F;                                                         // lower 6 bits used for interpolation
        uint32_t index = valueToSend >> 6;                                                           // upper 5 bits (0...31) used as index in the table
        valueToSend    = (benderTable[index] * (64 - fract) + benderTable[index + 1] * fract) >> 6;  // (0...8000) * 64 / 64
      }

      valueToSend = BIPOLAR_CENTER + valueToSend;  // 8001 ... 16000

      SendBenderIfChanged(valueToSend);
    }
    else if (value < -BENDER_DEADRANGE)  // is in the negative work range
    {
      valueToSend = -BENDER_DEADRANGE - value;  // absolute amount

      valueToSend = valueToSend * BENDER_FACTOR;  // saturation factor

      if (valueToSend > 2047 * 4096)
      {
        valueToSend = BIPOLAR_CENTER;  // clipping
      }
      else
      {
        valueToSend = valueToSend >> 12;  // 0 ... 2047 (11 Bit)

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

/*****************************************************************************
* @brief  Send Raw ADC values
******************************************************************************/
static void SendRawValues(void)
{
  if (send_raw_sensor_messages)
  {
    uint16_t data[13];
    // clang-format off
	    data[0] = (((IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL1_DETECT) >= 2048) ? 1 : 0) << 0)
	            | (((IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL2_DETECT) >= 2048) ? 1 : 0) << 1)
	            | (((IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL3_DETECT) >= 2048) ? 1 : 0) << 2)
	            | (((IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL4_DETECT) >= 2048) ? 1 : 0) << 3);
    // clang-format on
    data[1]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL1_TIP);
    data[2]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL1_RING);
    data[3]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL2_TIP);
    data[4]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL2_RING);
    data[5]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL3_TIP);
    data[6]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL3_RING);
    data[7]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL4_TIP);
    data[8]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PEDAL4_RING);
    data[9]  = IPC_ReadAdcBufferAveraged(IPC_ADC_PITCHBENDER);
    data[10] = IPC_ReadAdcBufferAveraged(IPC_ADC_AFTERTOUCH);
    data[11] = IPC_ReadAdcBufferAveraged(IPC_ADC_RIBBON1);
    data[12] = IPC_ReadAdcBufferAveraged(IPC_ADC_RIBBON2);
    BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_SENSORS_RAW, 13, data);
    BB_MSG_SendTheBuffer();
  }
}

/*****************************************************************************
* @brief  ADC_WORK_Process -
*         The four ADC_WORK_Process* sections are separated by only one 125us time-slice.
*
******************************************************************************/
void ADC_WORK_Process1(void)
{
  if (suspend)
    return;
  //  DBG_GPIO3_1_On();
  NL_EHC_ProcessControllers1();  // External Hardware Controllers (ADCs 7..4)
}

void ADC_WORK_Process2(void)
{
  if (suspend)
    return;
  NL_EHC_ProcessControllers2();  // External Hardware Controllers (ADCs 3..0)
}

void ADC_WORK_Process3(void)
{
  if (suspend)
    return;
  NL_EHC_ProcessControllers3();  // External Hardware Controllers (EEPROM, and send EHC data)

  SendRawValues();  // send raw ADC values if requested
}

void ADC_WORK_Process4(void)
{
  if (suspend)
    return;
  ProcessAftertouch();
  ProcessBender();
  ProcessRibbons();

  if (pollDataRequest)
  {
    pollDataRequest = 0;
    PollHWValues();
  }
  else
    SendAEMessages();  // send all AE messages in one go, to preserve priorities, like ribbons updating last etc
  //  DBG_GPIO3_1_Off();
}
