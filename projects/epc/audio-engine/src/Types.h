#pragma once

#include <cstdint>
#include <chrono>
#include <initializer_list>
#include <string>

using Sample = float;
using SamplePosition = uint64_t;

struct MidiEvent
{
  uint8_t raw[3]{ 0x0 };
  std::chrono::high_resolution_clock::time_point timestamp;
};

enum class DecoderEventType
{
  KeyDown,
  KeyUp,
  HardwareChange,
  PollStart,
  PollStop,
  UNKNOWN
};

// note: unnecessary code duplication (better use C15::Parameters::Hardware_Sources in <parameter_declarations.h>
enum class HardwareSource : int8_t
{
  PEDAL1 = 0,
  PEDAL2 = 1,
  PEDAL3 = 2,
  PEDAL4 = 3,
  BENDER = 4,
  AFTERTOUCH = 5,
  RIBBON1 = 6,
  RIBBON2 = 7,
  RIBBON3 = 8,
  RIBBON4 = 9,
  NONE = -1
};

inline std::string toString(HardwareSource s)
{
  switch(s)
  {
    case HardwareSource::PEDAL1:
      return "PEDAL1";
    case HardwareSource::PEDAL2:
      return "PEDAL2";
    case HardwareSource::PEDAL3:
      return "PEDAL3";
    case HardwareSource::PEDAL4:
      return "PEDAL4";
    case HardwareSource::BENDER:
      return "BENDER";
    case HardwareSource::AFTERTOUCH:
      return "AFTERTOUCH";
    case HardwareSource::RIBBON1:
      return "RIBBON1";
    case HardwareSource::RIBBON2:
      return "RIBBON2";
    case HardwareSource::RIBBON3:
      return "RIBBON3";
    case HardwareSource::RIBBON4:
      return "RIBBON4";
    case HardwareSource::NONE:
      return "NONE";
    default:
      return std::to_string(static_cast<int>(s));
  }
}

static auto sHardwareSources
    = { HardwareSource::PEDAL1, HardwareSource::PEDAL2,     HardwareSource::PEDAL3,  HardwareSource::PEDAL4,
        HardwareSource::BENDER, HardwareSource::AFTERTOUCH, HardwareSource::RIBBON1, HardwareSource::RIBBON2,
        HardwareSource::RIBBON3, HardwareSource::RIBBON4 };

struct SampleFrame
{
  Sample left;
  Sample right;
};

using SampleInt16 = int16_t;
using SampleInt32 = int32_t;
using SampleFloat32 = float;

using FrameId = uint64_t;
