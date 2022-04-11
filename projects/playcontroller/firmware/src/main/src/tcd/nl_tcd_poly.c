/*
 * nl_tcd_poly.c
 *
 *  Created on: 27.01.2015
 *      Author: ssc
 *	Last Changed : 2020-01-10 KSTR (implement 14bit velocities)
 */

#include "math.h"
#include "nl_tcd_poly.h"
#include "nl_tcd_adc_work.h"
#include "nl_tcd_msg.h"
#include "ipc/emphase_ipc.h"
#include "drv/nl_cgu.h"
#include "spibb/nl_bb_msg.h"
#include "playcontroller/playcontroller-defs.h"

#define KEY_MIN_TIME (2000)  // in us, shorter attack/release times than this will be clipped

//  key queue, needed to find the newest of all currently pressed keys

// ---------------------
/*

	--- Finding the newest of all currently pressed keys ---

	Maintain a queue of currently pressed keys, identified by their number (0...60, thus a uint8_t):
	| key-X | key-Y | key-Z | free
	    2       1       0     <--- queue index
	    ^                       ^
	queue tail              queue head

	A newly pressed key-T is attached to the head:
	| key-X | key-Y | key-Z | key-T | free
	    3       2       1       0     <--- queue index
	    ^                               ^
	queue tail                      queue head
	Queue head is incremented by one and the new key is placed there


	Handling released key:
	When a key is released, locate in the queue and delete it.
	If the position is the queue head, simple decrease the head.
	If the position is the queue tail, simply increase the tail.
	If the position is in-between, move the shorter section remaining,
	either from the head or from the tail

	1. Delete key-Y:
	| key-X || key-Z | key-T |
	    2        1       0     <--- queue index
	key-X must move on position to the right and the queue tail must be incremented by one.

	2. Delete key-Z:
	| key-X | key-Y || key-T |
	    2       1        0     <--- queue index
	key-T must move on position to the left and the queue head must be decremented by one.

	In Add and Remove operations, the queue is never extended to the left and thus will
	slowly rotate through the allocated memory.

	--- Obtaining the newest of all currently pressed keys ---
	>> The key at the queue head is the newest active key <<
	If the queue is empty (head == tail) then there is no key at all

	-----------

	The queue can never hold any more than 61 keys so it can actually be placed
	in a ring buffer of size 256 (for easy index modulo management).

	Because of the small data size memory searches and moves look like being tolerable.

*/

keyQueue_T POLY_keyQueue;
uint64_t   POLY_pressedKeyBF = 0;
uint64_t   POLY_maskedKeyBF  = 0;
int        POLY_silent       = 0;

void POLY_SetATMasking(uint16_t const length, uint16_t const *const data)
{
  if (length != 4)
    return;

  POLY_maskedKeyBF = *((uint64_t *) data);
  POLY_silent      = (POLY_maskedKeyBF & ((uint64_t) 1 << 63)) != 0;
  POLY_maskedKeyBF &= ~((uint64_t) 1 << 63);
}

static inline void addKeyToQueue(uint8_t const key)
{
  POLY_keyQueue.pressed[POLY_keyQueue.head++] = key;
  POLY_pressedKeyBF |= (uint64_t) 1 << key;
}

static inline void removeKeyFromQueue(uint8_t const key)
{
  POLY_pressedKeyBF &= ~((uint64_t) 1 << key);

  if (POLY_keyQueue.head == POLY_keyQueue.tail)
    return;

  uint8_t index = POLY_keyQueue.head;
  do
  {
    index--;
    if (POLY_keyQueue.pressed[index] == key)
    {
      if (index == POLY_keyQueue.tail)
      {  // key found at tail
        POLY_keyQueue.tail++;
        return;
      }
      if (index == POLY_keyQueue.head - 1)
      {  // key found at head
        POLY_keyQueue.head--;
        return;
      }
      // key is in between
      if ((POLY_keyQueue.head - 1 - index) <= (index - POLY_keyQueue.tail))
      {  // move head end down
        do
        {
          POLY_keyQueue.pressed[index] = POLY_keyQueue.pressed[index + 1];
          index++;
        } while (index != POLY_keyQueue.head - 1);
        POLY_keyQueue.head--;
      }
      else
      {  // move tail end up
        do
        {
          POLY_keyQueue.pressed[index] = POLY_keyQueue.pressed[index - 1];
          index--;
        } while (index != POLY_keyQueue.tail);
        POLY_keyQueue.tail++;
      }
      return;
    }
  } while (index != POLY_keyQueue.tail);
}

/*
void printQueue(void)
{
  printf("Queue ");
  uint8_t index = POLY_keyQueue.tail;
  printf("(tail = %d, head = %d) ", index, POLY_keyQueue.head);
  while (index != POLY_keyQueue.head)
  {
    printf("%d ", POLY_keyQueue.pressed[index]);
    index++;
  }
  printf("\n");

}
*/

//------- module local variables

static uint32_t keyEvent[32];  // array for new events read from the ring buffer for keybed events

static uint32_t allVelTables[VEL_CURVE_COUNT][65] = {};  // converts time difference (timeInUs) to velocities
                                                         // element  0: shortest timeInUs   (2500 us or lower) -> 16383 = max. velocity
                                                         // element 64: longest timeInUs (526788 us or higher) -> 0     = min. velocity
static uint32_t *velTable = allVelTables[VEL_CURVE_NORMAL];

enum KeyLog_T
{
  LOG_OFF = 0,
  LOG_ON,
};

static enum KeyLog_T logKeys = LOG_OFF;

#define KEYMAP_SIZE (62)  // physical size is 61 but we need an even amount for the 2 byte transfer
static uint16_t remapKeys = 0;
static int8_t   keyRemapTable[KEYMAP_SIZE];  // table, mapping physical key numbers to physical key numbers

// -------- key logging
void POLY_KeyLogging(uint16_t const on)
{
  logKeys = (on != 0) ? LOG_ON : LOG_OFF;
}

// -------- key re-mapping
void POLY_EnableKeyMapping(uint16_t const on)
{
  remapKeys = (on != 0);
}

void POLY_SetKeyRemapTable(uint16_t const length, uint16_t const *data)
{
  if (length != KEYMAP_SIZE / 2)
    return;
  for (uint8_t i = 0; i < KEYMAP_SIZE / 2; i += 2, data++)
  {
    keyRemapTable[i]     = *data & 0x00FF;
    keyRemapTable[i + 1] = *data >> 8;
  }
}

/*******************************************************************************
@brief  	Generate_VelTable - generates the elements of velTable[]
@param[in]	curve - selects one of the five verlocity curves
			table index: 0 ... 64 (shortest ... longest key-switch time)
			table values: 16383 ... 0
*******************************************************************************/
static void Generate_VelTable(uint32_t const curve)
{
  if (curve >= VEL_CURVE_COUNT)
    return;

  float_t vel_max = 16383.0;  // the hyperbola goes from vel_max (at 0) to 0 (at i_max)
  float_t b       = 0.5;

  switch (curve)  // b defines the curve shape
  {
    case VEL_CURVE_VERY_SOFT:
      b = 0.125;
      break;
    case VEL_CURVE_SOFT:
      b = 0.25;
      break;
    case VEL_CURVE_NORMAL:
      b = 0.5;
      break;
    case VEL_CURVE_HARD:
      b = 1.0;
      break;
    case VEL_CURVE_VERY_HARD:
      b = 2.0;
      break;
    default:
      /// Error
      break;
  }

  uint32_t i_max = 64;

  uint32_t i;

  for (i = 0; i <= i_max; i++)
  {
    allVelTables[curve][i] = (uint32_t)((vel_max + vel_max / (b * i_max)) / (1.0 + b * i) - vel_max / (b * i_max) + 0.5);
  }
}

//================= Initialisation:
void POLY_Init(void)
{
  for (int i = 0; i < VEL_CURVE_COUNT; i++)
    Generate_VelTable(i);
  velTable = allVelTables[VEL_CURVE_NORMAL];

  // init a "reverse" keymap, in case key mapping is activated without sending a table first
  for (int8_t i = 0; i < KEYMAP_SIZE; i++)
    keyRemapTable[i] = 64 - i;  // center is physical key #32

  POLY_keyQueue.head = POLY_keyQueue.tail = 0;
  POLY_pressedKeyBF                       = 0;
  POLY_maskedKeyBF                        = 0;
  POLY_silent                             = 0;
}

/******************************************************************************
	@brief		Select a Velocity table
*******************************************************************************/
void POLY_Select_VelTable(uint32_t const curve)
{
  if (curve >= VEL_CURVE_COUNT)
    return;
  velTable = allVelTables[curve];
}

/******************************************************************************
	@brief		ProcessKeyEvent : convert key event to MIDI event.
	            Also emits the key message back to BBB, and log the full
	            key event if requested
*******************************************************************************/
static void ProcessKeyEvent(uint32_t const keyEvent, enum KeyLog_T const logFlag)
{
  uint8_t physicalKey = (keyEvent & IPC_KEYBUFFER_KEYMASK);
  if (physicalKey > 60)  // > beyond keybed size of 61 keys
    return;

  // TODO : "AE currently doesn't support remappings outside a logical note range of 36...96"
  uint32_t logicalKey;
  if (remapKeys)
    logicalKey = 36 + keyRemapTable[physicalKey];  // table might produce "negative" key base values
  else
    logicalKey = 36 + physicalKey;

  MSG_KeyPosition(logicalKey);

  uint32_t time = M0_PERIOD_62_5NS * (keyEvent >> IPC_KEYBUFFER_TIME_SHIFT);  // us

  //--- Calc On velocity
  uint32_t vel;

  if (time <= KEY_MIN_TIME)  // clipping the low end: zero at a times <= 2.5 ms
    vel = velTable[0];
  else if (time >= ((1 << 19) + KEY_MIN_TIME))  // clipping at a maximum of 524 ms (2^19 us)
    vel = velTable[64];
  else
  {
    // first adjust the input to zero for a time of 2500us,
    // then lower 13 bits (0...8191) used for interpolation, upper 6 bits (0...63) used as index in the table
    uint32_t fract = (time - KEY_MIN_TIME) & 0x1FFF;
    uint32_t index = (time - KEY_MIN_TIME) >> 13;
    vel            = (velTable[index] * (8192 - fract) + velTable[index + 1] * fract) >> 13;  // ((0...16393) * 8192) / 8192
  }

  uint8_t keyPressed = (uint8_t) keyEvent & IPC_KEYBUFFER_NOTEON;
  if (keyPressed)
  {
    MSG_KeyDown(vel);
    addKeyToQueue(physicalKey);
    ADC_WORK_WriteHWValueForUI(HW_SOURCE_ID_LAST_KEY, (physicalKey << 8) | logicalKey);
  }
  else
  {
    MSG_KeyUp(vel);
    time = -time;  // negate time for key logging
    removeKeyFromQueue(physicalKey);
  }

  if (logFlag)
  {
    uint16_t buffer[3];
    buffer[0] = physicalKey;
    buffer[1] = time & 0xFFFF;
    buffer[2] = time >> 16;
    BB_MSG_WriteMessage(PLAYCONTROLLER_BB_MSG_TYPE_KEY_EMUL, 3, buffer);
  }
}

void ForceKey(uint16_t const hardwareKeyNumber, int time)
{
  uint32_t forcedKeyEvent = hardwareKeyNumber;
  if (time < 0)
    time = -time;
  else
    forcedKeyEvent |= IPC_KEYBUFFER_NOTEON;
  forcedKeyEvent |= ((time / M0_PERIOD_62_5NS) + 1) << IPC_KEYBUFFER_TIME_SHIFT;
  ProcessKeyEvent(forcedKeyEvent, LOG_OFF);  // emulated key, never log it
}

/******************************************************************************
	@brief		POLY_ForceKey : emulate a key down/up event via software
	            Special Function: If all 3 input parameters == 0, generate a
	            burst sequence of all 61 keys, switching note on/off selection
	            with every call (as it turned out sending note off really
	            immediately after note on seems to give no sound at all from
	            AE (which seems somewhat reasonable)
*******************************************************************************/
void POLY_ForceKey(uint16_t const hardwareKeyNumber, uint16_t const timeLow, uint16_t const timeHigh)
{
  if (hardwareKeyNumber > 64)
    return;
  if (hardwareKeyNumber != 0 || timeLow != 0 || timeHigh != 0)
  {
    ForceKey(hardwareKeyNumber, (int) (((uint32_t) timeHigh << 16) + (uint32_t) timeLow));
    return;
  }

  // generate key bursts
  static int time = -1;
  if (time == -1)
    time = 10000;  // 10ms, moderate press, to avoid shocking volume/distortion
  else
    time = -1;  // fast release
  for (uint16_t key = 0; key <= 60; key++)
    ForceKey(key, time);
}

/******************************************************************************
	@brief		POLY_Process: reading new key events from the ring buffer
*******************************************************************************/
void POLY_Process(void)
{
  uint32_t i;
  uint32_t numKeyEvents = Emphase_IPC_M4_KeyBuffer_ReadBuffer(keyEvent, 32);  // reads the latest key up/down events from M0 ring buffer

  for (i = 0; i < numKeyEvents; i++)
    ProcessKeyEvent(keyEvent[i], logKeys);  // hardware key event, so send key log if requested
  MSG_Process();                            // note this call also takes care of sending other pending MIDI data HW sources
}
