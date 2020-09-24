#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/SubmenuIndicator.h>
#include "BasicItem.h"
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/MenuOverlay.h"

class EditorItem : public BasicItem
{
 public:
  bool canEnter() override;

  template <class tCaption>
  explicit EditorItem(tCaption cap, const Rect& r)
      : BasicItem(cap, r)
  {
  }

  bool redraw(FrameBuffer& fb) override;
  Rect getDefaultOverlayRect();
  Rect getFullRightSideOverlayRect();
  virtual MenuOverlay* createOverlay() = 0;
  void doAction() override;

 protected:
  bool redrawEnterIndication(FrameBuffer& buffer);
};
