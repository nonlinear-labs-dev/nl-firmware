#include "DualSoundEditMenu.h"

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/PartLabelItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>

DualSoundEditMenu::DualSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void DualSoundEditMenu::init()
{
  auto fullWidth = Rect{ 0, 0, 254, 13 };
  addItem<PartLabelItem>(fullWidth);
  addItem<ConvertToSingleItem>(fullWidth);
  addItem<InitPart>(fullWidth);
}
