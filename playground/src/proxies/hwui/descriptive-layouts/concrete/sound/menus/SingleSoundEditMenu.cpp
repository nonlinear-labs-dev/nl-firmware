#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuItems/GenericItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/RandomizeItem.h>
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
