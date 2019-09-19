#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/BasicItem.h>

class LoadPresetIntoVoiceGroupItem : public BasicItem
{
 public:
  LoadPresetIntoVoiceGroupItem();
  void doAction() override;
};
