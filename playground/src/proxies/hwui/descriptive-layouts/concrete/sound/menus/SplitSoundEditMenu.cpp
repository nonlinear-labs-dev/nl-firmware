#include "SplitSoundEditMenu.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/RandomizeAmount.h>

#include <device-settings/TransitionTime.h>
#include <device-settings/EditSmoothingTime.h>
#include <device-settings/TuneReference.h>

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/GenericItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/SettingItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/LoadPresetIntoVoiceGroupItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/SplitPointItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/TextItem.h>

SplitSoundEditMenu::SplitSoundEditMenu(const Rect& r)
    : ScrollMenu(r)
{
  init();
}

void SplitSoundEditMenu::init()
{
  addItem<TextItem>("-- Split --");
  addItem<LoadPresetIntoVoiceGroupItem>();
  addItem<SplitPointItem>();
  addItem<ConvertToSingleItem>();
  addItem<InitSound>();
  addItem<RandomizeItem>();
  addItem<TextItem>("-- Common --");
  addItem<SettingItem<TransitionTime>>("Transition Time");
  addItem<SettingItem<EditSmoothingTime>>("Smoothing Time");
  addItem<SettingItem<TuneReference>>("Tune Reference");
}
