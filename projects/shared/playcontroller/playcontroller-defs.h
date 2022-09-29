#pragma once

#include <stdint.h>

//
// ---------- ENUMS ----------------
//

enum PLAYCONTROLLER_BB_MESSAGE_TYPES
{
  PLAYCONTROLLER_BB_MSG_TYPE_PRESET_DIRECT = 0x0100,  // not used, direction: input; arguments(uint16): N, Nx data
  PLAYCONTROLLER_BB_MSG_TYPE_MORPH_SET_A   = 0x0200,  // not used
  PLAYCONTROLLER_BB_MSG_TYPE_MORPH_SET_B   = 0x0300,  // not used
  PLAYCONTROLLER_BB_MSG_TYPE_PARAMETER     = 0x0400,  // direction: output; arguments(uint16): 2, 1x parameter ID , 1x data
  PLAYCONTROLLER_BB_MSG_TYPE_EDIT_CONTROL  = 0x0500,  //
  PLAYCONTROLLER_BB_MSG_TYPE_MORPH_POS     = 0x0600,  // not used
  PLAYCONTROLLER_BB_MSG_TYPE_SETTING       = 0x0700,  // direction: input;  arguments (uint16): 2, 1x SETTING_ID_*, 1x data
  PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION  = 0x0800,  // direction: output; arguments (uint16): 2, 1x type, 1x value
  PLAYCONTROLLER_BB_MSG_TYPE_ASSERTION     = 0x0900,  // direction: output; arguments (uint16): n (string)
  PLAYCONTROLLER_BB_MSG_TYPE_REQUEST       = 0x0A00,  // direction: input;  argument  (uint16): 1, 1x REQUEST_ID_*
  PLAYCONTROLLER_BB_MSG_TYPE_HEARTBEAT     = 0x0B00,  // direction: output; arguments (uint16): 4, 4x uint16 (==uint64)
  PLAYCONTROLLER_BB_MSG_TYPE_MUTESTATUS    = 0x0C00,  // direction: output; argument  (uint16): 1, 1x bit pattern
  PLAYCONTROLLER_BB_MSG_TYPE_RIBBON_CAL    = 0x0D00,  // direction: input;  arguments (uint16): 134, 134x data [2x (33x 34x)]
  PLAYCONTROLLER_BB_MSG_TYPE_SENSORS_RAW   = 0x0E00,  // direction: output; arguments (uint16): 13, sensor raw data (see nl_tcd_adc_work.c)
  PLAYCONTROLLER_BB_MSG_TYPE_EHC_CONFIG    = 0x0F00,  // direction: input;  arguments (uint16): 2, 1x command, 1x data
  PLAYCONTROLLER_BB_MSG_TYPE_EHC_DATA      = 0x1000,  // direction: output; arguments (uint16): n, (see nl_ehc_ctrl.c)
  PLAYCONTROLLER_BB_MSG_TYPE_KEY_EMUL      = 0x1100,  // direction: input/output;  arguments (uint16): 3, midi key , time(lo), time(high)
  PLAYCONTROLLER_BB_MSG_TYPE_STAT_DATA     = 0x1200,  // direction: output; arguments (uint16): 4
  PLAYCONTROLLER_BB_MSG_TYPE_KEYMAP_DATA   = 0x1300,  // direction: input; arguments  (uint16): 32 (for 61 keys)
  PLAYCONTROLLER_BB_MSG_TYPE_KEYCNTR_DATA  = 0x1400,  // direction: output; arguments  (uint16): 64+128 key counters (64 low-level, 128 midi-level)
  PLAYCONTROLLER_BB_MSG_TYPE_UHID64        = 0x1500,  // direction: output; arguments  (uint16): 4 words, comprising an uint64_t;
  PLAYCONTROLLER_BB_MSG_TYPE_AT_TEST_DATA  = 0x1600,  // direction: output; arguments  (uint16): 61 uint16, aftertouch test values (raw ADC) for all 61 keys
  PLAYCONTROLLER_BB_MSG_TYPE_AT_MASKING    = 0x1700,  // direction: input;  arguments  (uint16): 1 uint64 (bit 0..60 = key left to right, bit 64 = enable "silent" mode)
  PLAYCONTROLLER_BB_MSG_TYPE_AT_CAL        = 0x1800,  // direction: input;  arguments  (uint16): 64 (see typedef AT_calibration_T)
  PLAYCONTROLLER_BB_MSG_TYPE_TEST_MSG      = 0xFFFF,  // direction in/out
};

enum PLAYCONTROLLER_SETTING_IDS
{
  PLAYCONTROLLER_SETTING_ID_PLAY_MODE_UPPER_RIBBON_BEHAVIOUR = 0,       // ==> BIT 0 set if (returnMode == RETURN), ...
  PLAYCONTROLLER_SETTING_ID_PLAY_MODE_LOWER_RIBBON_BEHAVIOUR = 1,       // ... BIT 1 set if (touchBehaviour == RELATIVE)
  PLAYCONTROLLER_SETTING_ID_NOTE_SHIFT                       = 2,       // not used, ==> tTcdRange (-48, 48)
  PLAYCONTROLLER_SETTING_ID_BASE_UNIT_UI_MODE                = 3,       // ==> PLAY = 0, PARAMETER_EDIT = 1
  PLAYCONTROLLER_SETTING_ID_EDIT_MODE_RIBBON_BEHAVIOUR       = 4,       // ==> RELATIVE = 0, ABSOLUTE = 1
  PLAYCONTROLLER_SETTING_ID_PEDAL_1_MODE                     = 5,       // not used, ==> STAY = 0
  PLAYCONTROLLER_SETTING_ID_PEDAL_2_MODE                     = 6,       // not used, ... RETURN_TO_ZERO = 1
  PLAYCONTROLLER_SETTING_ID_PEDAL_3_MODE                     = 7,       // not used, ... RETURN_TO_CENTER = 2,
  PLAYCONTROLLER_SETTING_ID_PEDAL_4_MODE                     = 8,       // not used,
  PLAYCONTROLLER_SETTING_ID_RIBBON_REL_FACTOR                = 9,       // ==> tTcdRange(256, 2560), formerly "upper ribbon factor"
  PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_REL_FACTOR          = 10,      // not used, ==> tTcdRange(256, 2560)
  PLAYCONTROLLER_SETTING_ID_VELOCITY_CURVE                   = 11,      // ==> VERY_SOFT = 0, SOFT = 1, NORMAL = 2, HARD = 3, VERY_HARD = 4
  PLAYCONTROLLER_SETTING_ID_TRANSITION_TIME                  = 12,      // not used, ==> tTcdRange(0, 16000)
  PLAYCONTROLLER_SETTING_ID_PEDAL_1_TYPE                     = 26,      // ==> PotTipActive  = 0
  PLAYCONTROLLER_SETTING_ID_PEDAL_2_TYPE                     = 27,      // ... PotRingActive = 1
  PLAYCONTROLLER_SETTING_ID_PEDAL_3_TYPE                     = 28,      // ... SwitchClosing = 2 // aka momentary switch, normally open
  PLAYCONTROLLER_SETTING_ID_PEDAL_4_TYPE                     = 29,      // ... SwitchOpening = 3 // aka momentary switch, normally closed
  PLAYCONTROLLER_SETTING_ID_AFTERTOUCH_CURVE                 = 30,      // SOFT = 0, NORMAL = 1, HARD = 2
  PLAYCONTROLLER_SETTING_ID_BENDER_CURVE                     = 31,      // SOFT = 0, NORMAL = 1, HARD = 2
  PLAYCONTROLLER_SETTING_ID_PITCHBEND_ON_PRESSED_KEYS        = 32,      // not used, OFF = 0, ON = 1
  PLAYCONTROLLER_SETTING_ID_EDIT_SMOOTHING_TIME              = 33,      // not used, ==> tTcdRange(0, 16000)
  PLAYCONTROLLER_SETTING_ID_PRESET_GLITCH_SUPPRESSION        = 34,      // not used, OFF = 0, ON = 1
  PLAYCONTROLLER_SETTING_ID_BENDER_RAMP_BYPASS               = 35,      // not used, OFF = 0, ON = 1
  PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE               = 36,      // set initial output value (for relative mode)
  PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_VALUE               = 37,      // set initial output value (for relative mode)
  PLAYCONTROLLER_SETTING_ID_SOFTWARE_MUTE_OVERRIDE           = 0xFF01,  // direction: input; arguments(uint16): 1, mode bit pattern
  PLAYCONTROLLER_SETTING_ID_SEND_RAW_SENSOR_DATA             = 0xFF02,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_LOGGING               = 0xFF03,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_ENABLE_EHC                       = 0xFF04,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_AUDIO_ENGINE_CMD                 = 0xFF05,  // direction: input; arguments(uint16): 1, command (1:testtone OFF; 2:testtone ON; 3:default sound)
  PLAYCONTROLLER_SETTING_ID_SYSTEM_SPECIAL                   = 0xFF06,  // direction: input; arguments(uint16): 1, command (1:reset heartbeat: 2: system reset: 3:Enable MIDI)
  PLAYCONTROLLER_SETTING_ID_ENABLE_KEY_MAPPING               = 0xFF07,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_ENABLE_UI_PARAMETER_MSGS         = 0xFF08,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_ENABLE_AT_DATA_COLLECT           = 0xFF09,  // direction: input; arguments(uint16): 1, flag (!= 0)
  PLAYCONTROLLER_SETTING_ID_ENABLE_LEGACY_AT                 = 0xFF0A,  // direction: input; arguments(uint16): 1, flag (!= 0) (boot default is "disabled")
  PLAYCONTROLLER_SETTING_ID_ENABLE_LEGACY_BNDR               = 0xFF0B,  // direction: input; arguments(uint16): 1, flag (!= 0) (boot default is "enabled")
  PLAYCONTROLLER_SETTING_ID_BNDR_SETTLING_SENSITIVITY        = 0xFF0C,  // direction: input; arguments(uint16): 1, 0= INSENSITIVE(default), >=1 = SENSITIVE
};

enum PLAYCONTROLLER_REQUEST_IDS
{
  PLAYCONTROLLER_REQUEST_ID_SW_VERSION     = 0x0000,
  PLAYCONTROLLER_REQUEST_ID_UNMUTE_STATUS  = 0x0001,
  PLAYCONTROLLER_REQUEST_ID_EHC_DATA       = 0x0002,
  PLAYCONTROLLER_REQUEST_ID_CLEAR_EEPROM   = 0x0003,
  PLAYCONTROLLER_REQUEST_ID_STAT_DATA      = 0x0004,
  PLAYCONTROLLER_REQUEST_ID_EHC_EEPROMSAVE = 0x0005,
  PLAYCONTROLLER_REQUEST_ID_KEYCNTR_DATA   = 0x0006,
  PLAYCONTROLLER_REQUEST_ID_CLEAR_STAT     = 0x0007,
  PLAYCONTROLLER_REQUEST_ID_UHID64         = 0x0008,
  PLAYCONTROLLER_REQUEST_ID_POLLHWS        = 0x0009,
  PLAYCONTROLLER_REQUEST_ID_AT_TEST_DATA   = 0x000A,
  PLAYCONTROLLER_REQUEST_ID_AT_STATUS      = 0x000B,
  PLAYCONTROLLER_REQUEST_ID_BNDR_STATUS    = 0x000C,
  PLAYCONTROLLER_REQUEST_ID_BNDR_RESET     = 0x000D,
};

enum PLAYCONTROLLER_NOTIFICATION_IDS
{
  PLAYCONTROLLER_NOTIFICATION_ID_SW_VERSION     = 0x0000,
  PLAYCONTROLLER_NOTIFICATION_ID_UNMUTE_STATUS  = 0x0001,
  PLAYCONTROLLER_NOTIFICATION_ID_EHC_DATA       = 0x0002,
  PLAYCONTROLLER_NOTIFICATION_ID_CLEAR_EEPROM   = 0x0003,
  PLAYCONTROLLER_NOTIFICATION_ID_STAT_DATA      = 0x0004,
  PLAYCONTROLLER_NOTIFICATION_ID_EHC_EEPROMSAVE = 0x0005,
  PLAYCONTROLLER_NOTIFICATION_ID_KEYCNTR_DATA   = 0x0006,
  PLAYCONTROLLER_NOTIFICATION_ID_CLEAR_STAT     = 0x0007,
  PLAYCONTROLLER_NOTIFICATION_ID_UHID64         = 0x0008,
  PLAYCONTROLLER_NOTIFICATION_ID_POLLHWS        = 0x0009,
  PLAYCONTROLLER_NOTIFICATION_ID_AT_TEST_DATA   = 0x000A,
  PLAYCONTROLLER_NOTIFICATION_ID_AT_STATUS      = 0x000B,
  PLAYCONTROLLER_NOTIFICATION_ID_BNDR_STATUS    = 0x000C,
  PLAYCONTROLLER_NOTIFICATION_ID_BNDR_RESET     = 0x000D,
  PLAYCONTROLLER_NOTIFICATION_ID_TEST_MSG       = 0xFFFF,
};

enum PLAYCONTROLLER_EHC_COMMAND_IDS
{
  PLAYCONTROLLER_EHC_COMMAND_SET_CONTROL_REGISTER = 0x0100,  // configure a controller
  PLAYCONTROLLER_EHC_COMMAND_SET_RANGE_MIN        = 0x0200,  // set lower end of ranging
  PLAYCONTROLLER_EHC_COMMAND_SET_RANGE_MAX        = 0x0300,  // set upper end of ranging
  PLAYCONTROLLER_EHC_COMMAND_RESET_DELETE         = 0x0400,  // reset or delete a controller
  PLAYCONTROLLER_EHC_COMMAND_FORCE_OUTPUT         = 0x0500,  // force a controller to send current value (if valid)
  PLAYCONTROLLER_EHC_COMMAND_SET_DEAD_ZONES       = 0x0600,  // set dead zones for auto-ranging
};

enum HW_SOURCE_IDS
{
  HW_SOURCE_ID_PEDAL_1    = 0,
  HW_SOURCE_ID_PEDAL_2    = 1,
  HW_SOURCE_ID_PEDAL_3    = 2,
  HW_SOURCE_ID_PEDAL_4    = 3,
  HW_SOURCE_ID_PITCHBEND  = 4,
  HW_SOURCE_ID_AFTERTOUCH = 5,
  HW_SOURCE_ID_RIBBON_1   = 6,
  HW_SOURCE_ID_RIBBON_2   = 7,
  HW_SOURCE_ID_PEDAL_5    = 8,
  HW_SOURCE_ID_PEDAL_6    = 9,
  HW_SOURCE_ID_PEDAL_7    = 10,
  HW_SOURCE_ID_PEDAL_8    = 11,
  // HW_SOURCE_ID_LAST_KEY is only used to signal last pressed key to BBB (not a real HW_SOURCE for AE)
  // high byte is the physical key number 0..60, low byte is logical note number (36..96, unless hardware-remapped)
  HW_SOURCE_ID_LAST_KEY = 12,
};
#define NUM_HW_REAL_SOURCES (HW_SOURCE_ID_PEDAL_8 + 1)  // all but LAST_KEY
#define NUM_HW_SOURCES      (HW_SOURCE_ID_LAST_KEY + 1)

enum AE_PROTOCOL_MSG_IDS
{
  AE_PROTOCOL_WRAPPER  = 0x0E,       // USB MIDI packet header "Cable #0, packet type:PitchBend"
  AE_PROTOCOL_HW_POS   = 0xE0,       // MIDI command "Pitch Bender" + MIDI channel=HW_SOURCE_ID
  AE_PROTOCOL_CMD      = 0xE0 + 12,  // MIDI command "Pitch Bender", MIDI channel 12
  AE_PROTOCOL_KEY_POS  = 0xE0 + 13,  // MIDI command "Pitch Bender", MIDI channel 13
  AE_PROTOCOL_KEY_DOWN = 0xE0 + 14,  // MIDI command "Pitch Bender", MIDI channel 14
  AE_PROTOCOL_KEY_UP   = 0xE0 + 15,  // MIDI command "Pitch Bender", MIDI channel 15
};

enum AE_PROTOCOL_CMD_IDS
{
  AE_PROTOCOL_CMD_TONE_OFF        = 1,  // turn off the test tone
  AE_PROTOCOL_CMD_TONE_ON         = 2,  // turn on the test tone (synth remains active)
  AE_PROTOCOL_CMD_DEFAULT_SOUND   = 3,  // set up a simple default sound that is guaranteed to give output
  AE_PROTOCOL_CMD_TONE_ONLY       = 4,  // turn on the test tone (synth disabled)
  AE_PROTOCOL_CMD_POLL_DATA_START = 5,  // start of polled data
  AE_PROTOCOL_CMD_POLL_DATA_STOP  = 6,  // end of polled data
};

enum PLAYCONTROLLER_SYSTEM_SPECIAL_COMMANDS
{
  SYS_SPECIAL_RESET_HEARTBEAT = 1,
  SYS_SPECIAL_RESET_SYSTEM    = 2,
  SYS_SPECIAL_ENABLE_MIDI     = 3,
};

// void     SUP_SetMuteOverride(uint32_t mode);
// uint16_t SUP_GetUnmuteStatusBits(void);
// bit masks for these functions :
// (for SUP_SetMuteOverride() only *_SOFTWARE_* masks are used/allowed)
enum SUP_MUTING_BITS
{
  SUP_UNMUTE_STATUS_IS_VALID           = 0b1000000000000000,  // status has actually been set
  SUP_UNMUTE_STATUS_JUMPER_OVERRIDE    = 0b0000000010000000,  // hardware jumper overriding everything ...
  SUP_UNMUTE_STATUS_JUMPER_VALUE       = 0b0000000001000000,  // ... with this value (1:unmuted)
  SUP_UNMUTE_STATUS_SOFTWARE_OVERRIDE  = 0b0000000000100000,  // software command overriding midi-derived status ...
  SUP_UNMUTE_STATUS_SOFTWARE_VALUE     = 0b0000000000010000,  // ... with this value (1:unmuted)
  SUP_UNMUTE_STATUS_MIDI_DERIVED       = 0b0000000000001000,  // midi-derived status ...
  SUP_UNMUTE_STATUS_MIDI_DERIVED_VALUE = 0b0000000000000100,  // ... with this value (1:unmuted)
  SUP_UNMUTE_STATUS_HARDWARE_IS_VALID  = 0b0000000000000010,  // hardware status is valid (signal from SUP uC was detected) ...
  SUP_UNMUTE_STATUS_HARDWARE_VALUE     = 0b0000000000000001,  // ... with this value (1:unmuted)
};

//
// ---------- TYPEDEFS ----------------
//

typedef struct
{
  unsigned ctrlId : 3;            // controller number 0...7, aka input (main) ADC channel 0...7, 0/1=J1T/R, 2/3=J2T/R, etc,
  unsigned hwId : 4;              // hardware ID used for messages to AE and PG
  unsigned silent : 1;            // disable messaging to AudioEngine
  unsigned is3wire : 1;           // controller connection type, 0=2wire(rheo/sw/cv), 1=3wire(pot)
  unsigned pullup : 1;            // controller input sensing, 0=unloaded(pot/CV), 1=with pullup(rheo/sw)
  unsigned continuous : 1;        // controller output type, 0=continuous(all), 1=bi-stable(all)
  unsigned polarityInvert : 1;    // invert, or don't, the final output(all)
  unsigned autoHoldStrength : 3;  // controller auto-hold 0..7, 0(off)...4=autoHold-Strength for pot/rheo
  unsigned doAutoRanging : 1;     // enable auto-ranging, or assume static (but adjustable) thresholds/levels
} EHC_ControllerConfig_T;

typedef struct
{
  unsigned initialized : 1;    // controller is initialized (has valid setup)
  unsigned pluggedIn : 1;      // controller is plugged in
  unsigned isReset : 1;        // controller is freshly reset
  unsigned outputIsValid : 1;  // controller final output value has been set
  unsigned isAutoRanged : 1;   // controller has finished auto-ranging (always=1 when disabled)
  unsigned isSettled : 1;      // controller output is within 'stable' bounds and step-freezing (not valid for bi-stable)
  unsigned isRamping : 1;      // controller currently does a ramp to the actual value (pot/rheo) (not valid for bi-stable)
  unsigned isSaved : 1;        // controller state has been saved to EEPROM
  unsigned isRestored : 1;     // controller state has been restored from EEPROM

} EHC_ControllerStatus_T;

// --- Aftertouch per Key Calibration data structure ---
typedef struct
{
  uint16_t keybedId;       // keybed serial # (id = 0 forces legacy AT mode)
  uint16_t adcTarget;      // adc target value for the test force (typically for 10N)
  uint16_t adcDefault;     // default when AT is active without any key pressed (typically the average of all the following values)
  uint16_t adcValues[61];  // adc values obtained for test force for all 61 keys
} AT_calibration_T;

typedef struct
{
  unsigned legacyMode : 1;   // lecacy AT mode currently in use (Note: non-legacy mode only is active with a valid calibration)
  unsigned calibrated : 1;   // calibration in use (ignore in legacy mode)
  unsigned maskedKeys : 1;   // some key are masked (ignore in legacy mode)
  unsigned silentKeys : 1;   // masked key only will silence the AT output (ignore in legacy mode)
  unsigned eepromValid : 1;  // valid calibration data found at boot time
} AT_status_T;

// ---- Bender status
typedef struct
{
  unsigned legacyMode : 1;       // lecacy Bender mode currently in use
  unsigned zeroed : 1;           // current value is zero position  (ignore in legacy mode)
  unsigned everZeroed : 1;       // a zero position has ever been reached (ignore in legacy mode)
  unsigned leftEndStop : 1;      // left end stop has been adjusted (ignore in legacy mode)
  unsigned rightEndStop : 1;     // right end stop has been adjusted (ignore in legacy mode)
  unsigned offZero : 1;          // a settled value has occured outside the auto-zero range (and not being an end stop)
  unsigned useFineSettling : 1;  // using fine settling at the moment
  unsigned settledFine : 1;      // current movement as settled wrt "fine" settling criteria
  unsigned settledCoarse : 1;    // current movement as settled wrt "coarse" settling criteria
  unsigned reasonableZero : 1;   // current zero is with the usually expected range
} BNDR_status_T;
