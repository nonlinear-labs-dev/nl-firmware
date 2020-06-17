/******************************************************************************/
/** @file		nl_status.c
    @date		2020-04-14
    @version	0
    @author		KSTR
    @brief		system status global variable
*******************************************************************************/
#include "sys/nl_status.h"
#include "ipc/emphase_ipc.h"
#include "drv/nl_cgu.h"
#include "tcd/nl_tcd_msg.h"

#pragma GCC diagnostic ignored "-Wunused-function"

NL_systemStatus_T NL_systemStatus = {
  .M4_ticker                 = 0,
  .COOS_totalOverruns        = 0,
  .COOS_maxTasksPerSlice     = 0,
  .COOS_maxTaskTime          = 0,
  .COOS_maxDispatchTime      = 0,
  .BB_MSG_bufferOvers        = 0,
  .TCD_usbJams               = 0,
  .M0_ADCTime                = 0,
  .M0_KbsIrqOver             = 0,
  .DroppedMidiMessageBuffers = 0,
  .MissedKeybedEventsScanner = 0,
  .MissedKeybedEventsTCD     = 0,
};

uint16_t NL_STAT_GetDataSize(void)
{
  return sizeof(NL_systemStatus) / sizeof(uint16_t);
}

static uint16_t M4TicksToUS(uint32_t ticks)
{
  uint32_t ret = M4_PERIOD_US * ticks;
  if (ret > 65535)
    ret = 65535;
  return (uint16_t) ret;
}

static uint16_t M0JiffiesToNS(uint16_t jiffies)
{
  uint32_t ret = (NL_LPC_CLK_PERIOD_100PS * jiffies + 4) / 10;  // rounded
  if (ret > 65535)
    ret = 65535;
  return (uint16_t) ret;
}

void CheckMissedKeybedEvents(void)
{
  uint16_t missed = 0;
  for (uint16_t i = 0; i < 64; i++)
    if (s.keyOnOffCntr[i] && missed < 65535)
      missed++;
  NL_systemStatus.MissedKeybedEventsScanner = missed;

  missed = 0;
  for (uint16_t i = 0; i < 128; i++)
    if (TCD_keyOnOffCntr[i] && missed < 65535)
      missed++;
  NL_systemStatus.MissedKeybedEventsTCD = missed;
}

void NL_STAT_GetData(uint16_t *buffer)
{
  CheckMissedKeybedEvents();
  uint32_t ticker = NL_systemStatus.M4_ticker = (s.ticker) - NL_systemStatus.M4_ticker;
  NL_systemStatus.M4_ticker                   = s.ticker;
  *(buffer++)                                 = ticker & 0xFFFF;
  *(buffer++)                                 = ticker >> 16;
  *(buffer++)                                 = NL_systemStatus.COOS_totalOverruns;
  *(buffer++)                                 = NL_systemStatus.COOS_maxTasksPerSlice;
  *(buffer++)                                 = M4TicksToUS(NL_systemStatus.COOS_maxTaskTime);
  *(buffer++)                                 = M4TicksToUS(NL_systemStatus.COOS_maxDispatchTime);
  *(buffer++)                                 = NL_systemStatus.DroppedMidiMessageBuffers;
  *(buffer++)                                 = NL_systemStatus.BB_MSG_bufferOvers;
  *(buffer++)                                 = NL_systemStatus.TCD_usbJams;
  *(buffer++) = NL_systemStatus.M0_ADCTime = M4TicksToUS(IPC_GetAndResetADCTime());
  *(buffer++) = NL_systemStatus.M0_KbsIrqOver = (s.RitCrtlReg != 0) ? s.RitCrtlReg & 1 : 0xFFFF;
  s.RitCrtlReg                                = 0;
  *(buffer++)                                 = NL_systemStatus.MissedKeybedEventsScanner;
  *(buffer++)                                 = NL_systemStatus.MissedKeybedEventsTCD;

  NL_systemStatus.COOS_totalOverruns        = 0;
  NL_systemStatus.COOS_maxTasksPerSlice     = 0;
  NL_systemStatus.COOS_maxTaskTime          = 0;
  NL_systemStatus.COOS_maxDispatchTime      = 0;
  NL_systemStatus.BB_MSG_bufferOvers        = 0;
  NL_systemStatus.TCD_usbJams               = 0;
  NL_systemStatus.DroppedMidiMessageBuffers = 0;
  //NL_systemStatus.MissedKeybedEventsScanner = 0;
  //NL_systemStatus.MissedKeybedEventsTCD     = 0;
}

uint16_t NL_STAT_GetKeyDataSize(void)
{
  return (64 + 128);
}

void NL_STAT_GetKeyData(uint16_t *buffer)
{
  uint16_t *p = buffer;
  for (uint16_t i = 0; i < 64; i++)
    *(p++) = (uint16_t) s.keyOnOffCntr[i];
  for (uint16_t i = 0; i < 128; i++)
    *(p++) = (uint16_t) TCD_keyOnOffCntr[i];
}
