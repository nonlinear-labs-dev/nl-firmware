#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/descriptive-layouts/ConditionRegistry.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/LoadPresetIntoVoiceGroupItem.h>

#include "LayerSoundEditMenu.h"

LayerSoundEditMenu::LayerSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void LayerSoundEditMenu::init()
{
  addItem<LoadPresetIntoVoiceGroupItem>();
  addItem<ConvertToSingleItem>();
  addItem<InitSound>();
  addItem<RandomizeItem>();
}
