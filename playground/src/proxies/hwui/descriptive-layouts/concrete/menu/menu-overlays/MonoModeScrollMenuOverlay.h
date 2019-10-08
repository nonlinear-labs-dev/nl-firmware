#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>
#include "MenuOverlay.h"

class MonoModeScrollMenuOverlay : public MenuOverlay {
public:
  explicit MonoModeScrollMenuOverlay(const Rect& r);
  bool onButton(Buttons i, bool down, ButtonModifiers mod) override;

  bool redraw(FrameBuffer &fb) override;

private:
  class ScrollMenuOverlayMenu : public ScrollMenu {
  public:
    explicit ScrollMenuOverlayMenu(const Rect& r);
    void init() override;
  };

  ScrollMenuOverlayMenu m_menu;
};
