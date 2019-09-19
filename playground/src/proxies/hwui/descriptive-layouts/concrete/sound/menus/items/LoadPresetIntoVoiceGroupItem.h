#pragma once
#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuItems/BasicItem.h>

class LoadPresetIntoVoiceGroupItem : public BasicItem
{
 public:
  LoadPresetIntoVoiceGroupItem();
  void doAction() override;
};
