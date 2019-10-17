#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>

class MenuOverlay : public ControlWithChildren, public ButtonReceiver
{
 public:
  explicit MenuOverlay(const Rect& r);
  bool redraw(FrameBuffer& fb) override;
};
