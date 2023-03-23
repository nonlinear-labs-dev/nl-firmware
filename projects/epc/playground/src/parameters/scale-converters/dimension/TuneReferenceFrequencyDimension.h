#pragma once
#include "FrequencyDimension.h"

class TuneReferenceFrequencyDimension : public Dimension
{
 public:
  using Dimension::Dimension;
  static const TuneReferenceFrequencyDimension &get();

  Glib::ustring getStingizerJS() const override;

  Glib::ustring stringize(const tDisplayValue &displayValue) const override;
};