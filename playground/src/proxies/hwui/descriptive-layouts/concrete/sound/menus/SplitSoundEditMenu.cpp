#include "SplitSoundEditMenu.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/RandomizeAmount.h>

#include <device-settings/TransitionTime.h>
#include <device-settings/EditSmoothingTime.h>
#include <device-settings/TuneReference.h>

#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuItems/GenericItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuItems/SettingItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/Items/LoadPresetIntoVoiceGroupItem.h>

SplitSoundEditMenu::SplitSoundEditMenu(const Rect& r)
    : ScrollMenu(r)
{
  init();
}

void SplitSoundEditMenu::init()
{
  addItem<LoadPresetIntoVoiceGroupItem>();

  addItem<ConvertToSingleItem>();
  addItem<InitSound>();
  addItem<RandomizeItem>();

  addItem<SettingItem<TransitionTime>>("Transition Time");
  addItem<SettingItem<EditSmoothingTime>>("Smoothing Time");
  addItem<SettingItem<TuneReference>>("Tune Reference");
}
