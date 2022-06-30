#pragma once
#include "proxies/hwui/controls/Label.h"

class ChangedParameterIndicator : public Label
{
 public:
  explicit ChangedParameterIndicator(const Rect& pos);
  int getZPriority() const override;

 private:
  void update();
};