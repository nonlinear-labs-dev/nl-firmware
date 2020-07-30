#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/controls/SymbolLabel.h>

class SubmenuIndicator : public Control
{
 public:
  explicit SubmenuIndicator(const Rect& rect);
  bool redraw(FrameBuffer& fb) override;

 private:
  SymbolLabel m_label;
};
