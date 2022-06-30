#pragma once
#include "proxies/hwui/controls/Label.h"
#include "nltools/threading/Throttler.h"

class ChangedParameterIndicator : public Label
{
 public:
  explicit ChangedParameterIndicator(const Rect& pos);
  int getZPriority() const override;

 private:
  void update();

  Throttler m_worker;
};