#include "DualSoundEditMenu.h"

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/LoadPresetIntoVoiceGroupItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/PartLabelItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/TextItem.h>
#include <proxies/hwui/HWUI.h>

DualSoundEditMenu::DualSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void DualSoundEditMenu::init()
{
  auto fullWidth = Rect{ 0, 0, 254, 13 };
  addItem<LoadPresetIntoVoiceGroupItem>(fullWidth);
  addItem<PartLabelItem>(fullWidth);
  addItem<ConvertToSingleItem>(fullWidth);
}
