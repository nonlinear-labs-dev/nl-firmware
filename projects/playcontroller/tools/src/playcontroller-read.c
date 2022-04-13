#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include "shared/version.h"

#define VERSION_STRING "1.1"
#define PROGNAME       "lpc-read"

void printVersion(void)
{
  printf(PROGNAME " version " VERSION_STRING " (Build: %s, %s)\n", GetC15Version(), GetC15Build());
}

#define HELP    "--help"
#define VERSION "--version"

#include "process-read-msgs.h"

#define MAX_DATA_SIZE (1500)
uint16_t id;
uint16_t len;
uint16_t data[MAX_DATA_SIZE];
int      dataIndex;
uint32_t displayFlags;
FILE *   driver;

void Error(const char *msg)
{
  printf("\nError : %s \nAborting.\n", msg);
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
int readWord(uint16_t *const data)
{
  static int bl;
  static int step = 0;
  int        bh;
  int        ret = 0;

  switch (step)
  {
    case 0:
      errno = 0;
      bl    = fgetc(driver);
      if (bl == EOF)
      {
        exit(0);
        //        if (errno && errno != EAGAIN)
        //          Error(strerror(errno));
      }
      else
        step++;
      break;
    case 1:
      errno = 0;
      bh    = fgetc(driver);
      if (bh == EOF)
      {
        exit(0);
        //        if (errno && errno != EAGAIN)
        //          Error(strerror(errno));
      }
      else
      {
        *data = ((bh & 0xFF) << 8) | (bl & 0xFF);
        step  = 0;
        ret   = 1;
      }
      break;
  }
  return ret;
}

// ===================
int readMessages(void)
{
  static int step  = 0;
  static int count = 0;
  uint16_t   word;

  int ret = 0;

  switch (step)
  {
    case 0:  // read ID
      if (readWord(&id))
      {
        dataIndex = 0;
        step      = 1;
      }
      break;
    case 1:
      if (readWord(&len))
      {
        count = len;
        if (len)
          step = 2;
        else
          step = 3;
      }
      break;
    case 2:
      if (count)
      {
        if (readWord(&word))
        {
          count--;
          data[dataIndex] = word;
          if (++dataIndex >= MAX_DATA_SIZE)
            Error("LPC Message too long to fit in buffer");
        }
      }
      else
        step = 3;
      break;
    case 3:
    {
      int didPrint = processReadMsgs(id, len, &data[0], displayFlags);
      if (didPrint && ((displayFlags & NO_QUIT) == 0))
        ret = 1;
      step = 0;
      break;
    }
  }
  return ret;
}

void writeMessages(void)
{
}

// ===================
void Usage(char const *const string, int const exitCode)
{
  printVersion();
  if (string)
    puts(string);
  puts("lpc-read --help           : display usage and exit");
  puts("lpc-read --version        : print version and exit");
  puts("lpc-read [@filename] <options>");
  puts("  @filename : specify input file rather than using /dev/lpc_bb_driver");
  puts("  <options> is a white-space seperated list of letters, preceeded");
  puts("            by either a + or -, turning the display on or off");
  puts("  default is +a -d -i -q -o");
  puts(" a   All options");
  puts(" c   diagnostiC status data");
  puts(" d   additional hex Dump, forces -r");
  puts(" e   EHC data");
  puts(" h   Heartbeat");
  puts(" i   raw rIbbon values (use \"LPC set sensors on\")");
  puts(" k   Key logging and key counters");
  puts(" m   Mute status");
  puts(" n   Notificiation");
  puts(" o   Overlay messages of same type");
  puts(" p   Parameter");
  puts(" q   print single message, then Quit");
  puts(" r   Reduced output (entity value only, or shorted list)");
  puts(" s   Sensors raw data");
  puts(" to  afterTouch min. Ohms value data");
  puts(" ta  afterTouch max. ADC value data");
  puts(" u   hexdump of Unknown messages");
  puts(" 6   64bit unique hardware ID");
  exit(exitCode);
}

// ===================
int main(int argc, char *argv[])
{
  int   driverFileNo;
  int   flags;
  char  drpath[] = "/dev/lpc_bb_driver";
  char *path     = drpath;

  if (argc > 1 && argv[1][0] == '@')
  {  // cmdline override of input
    path = &argv[1][1];
    argc--;
    argv++;
  }

  if (argc > 1)
  {
    if (strncmp(argv[1], HELP, sizeof HELP) == 0)
      Usage(NULL, 0);

    if (strncmp(argv[1], VERSION, sizeof VERSION) == 0)
    {
      printVersion();
      return 0;
    }
  }

  driver = fopen(path, "r+");
  if (!driver)
    Error("cannot open driver or input file");

  driverFileNo = fileno(driver);
  if (driverFileNo == -1)
    Error("cannot get fileo of driver");

  flags = getDriverFlags(driverFileNo);
  makeDriverBlocking(driverFileNo, flags);

  displayFlags = NO_HEXDUMP + NO_RIBBONS + NO_QUIT + NO_REDUCED;

  while (argc > 1)
  {
    if (strncmp(argv[1], "-a", 2) == 0)
      displayFlags |= NO_ALL;
    else if (strncmp(argv[1], "+a", 2) == 0)
      displayFlags &= ~NO_ALL;

    else if (strncmp(argv[1], "-o", 2) == 0)
      displayFlags |= NO_OVERLAY;
    else if (strncmp(argv[1], "+o", 2) == 0)
      displayFlags &= ~NO_OVERLAY;

    else if (strncmp(argv[1], "-c", 2) == 0)
      displayFlags |= NO_STATDATA;
    else if (strncmp(argv[1], "+c", 2) == 0)
      displayFlags &= ~NO_STATDATA;

    else if (strncmp(argv[1], "-d", 2) == 0)
      displayFlags |= NO_HEXDUMP;
    else if (strncmp(argv[1], "+d", 2) == 0)
      displayFlags &= ~NO_HEXDUMP;

    else if (strncmp(argv[1], "-e", 2) == 0)
      displayFlags |= NO_EHCDATA;
    else if (strncmp(argv[1], "+e", 2) == 0)
      displayFlags &= ~NO_EHCDATA;

    else if (strncmp(argv[1], "-h", 2) == 0)
      displayFlags |= NO_HEARTBEAT;
    else if (strncmp(argv[1], "+h", 2) == 0)
      displayFlags &= ~NO_HEARTBEAT;

    else if (strncmp(argv[1], "-i", 2) == 0)
      displayFlags |= NO_RIBBONS;
    else if (strncmp(argv[1], "+i", 2) == 0)
      displayFlags &= ~NO_RIBBONS;

    else if (strncmp(argv[1], "-k", 2) == 0)
      displayFlags |= NO_KEY_LOG;
    else if (strncmp(argv[1], "+k", 2) == 0)
      displayFlags &= ~NO_KEY_LOG;

    else if (strncmp(argv[1], "-m", 2) == 0)
      displayFlags |= NO_MUTESTATUS;
    else if (strncmp(argv[1], "+m", 2) == 0)
      displayFlags &= ~NO_MUTESTATUS;

    else if (strncmp(argv[1], "-n", 2) == 0)
      displayFlags |= NO_NOTIFICATION;
    else if (strncmp(argv[1], "+n", 2) == 0)
      displayFlags &= ~NO_NOTIFICATION;

    else if (strncmp(argv[1], "-r", 2) == 0)
      displayFlags |= NO_REDUCED;
    else if (strncmp(argv[1], "+r", 2) == 0)
      displayFlags &= ~NO_REDUCED;

    else if (strncmp(argv[1], "-p", 2) == 0)
      displayFlags |= NO_PARAMS;
    else if (strncmp(argv[1], "+p", 2) == 0)
      displayFlags &= ~NO_PARAMS;

    else if (strncmp(argv[1], "-q", 2) == 0)
      displayFlags |= NO_QUIT;
    else if (strncmp(argv[1], "+q", 2) == 0)
      displayFlags &= ~NO_QUIT;

    else if (strncmp(argv[1], "-s", 2) == 0)
      displayFlags |= NO_SENSORSRAW;
    else if (strncmp(argv[1], "+s", 2) == 0)
      displayFlags &= ~NO_SENSORSRAW;

    else if (strncmp(argv[1], "-to", 3) == 0)
      displayFlags |= NO_AT_DATA_OHMS;
    else if (strncmp(argv[1], "+to", 3) == 0)
      displayFlags &= ~NO_AT_DATA_OHMS, displayFlags |= NO_AT_DATA;

    else if (strncmp(argv[1], "-ta", 3) == 0)
      displayFlags |= NO_AT_DATA;
    else if (strncmp(argv[1], "+ta", 3) == 0)
      displayFlags &= ~NO_AT_DATA, displayFlags |= NO_AT_DATA_OHMS;

    else if (strncmp(argv[1], "-u", 2) == 0)
      displayFlags |= NO_UNKNOWN;
    else if (strncmp(argv[1], "+u", 2) == 0)
      displayFlags &= ~NO_UNKNOWN;

    else if (strncmp(argv[1], "-6", 2) == 0)
      displayFlags |= NO_UHID;
    else if (strncmp(argv[1], "+6", 2) == 0)
      displayFlags &= ~NO_UHID;

    else
      Usage("unknown option", 3);
    argc--;
    argv++;
  }

  if (!(displayFlags & NO_HEXDUMP))
    displayFlags |= NO_OVERLAY;

  if (displayFlags & NO_REDUCED)
    printf("\nOutput from '%s' :\n", path);

#if 0
  printf("flags: ");  
  printf("%s ", (displayFlags & NO_HEARTBEAT) ? "-h" : "+h");
  printf("%s ", (displayFlags & NO_MUTESTATUS) ? "-m" : "+m");
  printf("%s ", (displayFlags & NO_PARAMS) ? "-p" : "+p");
  printf("%s ", (displayFlags & NO_NOTIFICATION) ? "-n" : "+n");
  printf("%s ", (displayFlags & NO_EHCDATA) ? "-e" : "+e");
  printf("%s ", (displayFlags & NO_SENSORSRAW) ? "-s" : "+s");
  printf("%s ", (displayFlags & NO_HEXDUMP) ? "-d" : "+d");
  printf("%s ", (displayFlags & NO_UNKNOWN) ? "-u" : "+u");
  printf("%s ", (displayFlags & NO_STATDATA) ? "-c" : "+c");
  printf("%s ", (displayFlags & NO_OVERLAY) ? "-r" : "+r");
  printf("%s ", (displayFlags & NO_KEY_LOG) ? "-k" : "+k");
  printf("%s ", (displayFlags & NO_RIBBONS) ? "-i" : "+i");
  printf("%s ", (displayFlags & NO_UHID) ? "-6" : "+6");
  printf("%s ", (displayFlags & NO_REDUCED) ? "-o" : "+o");
  printf("%s ", (displayFlags & NO_QUIT) ? "-q" : "+q");
  printf("%s ", (displayFlags & NO_AT_DATA) ? "-ta" : "+ta");
  printf("%s ", (displayFlags & NO_AT_DATA_OHMS) ? "-to" : "+to");
  printf("\n");
#endif

  while (1)
  {
    int quit = readMessages();
    fflush(stdout);
    writeMessages();
    if (quit)
      break;
  }
  return 0;
}
