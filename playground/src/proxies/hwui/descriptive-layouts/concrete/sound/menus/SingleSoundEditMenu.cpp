#include "SingleSoundEditMenu.h"

#include <device-settings/TuneReference.h>
#include <device-settings/TransitionTime.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/GenericItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/TextItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/SettingItem.h>

SingleSoundEditMenu::SingleSoundEditMenu(const Rect &r)
    : SoundEditMenu(r)
{
  init();
}

void SingleSoundEditMenu::init()
{
  const auto height = 52 / 4;
  auto fullWidth = Rect{ 0, 0, 254, height };
  addItem<TextItem>("-- Single --", fullWidth);
  addItem<InitSound>(fullWidth);
  addItem<RandomizeItem>(fullWidth);
  addItem<SettingItem<TransitionTime>>("Transition Time", fullWidth);
  addItem<SettingItem<TuneReference>>("Tune Reference", fullWidth);
}
