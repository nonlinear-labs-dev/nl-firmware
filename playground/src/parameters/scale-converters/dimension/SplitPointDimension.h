#pragma once

#include <nltools/Types.h>
#include "Dimension.h"

class SplitPointDimension : public Dimension
{
 public:
  static Glib::ustring stringizeSplitPointDisplay(tControlPositionValue value, VoiceGroup seenFrom);
  static Glib::ustring stringizeNote(int note);
  Glib::ustring stringize(const tDisplayValue &displayValue) const override;
  static const SplitPointDimension &get();
  Glib::ustring getStingizerJS() const override;

  static constexpr auto OctaveCount = 5;
  static constexpr auto KeysPerOctave = 12;
  static constexpr auto KeyCount = OctaveCount * KeysPerOctave;
};
