#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>

class MultiLineInfoContent : public MultiLineLabel
{
 public:
  MultiLineInfoContent();
  void setPosition(const Rect& rect) override;
  int getBottomPadding() override;
};
