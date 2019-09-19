#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/GenericItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/ConditionRegistry.h>
#include "SingleSoundEditMenu.h"
#include <proxies/hwui/HWUI.h>

SingleSoundEditMenu::SingleSoundEditMenu(const Rect& r)
    : ScrollMenu(r)
{
  init();
}

void SingleSoundEditMenu::init()
{
  addItem<InitSound>();
  addItem<RandomizeItem>();
}
