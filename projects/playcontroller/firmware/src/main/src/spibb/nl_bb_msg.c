/*
 *  last mod: 2016-04-27 DTZ
 *  Created on: 21.01.2015
 *      Author: ssc
 *  changed 2020-03-31 KSTR
 */

//#define BB_MSG_DBG  // if defined, discards normal BB_MSG_WriteMessage*() messages and enables the *_DBG() variants instead

#include "nl_bb_msg.h"
#include "nl_spi_bb.h"

#include "tcd/nl_tcd_adc_work.h"
#include "tcd/nl_tcd_poly.h"
#include "tcd/nl_tcd_msg.h"
#include "drv/nl_dbg.h"
#include "sup/nl_sup.h"
#include "sys/nl_status.h"
#include "tcd/ehc/nl_ehc_ctrl.h"
#include "sys/nl_eeprom.h"
#include "sys/nl_coos.h"
#include "sys/nl_version.h"
#include "sys/nl_watchdog.h"
#include "sys/nl_uhid.h"
#include "heartbeat/nl_heartbeat.h"
#include "playcontroller/playcontroller-defs.h"

#define SENDBUFFER_SIZE 510  // 16-bit words, stays below the maximum of 1020 bytes

static uint16_t sendBuffer[SENDBUFFER_SIZE] = {};
static uint32_t sendBufferLen               = 0;  // behind the 4-byte header

void SignalBufferOverrun(void);

void BB_MSG_WriteMessage_DBG(uint16_t type, uint16_t length, uint16_t* data)
{
#ifdef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + length + 2);

  if (remainingBuffer < 0)
  {
    SignalBufferOverrun();
  }

  sendBuffer[sendBufferLen] = type;
  sendBufferLen++;
  sendBuffer[sendBufferLen] = length;
  sendBufferLen++;

  if (data != NULL)
  {
    uint32_t i;

    for (i = 0; i < length; i++)
    {
      sendBuffer[sendBufferLen] = data[i];
      sendBufferLen++;
    }
  }
  BB_MSG_SendTheBuffer();
#endif
}

void BB_MSG_WriteMessage1Arg_DBG(uint16_t type, uint16_t arg)
{
#ifdef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + 3);

  if (remainingBuffer < 0)
  {
    SignalBufferOverrun();
  }

  sendBuffer[sendBufferLen] = type;
  sendBufferLen++;
  sendBuffer[sendBufferLen] = 1;
  sendBufferLen++;

  sendBuffer[sendBufferLen] = arg;
  sendBufferLen++;

  BB_MSG_SendTheBuffer();
#endif
}

void SignalBufferOverrun(void)
{
  // TODO :
  // Is throwing away the complete buffer really a good solution?
  sendBufferLen = 0;
  if (NL_systemStatus.BB_MSG_bufferOvers < 0xFFFF)
    NL_systemStatus.BB_MSG_bufferOvers++;
  DBG_Led_Error_TimedOn(2);
}

/**********************************************************************
 * @brief		Writing a generic message into the SPI send buffer
 * @param[in]	type	message type (see defines)
 * @param[in]	length	number of 16-bit data fields
 * @param[in]	data	pointer to an array of 16-bit data fields
 * @return		>= 0: "success" and remaining buffer space
 * 				-1: "buffer is full, try again later"
 *
 * 				NOTE: NO ASSERTION IN THIS FUNCTION => RECURSION
 **********************************************************************/

int32_t BB_MSG_WriteMessage(uint16_t type, uint16_t length, uint16_t* data)
{
#ifndef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + length + 2);

  if (remainingBuffer < 0)
  {
    SignalBufferOverrun();
    return -1;  // buffer is full
  }

  sendBuffer[sendBufferLen] = type;
  sendBufferLen++;
  sendBuffer[sendBufferLen] = length;
  sendBufferLen++;

  if (data != NULL)
  {
    uint32_t i;

    for (i = 0; i < length; i++)
    {
      sendBuffer[sendBufferLen] = data[i];
      sendBufferLen++;
    }
  }

  return remainingBuffer;
#else
  return 0;
#endif
}

/**********************************************************************
 * @brief		Writes a message with 2 arguments into the send buffer
 * @param[in]	type	message type (see defines)
 * @param[in]	arg0	e.g. the identifier of a parameter
 * @param[in]	arg1	e.g. the value of the parameter
 * @return		>= 0: "success" and remaining buffer space
 * 				-1: "buffer is full, try again later"
 **********************************************************************/

int32_t BB_MSG_WriteMessage2Arg(uint16_t type, uint16_t arg0, uint16_t arg1)
{
#ifndef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + 4);

  if (remainingBuffer < 0)
  {
    SignalBufferOverrun();
    return -1;  // buffer is full
  }

  sendBuffer[sendBufferLen] = type;
  sendBufferLen++;
  sendBuffer[sendBufferLen] = 2;
  sendBufferLen++;

  sendBuffer[sendBufferLen] = arg0;
  sendBufferLen++;

  sendBuffer[sendBufferLen] = arg1;
  sendBufferLen++;

  return remainingBuffer;
#else
  return 0;
#endif
}

/**********************************************************************
 * @brief		Writes a message with 1 argument into the send buffer
 * @param[in]	type	message type (see defines)
 * @param[in]	arg		e.g. the morph position
 * @return		>= 0: "success" and remaining buffer space
 * 				-1: "buffer is full, try again later"
 **********************************************************************/

int32_t BB_MSG_WriteMessage1Arg(uint16_t type, uint16_t arg)
{
#ifndef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + 3);

  if (remainingBuffer < 0)
  {
    SignalBufferOverrun();
    return -1;  // buffer is full
  }

  sendBuffer[sendBufferLen] = type;
  sendBufferLen++;
  sendBuffer[sendBufferLen] = 1;
  sendBufferLen++;

  sendBuffer[sendBufferLen] = arg;
  sendBufferLen++;

  return remainingBuffer;
#else
  return 0;
#endif
}

/**********************************************************************
 * @brief		Writes a message with 0 arguments into the send buffer
 * @param[in]	type	message type (see defines)
 * @return		>= 0: "success" and remaining buffer space
 * 				-1: "buffer is full, try again later"
 **********************************************************************/

int32_t BB_MSG_WriteMessageNoArg(uint16_t type)
{
#ifndef BB_MSG_DBG
  int32_t remainingBuffer = SENDBUFFER_SIZE - (sendBufferLen + 1);

  if (remainingBuffer < 0)
  {
    return -1;  // buffer is full
  }

  sendBuffer[sendBufferLen] = (type << 16);  // no data fields
  sendBufferLen++;

  return remainingBuffer;
#else
  return 0;
#endif
}

/**********************************************************************
 * @brief		Sending a package with one or more messages via SPI
 * @return		buffer length in bytes - success
 *              0 = "nothing to send"
 *              -1 = "failure, try again later"
 *              sendBufferLen*2 = "success"
 **********************************************************************/

int32_t BB_MSG_SendTheBuffer(void)
{
  if (sendBufferLen == 0)
    return 0;  // nothing to send

  uint8_t* buff    = (uint8_t*) sendBuffer;
  uint32_t success = SPI_BB_Send(buff, sendBufferLen * 2);

  if (success)
  {
    sendBufferLen = 0;  // position for first messages of next buffer (behind the header)
    return success;
  }
  else  // sending failed, try again later
    return -1;
}

// -- integrity check test messaging
static void ProcessTestMessage(uint16_t length, uint16_t* data)
{
  static uint16_t seqNo  = 0xFFFF;
  uint16_t        dataOk = 1;
  uint16_t        seqOk  = ((uint16_t)(1 + seqNo) == data[0]);
  seqNo                  = data[0];
  if (length > 1)
    for (uint16_t i = 0; i < length - 1; i++)
      if (data[i + 1] != i)
      {
        dataOk = 0;
        break;
      }
  data[0] = (data[0] & 0x3FFF) | (!dataOk << 14) | (!seqOk << 15);
  BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_TEST_MSG, data[0]);
  BB_MSG_SendTheBuffer();
}

/*****************************************************************************
 * @brief		BB_MSG_ReceiveCallback - Callback for receiving messages from
 * the Beaglebone (called from the PackageParser of the spi_bb driver).
 *****************************************************************************/

void BB_MSG_ReceiveCallback(uint16_t type, uint16_t length, uint16_t* data)
{
  // data[0]  - parameter id
  // data[1]  - first value
  // data[2]  - second value

  if (type == PLAYCONTROLLER_BB_MSG_TYPE_TEST_MSG)
    ProcessTestMessage(length, data);
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_RIBBON_CAL)
    ADC_WORK_SetRibbonCalibration(length, data);
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_EHC_CONFIG)
    NL_EHC_SetEHCconfig(data[0], data[1]);  // Configurate External Hardware Controller
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_KEY_EMUL)
    POLY_ForceKey(data[0], data[1], data[2]);
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_KEYMAP_DATA)
    POLY_SetKeyRemapTable(length, data);
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_SETTING)
  {
    switch (data[0])
    {
      case PLAYCONTROLLER_SETTING_ID_PLAY_MODE_UPPER_RIBBON_BEHAVIOUR:  // Play mode ribbon 1 behaviour
        ADC_WORK_SetRibbon1Behaviour(data[1]);                          // 0: Abs + Non-Return, 1: Abs + Return, 2: Rel + Non-Return, 3: Rel + Return
        break;
      case PLAYCONTROLLER_SETTING_ID_PLAY_MODE_LOWER_RIBBON_BEHAVIOUR:  // Play mode ribbon 2 behaviour
        ADC_WORK_SetRibbon2Behaviour(data[1]);                          // 0: Abs + Non-Return, 1: Abs + Return, 2: Rel + Non-Return, 3: Rel + Return
        break;
      case PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE:
        ADC_WORK_SetRibbon1OutputValue(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_VALUE:
        ADC_WORK_SetRibbon2OutputValue(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_BASE_UNIT_UI_MODE:  // "Unit Mode" - Ribbon 1 can be switched between Edit and Play mode.
        ADC_WORK_SetRibbon1EditMode(data[1]);            // 0: Play, 1: Parameter Edit
        break;
      case PLAYCONTROLLER_SETTING_ID_EDIT_MODE_RIBBON_BEHAVIOUR:  // Parameter edit mode ribbon behaviour
        ADC_WORK_SetRibbon1EditBehaviour(data[1]);                // 0: Rel, 1: Abs
        break;
      case PLAYCONTROLLER_SETTING_ID_RIBBON_REL_FACTOR:  // Factor for the increments when a ribbon is in Relative mode
        ADC_WORK_SetRibbonRelFactor(data[1]);            // factor = data[1] / 256
        break;
      case PLAYCONTROLLER_SETTING_ID_VELOCITY_CURVE:  // Velocity Curve
        POLY_Select_VelTable(data[1]);                // Parameter: 0 = very soft ... 4 = very hard
        break;
      case PLAYCONTROLLER_SETTING_ID_AFTERTOUCH_CURVE:  // Aftertouch Curve
        ADC_WORK_Select_AftertouchTable(data[1]);       // 0: soft, 1: normal, 2: hard
        break;
      case PLAYCONTROLLER_SETTING_ID_BENDER_CURVE:  // Bender Curve
        ADC_WORK_Select_BenderTable(data[1]);       // 0: soft, 1: normal, 2: hard
        break;
      case PLAYCONTROLLER_SETTING_ID_SOFTWARE_MUTE_OVERRIDE:
        SUP_SetMuteOverride(data[1]);  // enable/disable Software Mute Override and value
        break;                         // see sup/nl_sup.h for bit patterns
      case PLAYCONTROLLER_SETTING_ID_SEND_RAW_SENSOR_DATA:
        ADC_WORK_SetRawSensorMessages(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_PEDAL_1_TYPE:
      case PLAYCONTROLLER_SETTING_ID_PEDAL_2_TYPE:
      case PLAYCONTROLLER_SETTING_ID_PEDAL_3_TYPE:
      case PLAYCONTROLLER_SETTING_ID_PEDAL_4_TYPE:
        NL_EHC_SetLegacyPedalType(data[0] - PLAYCONTROLLER_SETTING_ID_PEDAL_1_TYPE, data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_ENABLE_EHC:
        NL_EHC_Enable(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_LOGGING:
        POLY_KeyLogging(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_MAPPING:
        POLY_EnableKeyMapping(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_ENABLE_UI_PARAMETER_MSGS:
        ADC_WORK_EnableSendUIMessages(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_AUDIO_ENGINE_CMD:
        MSG_SendAEDevelopperCmd(data[1]);
        break;
      case PLAYCONTROLLER_SETTING_ID_SYSTEM_SPECIAL:
        switch (data[1])
        {
          case SYS_SPECIAL_RESET_HEARTBEAT:
            HBT_ResetCounter();
            break;
          case SYS_SPECIAL_RESET_SYSTEM:
            SYS_Reset();
            break;
          case SYS_SPECIAL_ENABLE_MIDI:
            MSG_DropMidiMessages(0);
            break;
        }
        break;
      default:
        // do nothing
        type = 0;  // to set a breakpoint only
        break;
    }
  }
  else if (type == PLAYCONTROLLER_BB_MSG_TYPE_REQUEST)
  {
    switch (data[0])
    {
      case PLAYCONTROLLER_REQUEST_ID_SW_VERSION:  // sending the firmware version to the BB
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_SW_VERSION, SW_VERSION);
        BB_MSG_SendTheBuffer();
        break;
      case PLAYCONTROLLER_REQUEST_ID_UNMUTE_STATUS:  // sending the muting status to the BB
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_UNMUTE_STATUS, SUP_GetUnmuteStatusBits());
        BB_MSG_SendTheBuffer();
        break;
      case PLAYCONTROLLER_REQUEST_ID_EHC_DATA:  // send EHC data to BB
        NL_EHC_RequestToSendEHCdata();
        break;
      case PLAYCONTROLLER_REQUEST_ID_CLEAR_EEPROM:
        NL_EEPROM_RequestFullErase();
        break;
      case PLAYCONTROLLER_REQUEST_ID_STAT_DATA:
      {
        uint16_t words = NL_STAT_GetDataSize();
        uint16_t buffer[words];
        NL_STAT_GetData(buffer);
        BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_STAT_DATA, words, buffer);
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_STAT_DATA, 1);
        BB_MSG_SendTheBuffer();
        break;
      }
      case PLAYCONTROLLER_REQUEST_ID_AT_MAX_DATA:
      {
        uint16_t  words  = ADC_WORK_GetATAdcDataSize();
        uint16_t* buffer = ADC_WORK_GetATAdcData();
        BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_AT_MAX_DATA, words, buffer);
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_AT_MAX_DATA, 1);
        BB_MSG_SendTheBuffer();
        break;
      }
      case PLAYCONTROLLER_REQUEST_ID_UHID64:
      {
        BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_UHID64, 4, (uint16_t*) &NL_uhid64);
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_UHID64, 1);
        BB_MSG_SendTheBuffer();
        break;
      }
      case PLAYCONTROLLER_REQUEST_ID_POLLHWS:
      {
        ADC_WORK_PollRequestHWValues();
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_POLLHWS, 1);
        BB_MSG_SendTheBuffer();
        break;
      }
      case PLAYCONTROLLER_REQUEST_ID_CLEAR_STAT:
        NL_STAT_ClearData();
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_CLEAR_STAT, 1);
        BB_MSG_SendTheBuffer();
        break;
      case PLAYCONTROLLER_REQUEST_ID_EHC_EEPROMSAVE:
        NL_EHC_ForceEepromUpdate();
        break;
#if LPC_KEYBED_DIAG
      case PLAYCONTROLLER_REQUEST_ID_KEYCNTR_DATA:
      {
        uint16_t words = NL_STAT_GetKeyDataSize();
        uint16_t buffer[words];
        NL_STAT_GetKeyData(buffer);
        BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_KEYCNTR_DATA, words, buffer);
        BB_MSG_WriteMessage2Arg(PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION, PLAYCONTROLLER_NOTIFICATION_ID_KEYCNTR_DATA, 1);
        BB_MSG_SendTheBuffer();
        break;
      }
#endif
      default:
        type = 0;  // to set a breakpoint only
        break;
    }
  }
  else
  {
    type = 0;  // to set a breakpoint only
  }
}
