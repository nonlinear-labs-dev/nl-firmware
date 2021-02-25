#pragma once
#include <Types.h>

class TCDDecoder
{
 public:
  bool decode(const MidiEvent& event);

  [[nodiscard]] DecoderEventType getEventType() const;
  [[nodiscard]] int getKeyOrController() const;
  [[nodiscard]] double getValue() const;

 private:
  int keyOrController = -1;
  double value = 0;
  DecoderEventType m_type = DecoderEventType::UNKNOWN;
};