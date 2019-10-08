#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/CaptionLabel.h>
#include "MenuItem.h"

class BasicItem : public MenuItem
{
 public:
  template <class T>
  explicit BasicItem(T caption, const Rect& r)
      : MenuItem(r)
      , m_caption{ caption }
  {
    auto leftHalf = getPosition();
    leftHalf.setLeft(leftHalf.getLeft() + 1);
    leftHalf.setWidth((leftHalf.getWidth() / 2) - 1);

    addControl(new CaptionLabel<LeftAlignedLabel>(m_caption, leftHalf, true));
  }

  bool redraw(FrameBuffer& fb) override;
  bool canEnter() override;

 protected:
  virtual bool drawHighlightBorder(FrameBuffer& fb);

  Glib::ustring m_caption;
};
