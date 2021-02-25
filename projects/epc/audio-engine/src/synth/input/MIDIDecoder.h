#pragma once
#include <Types.h>

class MIDIDecoder
{
 public:
  bool decode(const MidiEvent& event);

  [[nodiscard]] DecoderEventType getEventType() const;
  [[nodiscard]] int getKeyOrControl() const;
  [[nodiscard]] double getValue() const;

 private:
  int keyOrControl = -1;
  double value = 0;  // value or velocity
  DecoderEventType m_type = DecoderEventType::UNKNOWN;
};