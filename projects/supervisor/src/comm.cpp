﻿// BBB communication
// KSTR 2019-11-04

#include "comm.h"

#include "bit_manipulation.h"
#include "pin_manipulation.h"
#include "hardware.h"
#include "version.h"
#include <avr/interrupt.h>

static uint8_t enable = 0;

#define BAUD (1200ul)
#define UBRR (F_OSC / (16ul * BAUD) - 1)

static uint8_t data[3];

// async uart, 8 data bits, 2 stop bits, odd parity
void COMM_Init(void)
{
  data[0] = data[1] = data[2] = 0;
  cli();
#if IS_TYPE_XX4_CHIP == 0
  UBRRH = (uint8_t)(UBRR >> 8);
  UBRRL = (uint8_t)(UBRR);
  UCSRB = (1 << RXEN) | (1 << TXEN);
  UCSRC = (1 << URSEL) | (0 << UMSEL) | (3 << UPM0) | (1 << USBS) | (3 << UCSZ0);
#else
  UBRR0H = (uint8_t)(UBRR >> 8);
  UBRR0L = (uint8_t)(UBRR);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (0b00 << UMSEL00) | (3 << UPM00) | (1 << USBS0) | (0b11 << UCSZ00);
#endif
  sei();
  enable = 1;
}

void COMM_DeInit(void)
{
  enable = 0;
#if IS_TYPE_XX4_CHIP == 0
  UCSRB = 0;
#else
  UCSR0B = 0;
#endif
}

static uint8_t step = 0;

void COMM_StartStatusWrite(void)
{
  if (step == 0)
    step = 1;
}

void COMM_ProccessReadCommands(void)
{
  if (!enable)
    return;

#if IS_TYPE_XX4_CHIP == 0
  if (BitGet(UCSRA, RXC))  // receive register full ?
  {
    uint8_t ok = !BitGet(UCSRA, FE) && !BitGet(UCSRA, DOR) && !BitGet(UCSRA, UPE);
    data[2]    = data[1];
    data[1]    = data[0];
    data[0]    = UDR;
#else
  if (BitGet(UCSR0A, RXC0))  // receive register full ?
  {
    uint8_t ok = !BitGet(UCSR0A, FE0) && !BitGet(UCSR0A, DOR0) && !BitGet(UCSR0A, UPE0);
    data[2]    = data[1];
    data[1]    = data[0];
    data[0]    = UDR0;
#endif
    if (!ok)
    {
      data[0] = data[1] = data[2] = 0;
    }
    else if (data[0] == 'c' && data[2] == 'c')
      switch (data[1])
      {
        case 'L':
          if (config.GetRunState() == RS_RUNNING || config.GetRunState() == RS_BOOTING)
            BitSet(config.status, STAT_SYSTEM_LOCKED);
          break;
        case 'l':
          BitClr(config.status, STAT_SYSTEM_LOCKED);
          break;

        case 'O':
          BitSet(config.status, STAT_MUTING_OVERRIDE_ENABLE);
          break;
        case 'o':
          BitClr(config.status, STAT_MUTING_OVERRIDE_ENABLE);
          break;

        case 'U':
          BitSet(config.status, STAT_MUTING_OVERRIDE_VALUE);
          break;
        case 'u':
          BitClr(config.status, STAT_MUTING_OVERRIDE_VALUE);
          break;
      }
  }
}

void COMM_ProccessWriteStatus(void)
{
  if (!enable)
    return;
#if IS_TYPE_XX4_CHIP == 0
  if (BitGet(UCSRA, UDRE))  // send register empty ?
  {
    switch (step)
    {
      case 0:
        break;  // wait for someone start the chain
      case 1:
        UDR = 0x80 + config.status;
        step++;
        break;
      case 2:
        UDR = config.hardware_id;
        step++;
        break;
      case 3:
        UDR  = config.firmware_id;
        step = 0;
        break;
    }
  }
#else
  if (BitGet(UCSR0A, UDRE0))  // send register empty ?
  {
    switch (step)
    {
      case 0:
        break;  // wait for someone start the chain
      case 1:
        UDR0 = 0x80 + config.status;
        step++;
        break;
      case 2:
        UDR0 = config.hardware_id;
        step++;
        break;
      case 3:
        UDR0 = config.firmware_id;
        step = 0;
        break;
    }
  }
#endif
}

// end of file
