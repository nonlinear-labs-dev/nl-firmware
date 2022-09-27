#pragma once
#include <proxies/hwui/controls/Control.h>

class Setting;

class SelectedRibbonsIndication : public Control
{
 public:
  explicit SelectedRibbonsIndication(const Rect& pos);
  void onSettingChanged(const Setting* s);
  bool redraw(FrameBuffer& fb) override;

 protected:
  bool m_isRibbon1_2_selected = true;
};
