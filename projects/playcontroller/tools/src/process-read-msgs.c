#include "process-read-msgs.h"
#include "shared/playcontroller/playcontroller-defs.h"
#include "shared/playcontroller/playcontroller-converters.h"
#include "globals.h"

char paramNameTable[][NUM_HW_SOURCES] = {
  "EHC 1     ",
  "EHC 2     ",
  "EHC 3     ",
  "EHC 4     ",
  "BENDER    ",
  "AFTERTOUCH",
  "RIBBON 1  ",
  "RIBBON 2  ",
  "EHC 5     ",
  "EHC 6     ",
  "EHC 7     ",
  "EHC 8     ",
  "LASTKEY",
};
char paramShortNameTable[][16] = {
  "E1",
  "E2",
  "E3",
  "E4",
  "PB",
  "AT",
  "R1",
  "R2",
  "E5",
  "E6",
  "E7",
  "E8",
  "--",
  "--",
  "--",
  "--",
};

// ==================================================================================

typedef enum
{
  RED,
  GREEN,
  DEFAULT
} color_t;

void setColor(color_t color)
{
  switch (color)
  {
    case RED:
      printf("\033[0;31m");
      break;
    case GREEN:
      printf("\033[0;32m");
      break;
    case DEFAULT:
      printf("\033[0m");
      break;
  }
}

int greenNotRed(int condition)
{
  setColor(condition ? GREEN : RED);
  return condition;
}

void setSettledColor(uint16_t const index, uint16_t const settled)
{
  if (index == (settled & 0x7FFF))
    greenNotRed((settled & 0x8000) != 0);
}

void cursorUp(uint8_t lines)
{
  char buffer[12];
  sprintf(buffer, "\033[%dA", lines);
  printf(buffer);
}

// ==================================================================================
void dump(uint16_t const cmd, uint16_t const len, uint16_t *const data, uint16_t const flags)
{
  if (flags & NO_HEXDUMP)
    return;
  printf("hexdump cmd=%04X len=%04X: data=", cmd, len);
  for (int i = 0; i < len; i++)
    printf("%04X ", data[i]);
  printf("\n");
}

// ==================================================================================, uint16_t const flags

void printfResistance(double kOhms)
{
  kOhms *= 10;
  if (kOhms > 99.9)
    printf("%5.1lfk", kOhms);
  else if (kOhms > 0.999)
    printf("%5.2lfk", kOhms);
  else
    printf("%5.0lfR", kOhms * 1000);
}

void printfPercent(double factor)
{
  factor *= 100;
  if (factor > 999.9)
    printf("%5.0lf%%", factor);  // integer value
  else if (factor > 9.99)
    printf("%5.1lf%%", factor);  // one fractional digit
  else
    printf("%5.2lf%%", factor);  // two fractional digits
}

void printfControlVoltage(double const factor)
{
  printf("%5.3lfV", factor * 5.0);
}

void printfDeadZones(uint16_t const deadZones)
{
  printf("%2d %2d", deadZones & 0xFF, deadZones >> 8);
}

void displayCounter(void)
{
  static uint32_t cntr = 0;
  printf("%010u ", cntr++);
}

// ==================================================================================, uint16_t const flags
int processReadMsgs(uint16_t const cmd, uint16_t const len, uint16_t *const data, uint32_t flags)
{
  int       i;
  uint16_t *p;
  double    last[8], intermediate[8], min[8], max[8], scale[8];
  uint16_t  adc[8], deadzone[8];
  int       same;

  EHC_ControllerConfig_T config[8];
  EHC_ControllerStatus_T status[8];

  static uint32_t lastMessage = ~0;
  static double   makeTime    = 0;

  switch (cmd)
  {
    case PLAYCONTROLLER_BB_MSG_TYPE_KEY_EMUL:
      if (flags & NO_KEY_LOG)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 3)
      {
        printf("KEY : wrong length of %d\n", len);
        return 3;
      }
      if ((same = !(flags & NO_OVERLAY) && (lastMessage == (((uint32_t) cmd << 16)) + (uint32_t) data[0])))
        cursorUp(1);
      displayCounter();
      double time = (data[1] | (int) data[2] << 16) / 1000.0;
      if (time > 0)
      {
        makeTime = time;
        printf("KEY = %3d, %8.2lfms            ", data[0], time);
      }
      else
      {
        if (same)
          printf("KEY = %3d, %8.2lfms  %8.2lfms", data[0], makeTime, time);
        else
          printf("KEY = %3d,             %8.2lfms", data[0], time);
      }
      printf("\n");
      lastMessage = (cmd << 16) + data[0];
      return 1;

#if LPC_KEYBED_DIAG
    case PLAYCONTROLLER_BB_MSG_TYPE_KEYCNTR_DATA:
      if (flags & NO_KEY_LOG)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 64 + 128)
      {
        printf("KEY_COUNTERS : wrong length of %d\n", len);
        return 3;
      }
      if ((same = !(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16))))
        cursorUp(20);
      displayCounter();
      puts("Key Counters");
      puts(" Scanner:");
      for (int i = 0; i < 61; i++)
      {
        if (data[i])
          printf("%+5d ", data[i]);
        else
          printf("    . ");
        if (i % 12 == 11)
          printf("\n");
      }
      puts("\n TCD:");
      for (int i = 0; i < 128; i++)
      {
        if (data[i + 64])
          printf("%+5d ", data[i + 64]);
        else
          printf("    . ");
        if (i % 12 == 11)
          printf("\n");
      }
      printf("\n");
      lastMessage = (cmd << 16);
      return 1;
#endif

    case PLAYCONTROLLER_BB_MSG_TYPE_PARAMETER:
      if (flags & NO_PARAMS)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 2)
      {
        printf("PARAM : wrong length of %d\n", len);
        return 3;
      }
      if (data[0] > NUM_HW_SOURCES)
        return 1;
      if (!(flags & NO_OVERLAY) && (lastMessage == (((uint32_t) cmd << 16)) + (uint32_t) data[0]))
        cursorUp(1);
      displayCounter();
      if (data[0] <= HW_SOURCE_ID_PEDAL_8)
        printf("PARAM (HWSID %02d) %s = %5d (%5.1lf%%)", data[0], paramNameTable[data[0]], data[1], 100.0 * data[1] / 16000.0);
      else  // key
        printf("PARAM (HWSID %02d) %s : key number=%3d, note number=%3d", data[0], paramNameTable[data[0]], data[1] >> 8, data[1] & 0xFF);
      printf("\n");
      lastMessage = (cmd << 16) + data[0];
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION:
      if (flags & NO_NOTIFICATION)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 2)
      {
        printf("NOTIFICATION : wrong length of %d\n", len);
        return 3;
      }
      if (!(flags & NO_OVERLAY) && (lastMessage == (((uint32_t) cmd << 16)) + (uint32_t) data[0]))
        cursorUp(1);
      displayCounter();
      switch (data[0])
      {
        case PLAYCONTROLLER_NOTIFICATION_ID_CLEAR_EEPROM:
          printf("NOTIFICATION : EEPROM cleared\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_STAT_DATA:
          printf("NOTIFICATION : Status Data sent\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_UHID64:
          printf("NOTIFICATION : UHID64 sent\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_POLLHWS:
          printf("NOTIFICATION : Initiated Poll of Hardware-Sources\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_AT_TEST_DATA:
          printf("NOTIFICATION : Aftertouch Data sent\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_CLEAR_STAT:
          printf("NOTIFICATION : Status Data cleared\n");
          break;
#if LPC_KEYBED_DIAG
        case PLAYCONTROLLER_NOTIFICATION_ID_KEYCNTR_DATA:
          printf("NOTIFICATION : Key Errors Counters Data sent\n");
          break;
#endif
        case PLAYCONTROLLER_NOTIFICATION_ID_SW_VERSION:
          printf("NOTIFICATION : Software Version: %hu\n", data[1]);
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_EHC_DATA:
          printf("NOTIFICATION : EHC data sent: %s\n", data[1] ? "success  " : "failed  ");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_EHC_EEPROMSAVE:
          printf("NOTIFICATION : EHC data save to EEPROM: %s\n", data[1] ? "started successfully" : "postponed/failed   ");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_UNMUTE_STATUS:
          printf("NOTIFICATION : ");
          data[0] = data[1];
          goto ShowMuteStatus;
        case PLAYCONTROLLER_NOTIFICATION_ID_TEST_MSG:
          printf("NOTIFICATION : TestMessage received, seq. #: %5hu", data[1] & 0x3FFF);
          if (data[1] & (1 << 15))
            printf(" (seq. broken)");
          else
            printf("              ");
          if (data[1] & (1 << 14))
            printf(" (data corrupt)");
          else
            printf("               ");
          printf("\n");
          break;
        case PLAYCONTROLLER_NOTIFICATION_ID_AT_STATUS:
        {
          AT_status_T s = AT_uint16ToStatus(data[1]);
          printf("NOTIFICATION : AT status: legacyMode=%u, calibrated=%u, validEEPROM=%u, maskedKeys=%u, silentKeys=%u\n",
                 s.legacyMode, s.calibrated, s.eepromValid, s.maskedKeys, s.silentKeys);
          break;
        }
        case PLAYCONTROLLER_NOTIFICATION_ID_BNDR_STATUS:
        {
          BNDR_status_T s = BNDR_uint16ToStatus(data[1]);
          printf("NOTIFICATION : Bender status: legacy=%u, (ever)zeroed=(%u)%u, Stop L/R=%u/%u, ZeroPos bad/normal=%u/%u, Settling Mode/Fine/Coarse=%u/%u/%u\n",
                 s.legacyMode, s.everZeroed, s.zeroed, s.leftEndStop, s.rightEndStop, s.offZero, s.reasonableZero, s.useFineSettling, s.settledFine, s.settledCoarse);
          break;
        }
        case PLAYCONTROLLER_NOTIFICATION_ID_BNDR_RESET:
          printf("NOTIFICATION : Pitchbender reset\n");
          break;

        default:
          printf("NOTIFICATION : unknown ID=%d, data=%d     \n", data[0], data[1]);
          break;
      }
      lastMessage = (cmd << 16) + data[0];
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_STAT_DATA:
      if (flags & NO_STATDATA)
        return 0;
      dump(cmd, len, data, flags);
#if LPC_KEYBED_DIAG
      if (len != 13)
#else
      if (len != 11)
#endif
      {
        printf("STATUS : wrong length of %d\n", len);
        return 3;
      }
      if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
#if LPC_KEYBED_DIAG
        cursorUp(16);
#else
        cursorUp(13);
#endif
      displayCounter();
      printf("SYSTEM STATUS:\n");
      printf("cleared with 'lpc req clear-status':\n");
      printf("  M4 ticker      : %10u ticks since last call\n", (unsigned) data[0] + ((unsigned) data[1] << 16));
      printf("  Scheduler      : %5d task overruns\n", data[2]);
      printf("  Scheduler      : %5d tasks max. per time-slice\n", data[3]);
      printf("  Scheduler      : %5dus max. task runtime\n", data[4]);
      printf("  Scheduler      : %5dus max. time-slice\n", data[5]);
      printf("  Midi Buffers   : %5d dropped\n", data[6]);
      printf("  BBB Msg        : %5d buffer overruns / ESPI send fails\n", data[7]);
      printf("  TCD Msg        : %5d buffer overruns / USB send fails\n", data[8]);
      printf("cleared after every call:\n");
      printf("  M0 ADC Scanner : %5dus max. round trip time\n", data[9]);
      if (data[10] != 0xFFFF)
        printf("  M0 Key Scanner : %5d (overrun flag)\n", data[10]);
      else
        printf("  M0 Key Scanner : -n/a- (overrun flag)\n");
#if LPC_KEYBED_DIAG
      printf("cleared only by reset:\n");
      printf("  Keybed:        : %5d missed events (Scanner)\n", data[11]);
      printf("  Keybed:        : %5d missed events (TCD)\n", data[12]);
#endif
      lastMessage = cmd << 16;
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_UHID64:
      if (flags & NO_UHID)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 4)
      {
        printf("STATUS : wrong length of %d\n", len);
        return 3;
      }
      if (flags & NO_REDUCED)
      {
        if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
          cursorUp(1);
        displayCounter();
        printf("Unique Hardware ID (64bit): ");
      }
      printf("%04X-%04X-%04X-%04X\n", data[3], data[2], data[1], data[0]);
      lastMessage = cmd << 16;
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_HEARTBEAT:
      if (flags & NO_HEARTBEAT)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 4)
      {
        printf("HEARTBEAT : wrong length of %d\n", len);
        return 3;
      }
      uint64_t heartbeat = 0;
      for (int i = 3; i >= 0; i--)
      {
        heartbeat <<= 16;
        heartbeat |= data[i];
      }
      if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
        cursorUp(1);
      displayCounter();
      printf("HEARTBEAT : %8llu\n", heartbeat);
      lastMessage = cmd << 16;
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_AT_TEST_DATA:
      if ((flags & NO_AT_DATA_OHMS) == 0)
      {
        dump(cmd, len, data, flags);
        if (len != 64)
        {
          printf("AT-DATA : wrong length of %d\n", len);
          return 3;
        }
        if (flags & NO_REDUCED)
        {
          if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
            cursorUp(8);
          displayCounter();
          printf("Aftertouch Sensor Ohms per Key\n");
          printf(" Oct  C   "
                 " C#  "
                 " D   "
                 " D#  "
                 " E   "
                 " F   "
                 " F#  "
                 " G   "
                 " G#  "
                 " A   "
                 " Bb  "
                 " B   "
                 "\n");
          p = data;
          i = 0;
          for (int octave = 0; octave < 5; octave++)
          {
            printf("%2d ", octave);
            for (int note = 0; note < 12; note++)
            {
              double ohms = 0;
              if (*p)
                ohms = 1000.0 / (*p / 4095.0) - 1000.0;
              setSettledColor(i, data[61]);
              printf("%5.0lf", ohms);
              setColor(DEFAULT);
              p++;
              i++;
            }
            printf("\n");
          }
          printf("%2d ", 6);
          double ohms = 0;
          if (*p)
            ohms = 1000.0 / (*p / 4095.0) - 1000.0;
          setSettledColor(i, data[61]);
          printf("%5.0lf\n", ohms);
          setColor(DEFAULT);
        }
        else
        {
          if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
            cursorUp(61);
          for (int i = 0; i < 61; i++)
            if (data[i])
              printf("%.0lf\n", 1000.0 / (data[i] / 4095.0) - 1000.0);
            else
              printf("0\n");
        }
        lastMessage = cmd << 16;
        return 1;
      }
      else if ((flags & NO_AT_DATA) == 0)
      {
        dump(cmd, len, data, flags);
        if (len != 64)
        {
          printf("AT-DATA : wrong length of %d\n", len);
          return 3;
        }
        if (flags & NO_REDUCED)
        {
          if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
            cursorUp(8);
          displayCounter();
          printf("Aftertouch Sensor ADC value per Key. Force [mN]: %5d, CalibPoint %5d \n", data[62], data[63]);
          printf(" Oct  C   "
                 " C#  "
                 " D   "
                 " D#  "
                 " E   "
                 " F   "
                 " F#  "
                 " G   "
                 " G#  "
                 " A   "
                 " Bb  "
                 " B   "
                 "\n");
          p = data;
          i = 0;
          for (int octave = 0; octave < 5; octave++)
          {
            printf("%2d ", octave);
            for (int note = 0; note < 12; note++)
            {
              setSettledColor(i, data[61]);
              printf("%5d", *p);
              setColor(DEFAULT);
              p++;
              i++;
            }
            printf("\n");
          }
          printf("%2d ", 6);
          setSettledColor(i, data[61]);
          printf("%5d\n", *p);
          setColor(DEFAULT);
        }
        else  // reduced
        {
          if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
            cursorUp(61);
          for (int i = 0; i < 60; i++)
            printf("%d,", data[i]);
          printf("%d,\n", data[60]);
        }
        lastMessage = cmd << 16;
        return 1;
      }
      return 0;

    case PLAYCONTROLLER_BB_MSG_TYPE_SENSORS_RAW:
      if ((flags & NO_SENSORSRAW) && (flags & NO_RIBBONS))
        return 0;
      dump(cmd, len, data, flags);
      if (len != 13)
      {
        printf("RAW SENSORS : wrong length of %d\n", len);
        return 3;
      }
      if (!(flags & NO_SENSORSRAW))
      {
        if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
          cursorUp(1);
        displayCounter();
        printf("RAW SENSORS: ");
        for (int i = 3; i >= 0; i--)
          printf("%c ", (data[0] & (1 << i)) ? '1' : '0');
        printf(" ");
        for (int i = 1; i < 13; i++)
        {
          printf("%04hu ", data[i]);
          if (i == 8 || i == 10)
            printf("/ ");
        }
        printf("\n");
      }
      else
      {
#define RIB_ARY_SIZE (16)
#define RIB_ARY_MASK (RIB_ARY_SIZE - 1)
        static uint16_t lower[RIB_ARY_SIZE], upper[RIB_ARY_SIZE];
        static uint16_t idx;
        static int      armed;

        lower[idx] = data[12];
        upper[idx] = data[11];
        idx        = (idx + 1) & RIB_ARY_MASK;

        if (data[12] > 90 || data[11] > 90)
        {
          armed = 1;
          if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
            cursorUp(1);
          displayCounter();
          uint32_t u = 0, l = 0;
          for (uint16_t i = idx; i < idx + 8; i++)
          {
            u += upper[i & RIB_ARY_MASK];
            l += lower[i & RIB_ARY_MASK];
          }
          printf("RAW RIBBONS: ");
          printf("%5hu  %5hu\n", u / 8, l / 8);
        }
        else if (armed)
        {
          armed = 0;
          printf("\n");
        }
      }
      lastMessage = cmd << 16;
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_MUTESTATUS:
      if (flags & NO_MUTESTATUS)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 1)
      {
        printf("MUTESTATUS : wrong length of %d\n", len);
        return 3;
      }
      if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
        cursorUp(1);
      displayCounter();
    ShowMuteStatus:
      printf("MUTESTATUS: valid:%d", data[0] & SUP_UNMUTE_STATUS_IS_VALID ? 1 : 0);
      printf(" jumper:");
      if (data[0] & SUP_UNMUTE_STATUS_JUMPER_OVERRIDE)
        printf("%d", data[0] & SUP_UNMUTE_STATUS_JUMPER_VALUE ? 1 : 0);
      else
        printf("-");
      printf(" software:");
      if (data[0] & SUP_UNMUTE_STATUS_SOFTWARE_OVERRIDE)
        printf("%d", data[0] & SUP_UNMUTE_STATUS_SOFTWARE_VALUE ? 1 : 0);
      else
        printf("-");
      printf(" midi:");
      if (data[0] & SUP_UNMUTE_STATUS_MIDI_DERIVED)
        printf("%d", data[0] & SUP_UNMUTE_STATUS_MIDI_DERIVED_VALUE ? 1 : 0);
      else
        printf("-");
      printf(" physical:");
      if (data[0] & SUP_UNMUTE_STATUS_HARDWARE_IS_VALID)
        printf("%d", data[0] & SUP_UNMUTE_STATUS_HARDWARE_VALUE ? 1 : 0);
      else
        printf("-");
      printf("\n");
      lastMessage = cmd << 16;
      return 1;

    case PLAYCONTROLLER_BB_MSG_TYPE_EHC_DATA:
      if (flags & NO_EHCDATA)
        return 0;
      dump(cmd, len, data, flags);
      if (len != 8 * 9)
      {
        printf("EHC DATA : wrong length of %d\n", len);
        return 3;
      }
      p = (uint16_t *) data;
      for (i = 0; i < 8; i++)
      {
        config[i]       = EHC_uint16ToConfig(*p++);
        status[i]       = EHC_uint16ToStatus(*p++);
        last[i]         = *p++;
        intermediate[i] = *p++;
        min[i]          = *p++;
        max[i]          = *p++;
        scale[i]        = *p++;
        adc[i]          = *p++;
        deadzone[i]     = *p++;
      }

      if (!(flags & NO_OVERLAY) && (lastMessage == ((uint32_t) cmd << 16)))
        cursorUp(20);
      displayCounter();
      printf("EHC data -------------------------------------------------\n");
      printf("Controller    ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("P%d%c(%d)%c", i / 2 + 1, i & 1 ? 'R' : 'T', i, i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("ADC           ");
      for (i = 0; i < 8; i++)
      {
        printf("%5d %c", adc[i], i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // config
      printf("Config        ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%04Xh %c", EHC_configToUint16(config[i]), i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  HWSID/SIL   ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%s/%c  %c", paramShortNameTable[config[i].hwId], config[i].silent ? 'S' : '-', i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  Pot/pUl/Con ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%c/%c/%c %c", config[i].is3wire ? 'P' : '-', config[i].pullup ? 'U' : '-', config[i].continuous ? 'C' : '-', i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  Inv/aRe/AHS ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%c/%c/%d %c", config[i].polarityInvert ? 'I' : '-', config[i].doAutoRanging ? 'R' : '-', config[i].autoHoldStrength, i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // status
      printf("Status        ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%04Xh %c", EHC_statusToUint16(status[i]), i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  Ini/Plg/rEs ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%c/%c/%c %c", status[i].initialized ? 'I' : '-', status[i].pluggedIn ? 'P' : '-', status[i].isReset ? 'E' : '-', i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  Rng/Val/Stl ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%c/%c/%c %c", status[i].isAutoRanged ? 'R' : '-', status[i].outputIsValid ? 'V' : '-', status[i].isSettled ? 'S' : '-', i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      printf("  rAm/eeS/eeR ");
      for (i = 0; i < 8; i++)
      {
        greenNotRed(status[i].initialized);
        printf("%c/%c/%c %c", status[i].isRamping ? 'A' : '-', status[i].isSaved ? 'S' : '-', status[i].isRestored ? 'R' : '-', i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // last
      printf("Last          ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized && status[i].pluggedIn && status[i].outputIsValid))
          printf("%5.0lf %c", last[i], i == 7 ? '\n' : ' ');
        else
          printf("      %c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // last %
      printf("Last %%        ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized && status[i].pluggedIn && status[i].outputIsValid))
          printfPercent(last[i] / 16000);
        else
          printf("      ");
        printf("%c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // min
      printf("Min           ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized))
        {
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          printf("%5.0lf %c", min[i], i == 7 ? '\n' : ' ');
        }
        else
          printf("      %c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // max
      printf("Max           ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized))
        {
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          printf("%5.0lf %c", max[i], i == 7 ? '\n' : ' ');
        }
        else
          printf("      %c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // scale
      printf("Scale         ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized))
        {
          printf("%5.0lf %c", scale[i], i == 7 ? '\n' : ' ');
        }
        else
          printf("      %c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // dead-zones
      printf("Dead zones(%%) ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized && config[i].doAutoRanging))
        {
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          printfDeadZones(deadzone[i]);
        }
        else
          printf("      ");
        printf("%c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // min scaled
      printf("Min (scaled)  ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized && min[i] < max[i]))
        {
          double tmp = min[i] / scale[i];
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          if (config[i].is3wire)  // pot, output is in %
            printfPercent(tmp);
          else if (!config[i].is3wire && config[i].pullup)  // rheostat, output is in 00.00k to 130.1k Ohms
            printfResistance(tmp);
          else if (!config[i].is3wire && !config[i].pullup)  // control voltage, output is 0.00V to 5.00V Volts
            printfControlVoltage(tmp);
          else
            printf("      ");
        }
        else
          printf("      ");
        printf("%c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // max scaled
      printf("Max (scaled)  ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized && min[i] < max[i]))
        {
          double tmp = max[i] / scale[i];
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          if (config[i].is3wire)  // pot, output is in %
            printfPercent(tmp);
          else if (!config[i].is3wire && config[i].pullup)  // rheostat, output is in 00.00k to 130.1k Ohms
            printfResistance(tmp);
          else if (!config[i].is3wire && !config[i].pullup)  // control voltage, output is 0.00V to 5.00V Volts
            printfControlVoltage(tmp);
          else
            printf("      ");
        }
        else
          printf("      ");
        printf("%c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      // intermediate scaled
      printf("Cur (scaled)  ");
      for (i = 0; i < 8; i++)
      {
        if (greenNotRed(config[i].hwId != 15 && status[i].initialized))
        {
          double tmp = intermediate[i] / scale[i];
          if (!(status[i].isAutoRanged))
            setColor(DEFAULT);
          if (config[i].is3wire)  // pot, output is in %
            printfPercent(tmp);
          else if (!config[i].is3wire && config[i].pullup)  // rheostat, output is in 00.00k to 130.1k Ohms
            printfResistance(tmp);
          else if (!config[i].is3wire && !config[i].pullup)  // control voltage, output is 0.00V to 5.00V Volts
            printfControlVoltage(tmp);
          else
            printf("      ");
        }
        else
          printf("      ");
        printf("%c", i == 7 ? '\n' : ' ');
      }
      setColor(DEFAULT);

      lastMessage = cmd << 16;
      return 1;

    // --------------------- send data -----------------------------
    // settings
    case PLAYCONTROLLER_BB_MSG_TYPE_SETTING:
      dump(cmd, len, data, flags);
      if (len != 2)
      {
        printf("SETTING : wrong length of %d\n", len);
        return 3;
      }
      displayCounter();
      printf("SETTING(ID=%04X): ", data[0]);
      switch (data[0])
      {
        case PLAYCONTROLLER_SETTING_ID_PLAY_MODE_UPPER_RIBBON_BEHAVIOUR:  // = 0,       // ==> BIT 0 set if (returnMode == RETURN), ...
        case PLAYCONTROLLER_SETTING_ID_PLAY_MODE_LOWER_RIBBON_BEHAVIOUR:  // = 1,       // ... BIT 1 set if (touchBehaviour == RELATIVE)
          printf("%s ribbon play mode=", data[0] == PLAYCONTROLLER_SETTING_ID_PLAY_MODE_UPPER_RIBBON_BEHAVIOUR ? "upper" : "lower");
          printf("%s,%s\n", data[1] & 1 ? "return" : "non-return", data[1] & 2 ? "relative" : "absolute");
          break;
        case PLAYCONTROLLER_SETTING_ID_BASE_UNIT_UI_MODE:  //= 3,       // ==> PLAY = 0, PARAMETER_EDIT = 1
          printf("base unit UI mode=%s\n", data[1] ? "edit" : "play");
          break;
        case PLAYCONTROLLER_SETTING_ID_EDIT_MODE_RIBBON_BEHAVIOUR:  // = 4,       // ==> RELATIVE = 0, ABSOLUTE = 1
          printf("edit-mode ribbon behavior=%s\n", data[1] ? "absolute" : "relative");
          break;
        case PLAYCONTROLLER_SETTING_ID_RIBBON_REL_FACTOR:  // = 9,       // ==> tTcdRange(256, 2560)
          printf("ribbon relative factor=%5.2lf\n", (double) (data[1]) / 256.0);
          break;
        case PLAYCONTROLLER_SETTING_ID_VELOCITY_CURVE:  // = 11,      // ==> VERY_SOFT = 0, SOFT = 1, NORMAL = 2, HARD = 3, VERY_HARD = 4
          printf("key velocity curve=%u\n", data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_PEDAL_1_TYPE:  //                     = 26,      // ==> PotTipActive  = 0
        case PLAYCONTROLLER_SETTING_ID_PEDAL_2_TYPE:  //                     = 27,      // ... PotRingActive = 1
        case PLAYCONTROLLER_SETTING_ID_PEDAL_3_TYPE:  //                     = 28,      // ... SwitchClosing = 2 // aka momentary switch, normally open
        case PLAYCONTROLLER_SETTING_ID_PEDAL_4_TYPE:  //                     = 29,      // ... SwitchOpening = 3 // aka momentary switch, normally closed
          printf("legacy pedal type, pedal#%u=%u\n", data[0] - PLAYCONTROLLER_SETTING_ID_PEDAL_1_TYPE + 1, data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_AFTERTOUCH_CURVE:  //                 = 30,      // SOFT = 0, NORMAL = 1, HARD = 2
          printf("aftertouch curve=%u\n", data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_BENDER_CURVE:  //                     = 31,      // SOFT = 0, NORMAL = 1, HARD = 2
          printf("bender curve=%u\n", data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE:  //               = 36,      // set initial output value (for relative mode)
        case PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_VALUE:  //               = 37,      // set initial output value (for relative mode)
          printf("%s ribbon value=%u\n", data[0] == PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE ? "upper" : "lower", data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_SOFTWARE_MUTE_OVERRIDE:  //           = 0xFF01,  // direction: input; arguments(uint16): 1, mode bit pattern
          printf("software mute override=%04X\n", data[1]);
          break;
        case PLAYCONTROLLER_SETTING_ID_SEND_RAW_SENSOR_DATA:  //             = 0xFF02,  // direction: input; arguments(uint16): 1, flag (!= 0)
          printf("raw sensor data=%s\n", data[1] ? "on" : "off");
          break;
        case PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_LOGGING:  //               = 0xFF03,  // direction: input; arguments(uint16): 1, flag (!= 0)
          printf("key logging=%s\n", data[1] ? "on" : "off");
          break;
        case PLAYCONTROLLER_SETTING_ID_ENABLE_EHC:  //                       = 0xFF04,  // direction: input; arguments(uint16): 1, flag (!= 0)
          printf("ehc processing=%s\n", data[1] ? "enabled" : "disabled");
          break;
        case PLAYCONTROLLER_SETTING_ID_AUDIO_ENGINE_CMD:  //                 = 0xFF05,  // direction: input; arguments(uint16): 1, command (1:testtone OFF; 2:testtone ON; 3:default sound)
          printf("Audio Engine special command=");
          switch (data[1])
          {
            case 1:
              printf("test-tone OFF\n");
              break;
            case 2:
              printf("test-tone ON\n");
              break;
            case 3:
              printf("init default sound\n");
              break;
            default:
              printf("unknown(%u)\n", data[1]);
              break;
          }
          break;
        case PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_MAPPING:  //               = 0xFF07,  // direction: input; arguments(uint16): 1, flag (!= 0)
          printf("key re-mapping=%s\n", data[1] ? "enabled" : "disabled");
          break;
        case PLAYCONTROLLER_SETTING_ID_SYSTEM_SPECIAL:  //                   = 0xFF06,  // direction: input; arguments(uint16): 1, command (1:reset heartbeat: 2: system reset: 3:Enable MIDI)
          printf("System special command=");
          switch (data[1])
          {
            case 1:
              printf("reset heartbeat\n");
              break;
            case 2:
              printf("system reset\n");
              break;
            default:
              printf("unknown(%u)\n", data[1]);
              break;
          }
          break;

        case PLAYCONTROLLER_SETTING_ID_NOTE_SHIFT:
        case PLAYCONTROLLER_SETTING_ID_PEDAL_1_MODE:
        case PLAYCONTROLLER_SETTING_ID_PEDAL_2_MODE:
        case PLAYCONTROLLER_SETTING_ID_PEDAL_3_MODE:
        case PLAYCONTROLLER_SETTING_ID_PEDAL_4_MODE:
        case PLAYCONTROLLER_SETTING_ID_TRANSITION_TIME:
        case PLAYCONTROLLER_SETTING_ID_PITCHBEND_ON_PRESSED_KEYS:
        case PLAYCONTROLLER_SETTING_ID_EDIT_SMOOTHING_TIME:
        case PLAYCONTROLLER_SETTING_ID_PRESET_GLITCH_SUPPRESSION:
        case PLAYCONTROLLER_SETTING_ID_BENDER_RAMP_BYPASS:
          printf("unused lecagy ID %u, data=0x%04X\n", data[0], data[1]);
          break;

        default:
          printf("unknown ID %u, data=0x%04X\n", data[0], data[1]);
          break;
      }
      return 1;

    // EHC config
    case PLAYCONTROLLER_BB_MSG_TYPE_EHC_CONFIG:
      dump(cmd, len, data, flags);
      if (len != 2)
      {
        printf("EHC COMMAND : wrong length of %d\n", len);
        return 3;
      }
      displayCounter();
      printf("EHC COMMAND: ");
      uint16_t ehcCmd = data[0] & 0xFF00;
      uint8_t  ctrlId = data[0] & 0x00FF;
      switch (ehcCmd)
      {
        case PLAYCONTROLLER_EHC_COMMAND_SET_CONTROL_REGISTER:
        {
          printf("config=0x%04X ", data[1]);
          EHC_ControllerConfig_T config = EHC_uint16ToConfig(data[1]);
          printf("ctrlId:%u", config.ctrlId);
          if (config.hwId == 15)
            printf(", OFF\n");
          else
          {
            printf(", hwId:%u", config.hwId);
            printf(", silent:%u", config.silent);
            printf(", is3wire:%u", config.is3wire);
            printf(", pullup:%u", config.pullup);
            printf(", cont.:%u", config.continuous);
            printf(", invert:%u", config.polarityInvert);
            printf(", hold:%u", config.autoHoldStrength);
            printf(", autoRange:%u\n", config.doAutoRanging);
          }
          break;
        }
        case PLAYCONTROLLER_EHC_COMMAND_SET_RANGE_MIN:
        case PLAYCONTROLLER_EHC_COMMAND_SET_RANGE_MAX:
          printf("range (ctrlId:%u) %s=%u\n", ctrlId, data[0] == PLAYCONTROLLER_EHC_COMMAND_SET_RANGE_MIN ? "min" : "max", data[1]);
          break;
        case PLAYCONTROLLER_EHC_COMMAND_RESET_DELETE:
          printf("%s ctrlId:%u\n", data[1] ? "delete" : "reset", ctrlId);
          break;
        case PLAYCONTROLLER_EHC_COMMAND_FORCE_OUTPUT:
          printf("force output (ctrlId:%u)\n", ctrlId);
          break;
        case PLAYCONTROLLER_EHC_COMMAND_SET_DEAD_ZONES:
          printf("dead zone (ctrlId:%u) upper=%u, lower=%u\n", ctrlId, data[1] >> 8, data[1] & 0xFF);
          break;
        default:
          printf("unknown EHC command 0x%04X, data=0x%04x\n", data[0], data[1]);
          break;
      }
      return 1;

    // Request
    case PLAYCONTROLLER_BB_MSG_TYPE_REQUEST:
      dump(cmd, len, data, flags);
      if (len != 1)
      {
        printf("REQUEST : wrong length of %d\n", len);
        return 3;
      }
      displayCounter();
      printf("REQUEST: ");
      switch (data[0])
      {
        case PLAYCONTROLLER_REQUEST_ID_SW_VERSION:
          printf("firmware version\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_UNMUTE_STATUS:
          printf("muting status\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_EHC_DATA:
          printf("EHC data\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_CLEAR_EEPROM:
          printf("clear EERPOM\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_STAT_DATA:
          printf("profiling data\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_UHID64:
          printf("unique hardware ID (64bit)\n");
          break;
        case PLAYCONTROLLER_REQUEST_ID_EHC_EEPROMSAVE:
          printf("save EHC data to EEPROM\n");
          break;
        default:
          printf("unknown(%u)\n", data[1]);
          break;
      }
      break;

    default:
      if (flags & NO_UNKNOWN)
        return 0;
      displayCounter();
      printf("UNKNOWN, ");
      flags &= ~NO_HEXDUMP;
      dump(cmd, len, data, flags);
      return 1;
  }
  return 0;
}
