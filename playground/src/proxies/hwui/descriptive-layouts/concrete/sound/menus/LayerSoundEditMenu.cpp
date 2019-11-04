#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/LoadPresetIntoVoiceGroupItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/TextItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/SettingItem.h>
#include <device-settings/TransitionTime.h>
#include <device-settings/TuneReference.h>

#include "LayerSoundEditMenu.h"

LayerSoundEditMenu::LayerSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void LayerSoundEditMenu::init()
{
  const auto height = 52 / 4;
  auto fullWidth = Rect{ 0, 0, 254, height };
  auto currentLayer = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  auto str = toString(currentLayer);
  addItem<TextItem>("-- Layer " + str + "  --", fullWidth);
  addItem<LoadPresetIntoVoiceGroupItem>(fullWidth);
  addItem<ConvertToSingleItem>(fullWidth);
}
