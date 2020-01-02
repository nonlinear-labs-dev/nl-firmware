#include <testing/TestDriver.h>
#include <cmath>
#include "SplitPointDimension.h"

Glib::ustring SplitPointDimension::stringizeSplitPointDisplay(tControlPositionValue value, VoiceGroup seenFrom)
{
  int note = std::round(value * 59);

  if(seenFrom == VoiceGroup::II)
    note++;

  return stringizeNote(note);
}

Glib::ustring SplitPointDimension::stringizeNote(int note)
{
  const std::array<const char *, SplitPointDimension::KeysPerOctave> keyLabels
      = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

  auto index = note % keyLabels.size();
  auto base = keyLabels[index];
  auto octave = static_cast<int>(note / keyLabels.size()) + 1;
  return base + std::to_string(octave);
}

Glib::ustring SplitPointDimension::stringize(const tDisplayValue &displayValue) const
{
#warning TODO
  return stringizeSplitPointDisplay(displayValue, VoiceGroup::I);
}

Glib::ustring SplitPointDimension::getStingizerJS() const
{
  return "formatNote(cpValue)";
}

const SplitPointDimension &SplitPointDimension::get()
{
  static SplitPointDimension dim;
  return dim;
}