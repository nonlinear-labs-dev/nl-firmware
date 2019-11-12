#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/BasicItemWithValueText.h>

class VoiceGroupLabelItem : public BasicItemWithValueText
{
 public:
  VoiceGroupLabelItem(const Rect& r);
  void doAction() override;
};
