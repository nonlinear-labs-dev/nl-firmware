#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "shared/playcontroller/playcontroller-defs.h"
#include "shared/playcontroller/playcontroller-data.h"
#include "shared/playcontroller/lpc_lib.h"

void Usage(void)
{
  printf("Usage: make-cal-bin <calibration-file> <binary-output-file>\n");
  printf("    <calibration-file>   : input, text-file with links to ribbon and aftertouch calibration data files\n");
  printf("    <binary-output-file> : output, binary file for sending cal data to LPC directly via /dev/lpc_bb_driver\n");
}

#define RIBBON1_STR    "Ribbon1="
#define RIBBON2_STR    "Ribbon2="
#define AFTERTOUCH_STR "Aftertouch="

typedef struct
{
  int      valid;
  uint16_t X[34];
  uint16_t Y[33];
} RibbonData;

static RibbonData ribbon[2];

static AT_calibration_T    ATData        = { 0 };
static LIB_interpol_data_T AT_forceToAdc = { 16, AT_adcToForceTableY, AT_adcToForceTableX };

int main(int const argc, char const* const argv[])
{
  SIZECHECK(AT_calibration_T, 32 * sizeof(uint32_t));

  ribbon[0].valid = 0;
  ribbon[1].valid = 0;

  if (argc != 3)
  {
    Usage();
    return 3;
  }

  FILE* infile;
  if (!(infile = fopen(argv[1], "r")))
  {
    printf("FATAL: Cannot open input file \"%s\"\n", argv[1]);
    return 3;  // --> exit
  }
  FILE* outfile;
  char* outFname = argv[2];
  if (!(outfile = fopen(outFname, "wb")))
  {
    printf("FATAL: Cannot create output file \"%s\"\n", argv[2]);
    return 3;  // --> exit
  }

  char buffer[1024];
  typedef enum
  {
    NONE       = -1,
    RIBBON1    = 0,
    RIBBON2    = 1,
    AFTERTOUCH = 2,
  } Action;

  while (!feof(infile))
  {
    char* buf = buffer;
    if (!fgets(buf, 1024, infile))
      break;
    Action action = NONE;
    if (strncmp(RIBBON1_STR, buf, sizeof(RIBBON1_STR) - 1) == 0)
      action = RIBBON1;
    else if (strncmp(RIBBON2_STR, buf, sizeof(RIBBON2_STR) - 1) == 0)
      action = RIBBON2;
    else if (strncmp(AFTERTOUCH_STR, buf, sizeof(AFTERTOUCH_STR) - 1) == 0)
      action = AFTERTOUCH;

    switch (action)
    {
      case NONE:
        break;

      case AFTERTOUCH:
      {
        buf += sizeof(AFTERTOUCH_STR) - 1;
        for (int i = 0; buf[i] != 0; i++)
          if (buf[i] == '\n' || buf[i] == '\r')
          {
            buf[i] = 0;
            break;
          }
        FILE* atfile;
        if (!(atfile = fopen(buf, "r")))
        {
          printf("FATAL: Cannot open aftertouch file \"%s\"\n", buf);
          fclose(outfile);
          remove(outFname);
          return 3;  // --> exit
        }
        {
          char  buffer[1024];
          char* buf = buffer;
          while (!feof(atfile))
          {
            buf = buffer;
            if (!fgets(buf, 1024, atfile))
              break;
            if (strncmp("[Header]", buf, 8) == 0)
            {
              fgets(buf, 1024, atfile);
              for (int k = 0; buf[k] != 0; k++)
              {
                if (buf[k] == ',' || buf[k] == '\n')
                {
                  buf[k] = ' ';
                }
              }

              if (sscanf(buf, "%hu", &(ATData.keybedId)) != 1)
              {
                printf("FATAL: Cannot read keybed id\n");
                fclose(outfile);
                remove(outFname);
                return 3;  // --> exit
              }
              if (ATData.keybedId == 0)
              {
                printf("FATAL: keybed id must not be zero\n");
                fclose(outfile);
                remove(outFname);
                return 3;  // --> exit
              }

              while (*buf != ' ' && *buf != 0)
                buf++;
              if (*buf == ' ')
                buf++;

              if (sscanf(buf, "%hu", &(ATData.adcTarget)) != 1)
              {
                printf("FATAL: Cannot read aftertouch test force\n");
                fclose(outfile);
                remove(outFname);
                return 3;  // --> exit
              }

              if (ATData.adcTarget < 8000 || ATData.adcTarget > 20000)
              {
                printf("FATAL: aftertouch test force must be within 8000 and 20000 [mN]\n");
                fclose(outfile);
                remove(outFname);
                return 3;  // --> exit
              }
              ATData.adcTarget = LIB_InterpolateValue(&AT_forceToAdc, ATData.adcTarget);
            }
            else if (strncmp("[Data]", buf, 6) == 0)
            {
              fgets(buf, 1024, atfile);
              for (int k = 0; buf[k] != 0; k++)
              {
                if (buf[k] == ',' || buf[k] == '\n')
                {
                  buf[k] = ' ';
                }
              }

              for (int i = 0; i < 61; i++)
              {
                if (sscanf(buf, "%hu", &(ATData.adcValues[i])) != 1)
                {
                  printf("FATAL: Cannot read 61 aftertouch per key values\n");
                  fclose(outfile);
                  remove(outFname);
                  return 3;  // --> exit
                }
                while (*buf != ' ' && *buf != 0)
                  buf++;
                if (*buf == ' ')
                  buf++;
              }
            }
          }
        }
        fclose(atfile);
        break;
      }

      case RIBBON1:
      case RIBBON2:
      {
        int select = (int) (action) - (int) (RIBBON1);
        buf += sizeof(RIBBON1_STR) - 1;
        for (int i = 0; buf[i] != 0; i++)
          if (buf[i] == '\n' || buf[i] == '\r')
          {
            buf[i] = 0;
            break;
          }
        FILE* ribfile;
        if (!(ribfile = fopen(buf, "r")))
        {
          printf("FATAL: Cannot open ribbon file \"%s\"\n", buf);
          return 3;  // --> exit
        }

        {
          char  buffer[1024];
          char* buf = buffer;
          while (!feof(ribfile))
          {
            buf = buffer;
            if (!fgets(buf, 1024, ribfile))
              break;
            if (strncmp("[Calibration-X]", buf, 15) == 0)
            {
              fgets(buf, 1024, ribfile);
              for (int k = 0; buf[k] != 0; k++)
              {
                if (buf[k] == ',' || buf[k] == '\n')
                {
                  buf[k] = ' ';
                }
              }

              for (int i = 0; i < 34; i++)
              {
                if (sscanf(buf, "%hu", &(ribbon[select].X[i])) != 1)
                {
                  printf("FATAL: Cannot read 34 X values ribbon file\n");
                  return 3;  // --> exit
                }
#warning : ToDo check monotony of X values
                while (*buf != ' ' && *buf != 0)
                  buf++;
                if (*buf == ' ')
                  buf++;
              }

              ribbon[select].valid++;
            }
            else if (strncmp("[Calibration-Y]", buf, 15) == 0)
            {
              fgets(buf, 1024, ribfile);
              for (int k = 0; buf[k] != 0; k++)
              {
                if (buf[k] == ',' || buf[k] == '\n')
                {
                  buf[k] = ' ';
                }
              }

              for (int i = 0; i < 33; i++)
              {
                if (sscanf(buf, "%hu", &(ribbon[select].Y[i])) != 1)
                {
                  printf("FATAL: Cannot read 33 Y values ribbon file\n");
                  return 3;  // --> exit
                }
#warning : ToDo check monotony of Y values
                while (*buf != ' ' && *buf != 0)
                  buf++;
                if (*buf == ' ')
                  buf++;
              }

              ribbon[select].valid++;
            }
          }
        }
        fclose(ribfile);
      }
      break;
    }
  }
  fclose(infile);

  if (ribbon[0].valid < 2 || ribbon[1].valid < 2)
  {
    printf("WARNING: failed to find/analyse two complete ribbon calibration sets\n");
  }
  else
  {
    printf("Ribbon calibration data found...\n");
    // write ribbon cal
    uint16_t rbcal_header[] = { PLAYCONTROLLER_BB_MSG_TYPE_RIBBON_CAL, 2 * 33 + 2 * 34 };

    if (fwrite(rbcal_header, 2, 2, outfile) != 2)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
    if (fwrite(ribbon[0].X, 2, 34, outfile) != 34)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
    if (fwrite(ribbon[0].Y, 2, 33, outfile) != 33)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
    if (fwrite(ribbon[1].X, 2, 34, outfile) != 34)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
    if (fwrite(ribbon[1].Y, 2, 33, outfile) != 33)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
  }

  if (ATData.keybedId == 0 || ATData.adcValues[0] == 0)
  {
    printf("WARNING: failed to find/analyse aftertouch calibration data\n");
  }
  else
  {
    printf("Aftertouch calibration data found...\n");
    // generate average for AT default
    uint32_t sum = 0;
    for (int i = 0; i < 61; i++)
      sum += ATData.adcValues[i];
    ATData.adcDefault = sum / 61;

    // write AT cal
    uint16_t atcal_header[] = {
      PLAYCONTROLLER_BB_MSG_TYPE_AT_CAL, 64
    };

    if (fwrite(atcal_header, 2, 2, outfile) != 2)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }

    if (fwrite(&ATData, 64 * 2, 1, outfile) != 1)
    {
      printf("FATAL: Error writing binary file\n");
      fclose(outfile);
      remove(outFname);
      return 3;  // --> exit
    }
  }

  fclose(outfile);

  printf("Done (binary calibration file created).\n");
  return 0;
}

// EOF
