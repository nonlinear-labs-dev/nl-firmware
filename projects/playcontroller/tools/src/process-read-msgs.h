#pragma once

#include <stdio.h>
#include <stdint.h>

#define NO_HEARTBEAT    (0x0001)
#define NO_MUTESTATUS   (0x0002)
#define NO_PARAMS       (0x0004)
#define NO_NOTIFICATION (0x0008)
#define NO_EHCDATA      (0x0010)
#define NO_SENSORSRAW   (0x0020)
#define NO_HEXDUMP      (0x0040)
#define NO_UNKNOWN      (0x0080)
#define NO_STATDATA     (0x0100)
#define NO_OVERLAY      (0x0200)
#define NO_KEY_LOG      (0x0400)
#define NO_RIBBONS      (0x0800)
#define NO_UHID         (0x1000)
#define NO_REDUCED      (0x2000)
#define NO_QUIT         (0x4000)
#define NO_AT_DATA      (0x8000)
#define NO_ALL          (0xFFFF)

int processReadMsgs(uint16_t const cmd, uint16_t const len, uint16_t* const data, uint16_t flags);
