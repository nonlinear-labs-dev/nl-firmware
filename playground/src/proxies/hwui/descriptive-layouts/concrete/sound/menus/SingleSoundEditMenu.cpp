#include "SingleSoundEditMenu.h"

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>

SingleSoundEditMenu::SingleSoundEditMenu(const Rect &r)
    : SoundEditMenu(r)
{
  init();
}

void SingleSoundEditMenu::init()
{
  auto fullWidth = Rect{ 0, 0, 254, 13 };
  addItem<InitSound>(fullWidth);
  addItem<RandomizeItem>(fullWidth);
}
