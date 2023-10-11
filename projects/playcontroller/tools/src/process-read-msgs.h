#pragma once

#include <stdio.h>
#include <stdint.h>

#define NO_HEARTBEAT     (0x00001)
#define NO_MUTESTATUS    (0x00002)
#define NO_PARAMS        (0x00004)
#define NO_NOTIFICATION  (0x00008)
#define NO_EHCDATA       (0x00010)
#define NO_SENSORSRAW    (0x00020)
#define NO_HEXDUMP       (0x00040)
#define NO_UNKNOWN       (0x00080)
#define NO_STATDATA      (0x00100)
#define NO_OVERLAY       (0x00200)
#define NO_KEY_LOG       (0x00400)
#define NO_RIBBONS       (0x00800)
#define NO_UHID          (0x01000)
#define NO_REDUCED       (0x02000)
#define NO_QUIT          (0x04000)
#define NO_AT_DATA       (0x08000)
#define NO_AT_DATA_OHMS  (0x10000)
#define NO_BENDER_ADJUST (0x20000)
#define NO_ALL           (0x3FFFF)

int processReadMsgs(uint16_t const cmd, uint16_t const len, uint16_t* const data, uint32_t flags);
