#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "shared/playcontroller/playcontroller-defs.h"
#include "shared/version.h"
#include "globals.h"

#include "linuxgpio.h"

#define VERSION_STRING "1.2"
#define PROGNAME       "playcontroller"

void printVersion(void)
{
  printf(PROGNAME " version " VERSION_STRING " (Build: %s, %s)\n", GetC15Version(), GetC15Build());
}

void Error(const char *msg)
{
  perror(msg);
  exit(3);
}

// ===================
void makeDriverNonblocking(int const driverFileNo, int const flags)
{
  if (fcntl(driverFileNo, F_SETFL, flags | O_NONBLOCK) < 0)
    Error("fcntl: can't switch to non-blocking");
}

void makeDriverBlocking(int const driverFileNo, int const flags)
{
  if (fcntl(driverFileNo, F_SETFL, flags & (~O_NONBLOCK)) < 0)
    Error("fcntl: can't switch to blocking");
}

int getDriverFlags(int const driverFileNo)
{
  int flags;
  if ((flags = fcntl(driverFileNo, F_GETFL, 0)) < 0)
    Error("fcntl: getflags failed");
  return flags;
}

// ===================
void writeData(int output, uint16_t const len, uint16_t *data)
{
  if (len == 0)
    return;
Retry:
  if (write(output, data, len) != len)
  {
    if (errno == EAGAIN || errno == EBUSY || errno == EFAULT)
    {
      usleep(1000);
      goto Retry;
    }

    if (errno == EMSGSIZE)
    {
      puts("Message too long, discarded");
      return;
    }

    perror("Error");
    exit(3);
  }
}
#define HELP    "--help"
#define VERSION "--version"

#define REQUEST         "req"
#define SW_VERSION      "sw-version"
#define MUTE_STATUS     "muting"
#define CLEAR_EEPROM    "clear-eeprom"
#define CLEAR_STAT      "clear-status"
#define STAT_DATA       "status"
#define EHC_SAVE_EEPROM "save-ehc"
#define UHID64          "uhid64"
#define POLLHWS         "pollhws"
#define AT_TEST_DATA    "at-test-data"
#define AT_STATUS       "at-status"
#if LPC_KEYBED_DIAG
#define KEY_CNTRS "key-counters"
#endif

#define SETTING             "set"
#define MUTE_CTRL           "mute-ctrl"
#define MUTE_CTRL_DISABLE   "disable"
#define MUTE_CTRL_MUTE      "mute"
#define MUTE_CTRL_UNMUTE    "unmute"
#define RAW_SENSORS         "sensors"
#define KEY_LOGGING         "key-logging"
#define HWS_TO_UI           "hws-to-ui"
#define ON                  "on"
#define OFF                 "off"
#define AE_CMD              "ae-cmd"
#define AE_CMD_TTON         "tton"
#define AE_CMD_TTOFF        "ttoff"
#define AE_CMD_DEFSND       "def-snd"
#define SSPECIAL            "system"
#define SSPECIAL_RESET      "reboot"
#define SSPECIAL_RESETHB    "hb-reset"
#define SSPECIAL_ENABLEMIDI "enable-midi"
#define AT_TEST_DATA        "at-test-data"
#define AT_LEGACY           "legacy-at"

#define KEY_EMUL "key"

#define TEST "test"

#define RESET "reset"

#define AT_MASK "at-mask"

uint16_t REQ_DATA[] = { PLAYCONTROLLER_BB_MSG_TYPE_REQUEST, 0x0001, 0x0000 };
uint16_t SET_DATA[] = { PLAYCONTROLLER_BB_MSG_TYPE_SETTING, 0x0002, 0x0000, 0x0000 };
uint16_t KEY_DATA[] = { PLAYCONTROLLER_BB_MSG_TYPE_KEY_EMUL, 0x0003, 0x0000, 0x0000, 0x0000 };
uint16_t ATM_DATA[] = { PLAYCONTROLLER_BB_MSG_TYPE_AT_MASKING, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000 };

// ===================
void Usage(void)
{
  printVersion();
  puts("Usage:");
  puts(" lpc --help           : display usage and exit");
  puts(" lpc --version        : print version and exit");
  puts(" lpc  <command>");
  puts("  <commands> : req|set|key|test");
#if LPC_KEYBED_DIAG
  puts("  req[uest] : sw-version|muting|clear-eeprom|status|clear-status|save-ehc|key-counters");
#else
  puts("  req[uest] : sw-version|muting|clear-eeprom|status|clear-status|save-ehc|pollhws|at-test-data|at-status");
#endif
  puts("     sw-version   : get LPC firware version");
  puts("     muting       : get software&hardware muting status");
  puts("     clear-eeprom : erase EEPROM");
  puts("     status       : get diagnostic status data");
  puts("     clear-status : clear diagnostic status data");
  puts("     save-ehc     : save current EHC config data to EEPROM");
  puts("     uhid64       : get unique hardware ID (64bit)");
  puts("     pollhws      : poll hardware sources");
  puts("     at-test-data : get aftertouch raw ADC (or Ohms) values for all keys");
  puts("     at-status    : get aftertouch status");
#if LPC_KEYBED_DIAG
  puts("     key-counters : get diagnostic key error counters");
#endif
  puts("  set[ting] : mute-ctrl|sensors|key-logging|hws-to-ui|ae-cmd|system");
  puts("     mute-ctrl: disable|mute|unmute : disable mute override or set/clear muting");
  puts("     sensors: on|off                : turn raw sensor messages on/off");
  puts("     key-logging: on|off            : turn key-logging messages on/off");
  puts("     hws-to-ui: on|off              : turn hardware-source to UI messages on/off");
  puts("     at-test-data: on|off           : turn collecting aftertouch test data on/off");
  puts("     legacy-at: on|off              : turn legacy aftertouch mode on/off");
  puts("     ae-cmd: tton|ttoff|def-snd     : Audio Engine Special, test-tone on/off, load default sound");
  puts("     system: reboot|hb-reset|enable-midi");
  puts("                  : System Special; reboot system, reset heartbeat counter, enable midi");
  puts("  key <note-nr> <time>      : send emulated key");
  puts("     <note-nr>              : MIDI key number, 60=\"C3\"");
  puts("     <time>                 : key time (~1/velocity) in us (1000...525000), negative means key release");
  puts("  at-mask <bitmask64> : mask keys for aftertouch focus tracking");
  puts("      <bitmask64>           : 16-digit hex value representing a 64bit bit mask, bit 0 is leftmost key");
  puts("                              Bit 63 causes AT also to be silent when only masked keys are pressed");
  puts("  test <size> <count> <delay>   : send test message");
  puts("     <size>                     : payload size in words (1..1000)");
  puts("     <count>                    : # of times the message is send (1..65535)");
  puts("     <delay>                    : delay in usecs between messages");
  puts("  reset [<retries>]           : reset and check if online, with optional retries if offline");
  puts("                              : return value is 0 when reset was successful, 1 otherwise");
  exit(3);
}

int purgeBuffer(int driver)
{
  uint8_t byte;
  int     ret = 0;
  while (read(driver, &byte, 1) == 1)
    ret = 1;
  return ret;
}

#define LPC_RESET_PIN (50u)                      // port number of the RESET pin GPIO
#define LPC_ISP1_PIN  (48u)                      // port number of the first ISP pin GPIO
#define LPC_ISP2_PIN  (51u)                      // port number of the second (alternate) ISP pin GPIO
#define WAIT_TIME_US  (200000u)                  // 200ms wait time after any port access
#define POLL_DELAY_US (1000000u - WAIT_TIME_US)  // 1 second total delay until polling for LPC response

int lpcReset(int driver)
{
  int fd;

  linuxgpio_export(LPC_RESET_PIN);
  linuxgpio_export(LPC_ISP1_PIN);
  linuxgpio_export(LPC_ISP2_PIN);
  linuxgpio_dir_out(LPC_RESET_PIN);
  linuxgpio_dir_out(LPC_ISP1_PIN);
  linuxgpio_dir_out(LPC_ISP2_PIN);

  fd = linuxgpio_openfd(LPC_ISP1_PIN);
  write(fd, "1", 1);  // pull ISP1 line high
  usleep(WAIT_TIME_US);
  close(fd);

  fd = linuxgpio_openfd(LPC_ISP2_PIN);
  write(fd, "1", 1);  // pull ISP2 line high
  usleep(WAIT_TIME_US);
  close(fd);

  fd = linuxgpio_openfd(LPC_RESET_PIN);
  write(fd, "0", 1);  // pull RESET line low --> reset
  purgeBuffer(driver);
  usleep(WAIT_TIME_US);
  write(fd, "1", 1);
  usleep(WAIT_TIME_US);
  close(fd);

  linuxgpio_dir_in(LPC_RESET_PIN);
  linuxgpio_unexport(LPC_RESET_PIN);

  usleep(POLL_DELAY_US);
  if (purgeBuffer(driver))
    return 1;
  return 0;
}

#define DRIVER "/dev/lpc_bb_driver"
// ===================
int main(int argc, char const *argv[])
{
  if (argc == 1)
    Usage();

  if (strncmp(argv[1], HELP, sizeof HELP) == 0)
    Usage();

  if (strncmp(argv[1], VERSION, sizeof VERSION) == 0)
  {
    printVersion();
    return 0;
  }

  int driver = open("/dev/lpc_bb_driver", O_WRONLY);
  if (driver < 0)
    Error("cannot open driver for write");

  int driverRd = open("/dev/lpc_bb_driver", O_RDONLY);
  if (driver < 0)
    Error("cannot open driver for read");

  int flags = getDriverFlags(driverRd);
  makeDriverNonblocking(driverRd, flags);

  // request
  if (strncmp(argv[1], REQUEST, sizeof REQUEST) == 0)
  {
    if (argc != 3)
    {
      puts("req: too few arguments!");
      Usage();
    }
    if (strncmp(argv[2], SW_VERSION, sizeof SW_VERSION) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_SW_VERSION;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], MUTE_STATUS, sizeof MUTE_STATUS) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_UNMUTE_STATUS;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], CLEAR_EEPROM, sizeof CLEAR_EEPROM) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_CLEAR_EEPROM;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], STAT_DATA, sizeof STAT_DATA) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_STAT_DATA;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], CLEAR_STAT, sizeof CLEAR_STAT) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_CLEAR_STAT;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], EHC_SAVE_EEPROM, sizeof EHC_SAVE_EEPROM) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_EHC_EEPROMSAVE;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], UHID64, sizeof UHID64) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_UHID64;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], POLLHWS, sizeof POLLHWS) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_POLLHWS;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], AT_TEST_DATA, sizeof AT_TEST_DATA) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_AT_TEST_DATA;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
    if (strncmp(argv[2], AT_STATUS, sizeof AT_STATUS) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_AT_STATUS;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
#if LPC_KEYBED_DIAG
    if (strncmp(argv[2], KEY_CNTRS, sizeof KEY_CNTRS) == 0)
    {
      REQ_DATA[2] = PLAYCONTROLLER_REQUEST_ID_KEYCNTR_DATA;
      writeData(driver, sizeof REQ_DATA, &REQ_DATA[0]);
      return 0;
    }
#endif
    puts("req: unknown request");
    Usage();
  }

  // setting
  if (strncmp(argv[1], SETTING, sizeof SETTING) == 0)
  {
    if (argc != 4)
    {
      puts("set: too few arguments (need 2)!");
      Usage();
    }
    // mute control
    if (strncmp(argv[2], MUTE_CTRL, sizeof MUTE_CTRL) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_SOFTWARE_MUTE_OVERRIDE;
      if (strncmp(argv[3], MUTE_CTRL_DISABLE, sizeof MUTE_CTRL_DISABLE) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], MUTE_CTRL_MUTE, sizeof MUTE_CTRL_MUTE) == 0)
      {
        SET_DATA[3] = SUP_UNMUTE_STATUS_SOFTWARE_OVERRIDE;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], MUTE_CTRL_UNMUTE, sizeof MUTE_CTRL_UNMUTE) == 0)
      {
        SET_DATA[3] = SUP_UNMUTE_STATUS_SOFTWARE_OVERRIDE | SUP_UNMUTE_STATUS_SOFTWARE_VALUE;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set mute-ctrl : illegal parameter");
      Usage();
    }

    // sensors
    if (strncmp(argv[2], RAW_SENSORS, sizeof RAW_SENSORS) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_SEND_RAW_SENSOR_DATA;
      if (strncmp(argv[3], OFF, sizeof OFF) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], ON, sizeof ON) == 0)
      {
        SET_DATA[3] = 1;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set sensors : illegal parameter");
      Usage();
    }

    // key-logging
    if (strncmp(argv[2], KEY_LOGGING, sizeof KEY_LOGGING) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_LOGGING;
      if (strncmp(argv[3], OFF, sizeof OFF) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], ON, sizeof ON) == 0)
      {
        SET_DATA[3] = 1;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set key-logging : illegal parameter");
      Usage();
    }

    // hardware-source to UI
    if (strncmp(argv[2], HWS_TO_UI, sizeof HWS_TO_UI) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_ENABLE_UI_PARAMETER_MSGS;
      if (strncmp(argv[3], OFF, sizeof OFF) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], ON, sizeof ON) == 0)
      {
        SET_DATA[3] = 1;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set hws-to-ui : illegal parameter");
      Usage();
    }

    // AT test data
    if (strncmp(argv[2], AT_TEST_DATA, sizeof AT_TEST_DATA) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_ENABLE_AT_DATA_COLLECT;
      if (strncmp(argv[3], OFF, sizeof OFF) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], ON, sizeof ON) == 0)
      {
        SET_DATA[3] = 1;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set at-test-data : illegal parameter");
      Usage();
    }

    // legacy AT
    if (strncmp(argv[2], AT_LEGACY, sizeof AT_LEGACY) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_ENABLE_LEGACY_AT;
      if (strncmp(argv[3], OFF, sizeof OFF) == 0)
      {
        SET_DATA[3] = 0;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], ON, sizeof ON) == 0)
      {
        SET_DATA[3] = 1;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set at-test-data : illegal parameter");
      Usage();
    }

    // audio engine command
    if (strncmp(argv[2], AE_CMD, sizeof AE_CMD) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_AUDIO_ENGINE_CMD;
      if (strncmp(argv[3], AE_CMD_TTOFF, sizeof AE_CMD_TTOFF) == 0)
      {
        SET_DATA[3] = AE_PROTOCOL_CMD_TONE_OFF;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], AE_CMD_TTON, sizeof AE_CMD_TTON) == 0)
      {
        SET_DATA[3] = AE_PROTOCOL_CMD_TONE_ON;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], AE_CMD_DEFSND, sizeof AE_CMD_DEFSND) == 0)
      {
        SET_DATA[3] = AE_PROTOCOL_CMD_DEFAULT_SOUND;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set ae-cmd : illegal parameter");
      Usage();
    }

    // system special
    if (strncmp(argv[2], SSPECIAL, sizeof SSPECIAL) == 0)
    {
      SET_DATA[2] = PLAYCONTROLLER_SETTING_ID_SYSTEM_SPECIAL;
      if (strncmp(argv[3], SSPECIAL_RESET, sizeof SSPECIAL_RESET) == 0)
      {
        SET_DATA[3] = SYS_SPECIAL_RESET_SYSTEM;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], SSPECIAL_RESETHB, sizeof SSPECIAL_RESETHB) == 0)
      {
        SET_DATA[3] = SYS_SPECIAL_RESET_HEARTBEAT;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      if (strncmp(argv[3], SSPECIAL_ENABLEMIDI, sizeof SSPECIAL_ENABLEMIDI) == 0)
      {
        SET_DATA[3] = SYS_SPECIAL_ENABLE_MIDI;
        writeData(driver, sizeof SET_DATA, &SET_DATA[0]);
        return 0;
      }
      puts("set system : illegal parameter");
      Usage();
    }
    puts("set: unknown parameter!");
    Usage();
  }

  // key emulation
  if (strncmp(argv[1], KEY_EMUL, sizeof KEY_EMUL) == 0)
  {
    if (argc != 4)
    {
      puts("set: too few arguments!");
      Usage();
    }
    if (sscanf(argv[2], "%hu", &KEY_DATA[2]) != 1)
    {
      puts("key: key number argument error (uint16 expected)");
      Usage();
    }
    int time;
    if (sscanf(argv[3], "%d", &time) != 1)
    {
      puts("key: key time argument error (int32 expected)");
      Usage();
    }
    KEY_DATA[3] = time & 0xFFFF;
    KEY_DATA[4] = (time >> 16) & 0xFFFF;
    writeData(driver, sizeof KEY_DATA, &KEY_DATA[0]);
    return 0;
  }

  // test data
  if (strncmp(argv[1], TEST, sizeof TEST) == 0)
  {
    if (argc != 5)
    {
      puts("test: wrong number of arguments");
      Usage();
    }

    uint16_t len, count;
    uint32_t delay;
    if (sscanf(argv[2], "%hu", &len) != 1)
    {
      puts("test: size argument error (uint16 expected)");
      Usage();
    }
    if (len < 1 || len > 1000)
    {
      puts("test: size must be 1..1000");
      Usage();
    }
    if (sscanf(argv[3], "%hu", &count) != 1)
    {
      puts("test: count argument error (uint16 expected)");
      Usage();
    }
    if (count < 1)
    {
      puts("test: count must be 1..65535");
      Usage();
    }
    if (sscanf(argv[4], "%u", &delay) != 1)
    {
      puts("test: delay argument error (uint32 expected)");
      Usage();
    }

    uint16_t testDataBuffer[len + 2];
    testDataBuffer[0] = PLAYCONTROLLER_BB_MSG_TYPE_TEST_MSG;
    testDataBuffer[1] = len;
    testDataBuffer[2] = 0;
    if (len > 1)
      for (int i = 0; i < len - 1; i++)
        testDataBuffer[i + 3] = i;

    while (count--)
    {
      writeData(driver, (len + 2) * sizeof(uint16_t), &testDataBuffer[0]);
      testDataBuffer[2]++;
      if (delay)
        usleep(delay);
    }
    return 0;
  }

  // reset and check
  if (strncmp(argv[1], RESET, sizeof RESET) == 0)
  {
    if (argc != 2 && argc != 3)
    {
      puts("reset: wrong number of arguments");
      Usage();
    }

    uint16_t retries = 1;
    if (argc == 3)
    {
      if (sscanf(argv[2], "%hu", &retries) != 1)
      {
        puts("reset: retries argument error (uint16 expected)");
        Usage();
      }
      if (retries < 1)
      {
        puts("reset: retries must be >= 1");
        Usage();
      }
    }

    uint16_t savedRetries = retries;
    while (retries--)
    {
      if (lpcReset(driverRd))
      {
        printf("LPC reset successful after %u retries\n", savedRetries - retries);
        return 0;
      }
    }
    puts("LPC appears to be down (Note: bbbb.service must be stopped for proper response)");
    return 1;
  }

  // at-mask
  if (strncmp(argv[1], AT_MASK, sizeof AT_MASK) == 0)
  {
    if (argc != 3)
    {
      puts("at-mask: wrong number of arguments");
      Usage();
    }

    uint64_t mask = 0;
    if (sscanf(argv[2], "%" SCNx64, &mask) != 1)
    {
      puts("at-mask: mask argument error (hex uint64 expected)");
      Usage();
    }
    uint64_t *p = (uint64_t *) (&ATM_DATA[2]);
    *p          = mask;
    writeData(driver, sizeof ATM_DATA, &ATM_DATA[0]);
    return 0;
  }

  // unknown
  puts("unknown command");
  Usage();
  return 0;
}
