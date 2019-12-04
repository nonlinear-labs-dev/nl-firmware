#pragma once
#include "Dimension.h"

class NoteWithOctaveDimension : public Dimension
{
 public:
  static Glib::ustring stringizeNote(int note);
  Glib::ustring stringize(const tDisplayValue &displayValue) const override;
  static const NoteWithOctaveDimension &get();
  Glib::ustring getStingizerJS() const override;
  static void registerTests();

  static constexpr auto OctaveCount = 5;
  static constexpr auto KeysPerOctave = 12;
  static constexpr auto KeyCount = OctaveCount * KeysPerOctave;
};
