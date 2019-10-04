#pragma once
#include "Dimension.h"

class NoteWithOctaveDimension : public Dimension {
public:
  ustring stringize(const tDisplayValue &displayValue) const override;
  static const NoteWithOctaveDimension &get();
  //TODO stringize JS

  static void registerTests();
};
