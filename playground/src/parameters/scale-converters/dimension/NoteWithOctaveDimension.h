#pragma once
#include "Dimension.h"

class NoteWithOctaveDimension : public Dimension
{
 public:
  static ustring stringizeNote(int note);
  ustring stringize(const tDisplayValue &displayValue) const override;
  static const NoteWithOctaveDimension &get();
  //TODO stringize JS
  static void registerTests();

  static constexpr auto OctaveCount = 5;
  static constexpr auto KeysPerOctave = 12;
  static constexpr auto KeyCount = OctaveCount * KeysPerOctave;
  static constexpr std::array<const char *, KeysPerOctave> keyLabels
      = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
};
