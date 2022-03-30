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
void POLY_SetKeyRemapTable(uint16_t const length, uint16_t *data);

void POLY_ForceKey(uint16_t const midiKeyNumber, uint16_t const timeLow, uint16_t const timeHigh);
void POLY_KeyLogging(uint16_t const on);
int  POLY_GetHighestKey(void);
int  POLY_GetSingleKey(void);
void POLY_Select_VelTable(uint32_t const curve);

// ---------------------
/*

	--- Finding the newest of all currently pressed keys ---

	Maintain a queue of currently pressed keys, identified by their number (0...60, thus a uint8_t):
	| key-X | key-Y | key-Z |
	    2       1       0     <--- queue index
	    ^               ^
	queue tail      queue head

	A newly pressed key-T is attached to the head:
	| key-X | key-Y | key-Z | key-T |
	    3       2       1       0     <--- queue index
	Queue head is incremented by one and the new key is placed there

	When key-Y is released, locate in the queue and delete it:
	| key-X | key-Z | key-T |
	    2       1       0     <--- queue index
	key-X and key-Y must move on position to the right and the queue tail must be incremented by one.

	>> The key at the queue head is the newest active key <<
	If the queue is empty (head == tail) then there is no key at all

	-----------

	The queue can never hold any more than 61 keys so it can actually be placed
	in a ring buffer of size 64 (for easy index management).

	Because of the small data size memory searches and moves look like being tolerable.

*/

extern uint8_t  POLY_keyQueue[64];
extern unsigned POLY_queueHead;
extern unsigned POLY_queueTail;

static inline int POLY_removeKey(uint16_t const keyNum)
{
  if (POLY_queueHead == POLY_queueTail)
    return 0;

  unsigned index = POLY_queueHead;
  do
  {
    index = (index + 63) % 64;
    if (POLY_keyQueue[index] == keyNum)
    {  // key found
      // copy remaining keys up one position
      while (index != POLY_queueTail)
      {
        POLY_keyQueue[index] = POLY_keyQueue[(index + 63) % 64];
        index                = (index + 63) % 64;
      }
      // adjust queue tail
      POLY_queueTail = (POLY_queueTail + 1) % 64;
      return 1;
    }
  } while (index != POLY_queueTail);
  return 0;
}

static inline void POLY_addKey(uint8_t const keyNum)
{
  while (POLY_removeKey(keyNum))
    ;
  POLY_keyQueue[POLY_queueHead] = keyNum;
  POLY_queueHead                = (POLY_queueHead + 1) % 64;
}

static inline int POLY_getNewestKey(void)  // -1 when no key
{
  if (POLY_queueHead == POLY_queueTail)
    return -1;
  return POLY_keyQueue[(POLY_queueHead + 63) % 64];
}
