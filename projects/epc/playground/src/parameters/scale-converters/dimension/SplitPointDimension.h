#pragma once

#include <nltools/Types.h>
#include "KeyDimension.h"
#include <cmath>

class SplitPointDimension : public KeyDimension
{
 public:
  [[nodiscard]] Glib::ustring stringize(const tDisplayValue &displayValue) const override;
  Glib::ustring getStingizerJS() const override;

  static const SplitPointDimension &get();
};
