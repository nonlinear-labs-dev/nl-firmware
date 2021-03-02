#include "MIDIDecoder.h"
#include <nltools/logging/Log.h>

DecoderEventType statusToType(uint8_t status)
{
  auto statusWithoutChannel = (status & 0b11110000);
  switch(statusWithoutChannel)
  {
    case 144:
      return DecoderEventType::KeyDown;
    case 128:
      return DecoderEventType::KeyUp;
    case 176:
      return DecoderEventType::HardwareChange;
    default:
      nltools::Log::warning("Unkown Status Byte:", (int) status);
      return DecoderEventType::UNKNOWN;
  }
}

bool MIDIDecoder::decode(const MidiEvent& event)
{
  auto type = statusToType(event.raw[0]);
  switch(type)
  {
    case DecoderEventType::KeyDown:
      m_type = type;
      keyOrControl = (int) event.raw[1];
      value = (int) event.raw[2];
      break;
    case DecoderEventType::KeyUp:
      m_type = type;
      keyOrControl = (int) event.raw[1];
      value = (int) event.raw[2];
      break;
    case DecoderEventType::HardwareChange:
      m_type = type;
      keyOrControl = (int) event.raw[1];
      value = (int) event.raw[2];
      break;
  }
  return m_type != DecoderEventType::UNKNOWN;
}

DecoderEventType MIDIDecoder::getEventType() const
{
  return m_type;
}

int MIDIDecoder::getKeyOrControl() const
{
  return keyOrControl;
}

double MIDIDecoder::getValue() const
{
  return value;
}
