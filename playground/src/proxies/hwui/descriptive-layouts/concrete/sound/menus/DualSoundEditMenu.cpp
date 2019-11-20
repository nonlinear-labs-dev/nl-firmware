#include "DualSoundEditMenu.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSingleItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/LoadPresetIntoVoiceGroupItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/TextItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/VoiceGroupLabelItem.h>
#include <proxies/hwui/HWUI.h>

DualSoundEditMenu::DualSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void DualSoundEditMenu::init()
{
  const auto height = 52 / 4;
  auto fullWidth = Rect { 0, 0, 254, height };
  auto currentLayer = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto str = toString(currentLayer);
  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();
  auto typeString = toString(type);
  addItem<TextItem>("-- " + typeString + " " + str + "  --", fullWidth);
  addItem<LoadPresetIntoVoiceGroupItem>(fullWidth);
  addItem<VoiceGroupLabelItem>(fullWidth);
  addItem<ConvertToSingleItem>(fullWidth);
}
