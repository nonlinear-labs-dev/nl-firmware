#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>
#include "MonoParameterOverlay.h"

class MonoParameterItem;
class Parameter;

class MonoModeOverlay : public MenuOverlay
{
 public:
  explicit MonoModeOverlay(const Rect& r);
  bool onButton(Buttons i, bool down, ButtonModifiers mod) override;

  bool redraw(FrameBuffer& fb) override;

 private:
  void selectNext();
  void selectPrev();
  void stepCurr(short inc);
  void enterCurr();
  bool exitCurr();

  Parameter* getParameter(uint16_t id) const;
  std::vector<MonoParameterItem*> m_labels;

  MonoParameterOverlay* m_overlay{ nullptr };

  size_t selection{ 0 };
};
