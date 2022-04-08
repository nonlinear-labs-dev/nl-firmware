/*
 * nl_tcd_poly.h
 *
 *  Created on: 30.01.2015
 *      Author: ssc
 */
#pragma once

#include "stdint.h"

//======== defines

#define VEL_CURVE_VERY_SOFT 0
#define VEL_CURVE_SOFT      1
#define VEL_CURVE_NORMAL    2
#define VEL_CURVE_HARD      3
#define VEL_CURVE_VERY_HARD 4
#define VEL_CURVE_COUNT     5

//======== public functions
void POLY_Init(void);
void POLY_Process(void);

void POLY_EnableKeyMapping(uint16_t const on);
void POLY_SetKeyRemapTable(uint16_t const length, uint16_t const* data);

void POLY_SetATMasking(uint16_t const length, uint16_t const* const data);

void POLY_ForceKey(uint16_t const midiKeyNumber, uint16_t const timeLow, uint16_t const timeHigh);
void POLY_KeyLogging(uint16_t const on);
void POLY_Select_VelTable(uint32_t const curve);

typedef struct
{
  uint8_t pressed[256];
  uint8_t head;
  uint8_t tail;
} keyQueue_T;

extern keyQueue_T POLY_keyQueue;
extern uint64_t   POLY_pressedKeyBF;  // bit field for currently pressed keys
extern uint64_t   POLY_maskedKeyBF;   // bit field for masked keys

static inline int POLY_AnyKeysPressed(void)
{
  return POLY_pressedKeyBF != 0;
}

static inline int POLY_AnyUmmaskedKeysPressed(void)
{
  return (POLY_pressedKeyBF & ~POLY_maskedKeyBF) != 0;
}

static inline int POLY_AnyMaskedKeysPressed(void)
{
  return (POLY_pressedKeyBF & POLY_maskedKeyBF) != 0;
}

static inline int POLY_OnlyMaskedKeysPressed(void)
{
  return POLY_AnyKeysPressed() && !POLY_AnyUmmaskedKeysPressed();
}

static inline int8_t POLY_getNewestKey(void)
{
  if (POLY_keyQueue.head == POLY_keyQueue.tail)
    return -1;

  if (POLY_maskedKeyBF == 0)  // no masking --> use head item
    return POLY_keyQueue.pressed[POLY_keyQueue.head - 1];

  uint8_t index = POLY_keyQueue.head;
  do
  {
    index--;
    if ((((uint64_t) 1 << POLY_keyQueue.pressed[index]) & POLY_maskedKeyBF) == 0)  // key not masked ?
      return POLY_keyQueue.pressed[index];
  } while (index != POLY_keyQueue.tail);

  return -1;
}

static inline int8_t POLY_getSingleKey(void)
{
  if (POLY_keyQueue.head != POLY_keyQueue.tail + 1)
    return -1;
  else
    return POLY_keyQueue.pressed[POLY_keyQueue.tail];
}
