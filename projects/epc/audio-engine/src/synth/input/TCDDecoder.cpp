#include "TCDDecoder.h"

bool TCDDecoder::decode(const MidiEvent& event)
{
  return m_type != DecoderEventType::UNKNOWN;
}

DecoderEventType TCDDecoder::getEventType() const
{
  return m_type;
}

int TCDDecoder::getKeyOrController() const
{
  return keyOrController;
}

double TCDDecoder::getValue() const
{
  return value;
}
