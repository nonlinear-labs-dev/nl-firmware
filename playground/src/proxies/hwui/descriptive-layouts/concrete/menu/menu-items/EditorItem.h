#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/SubmenuIndicator.h>
#include "BasicItem.h"
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ScrollMenuOverlay.h"

class EditorItem : public BasicItem
{
 public:
  bool canEnter() override;

  template <class tCaption>
  explicit EditorItem(tCaption cap)
      : BasicItem(cap)
  {
  }

  bool redraw(FrameBuffer& fb) override;
  Rect getStandardOverlayRect();
  virtual ScrollMenuOverlay* createOverlay() = 0;
  void doAction() override;

 protected:
  bool drawHighlightBorder(FrameBuffer& fb) override;
  bool redrawEnterIndication(FrameBuffer& buffer);
};
