#pragma once

#include <cstdint>
#include <chrono>
#include <initializer_list>

using Sample = float;
using SamplePosition = uint64_t;

struct MidiEvent
{
  uint8_t raw[3] { 0x0 };
  std::chrono::high_resolution_clock::time_point timestamp;
};

enum class DecoderEventType
{
  KeyDown,
  KeyUp,
  HardwareChange,
  UNKNOWN
};

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
  NONE = -1
};

static auto sHardwareSources
    = { HardwareSource::PEDAL1, HardwareSource::PEDAL2,     HardwareSource::PEDAL3,  HardwareSource::PEDAL4,
        HardwareSource::BENDER, HardwareSource::AFTERTOUCH, HardwareSource::RIBBON1, HardwareSource::RIBBON2 };

struct SampleFrame
{
  Sample left;
  Sample right;
};

using SampleInt16 = int16_t;
using SampleInt32 = int32_t;
using SampleFloat32 = float;

using FrameId = uint64_t;