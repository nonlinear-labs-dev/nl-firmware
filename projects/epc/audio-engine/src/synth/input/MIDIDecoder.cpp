#include "MIDIDecoder.h"

bool MIDIDecoder::decode(const MidiEvent& event)
{

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
