#include "DualSoundEditMenu.h"

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSoundTypeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/PartLabelItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>

#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>

#include <Application.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/SplitSyncItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/CopyFX.h>

DualSoundEditMenu::DualSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

void DualSoundEditMenu::init()
{

  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();

  if(type == SoundType::Split)
  {
    initSplit();
  }
  else if(type == SoundType::Layer)
  {
    initLayer();
  }
}

int DualSoundEditMenu::getDefaultItemHeight() const
{
  return 12;
}

void DualSoundEditMenu::initSplit()
{
  auto fullWidth = Rect { 0, 0, 256, 12 };
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Single);
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Layer);
  addItem<CopyFX<VoiceGroup::I, VoiceGroup::II>>(fullWidth);
  addItem<CopyFX<VoiceGroup::II, VoiceGroup::I>>(fullWidth);
  addItem<PartLabelItem>(fullWidth);
  addItem<SplitSyncItem>(fullWidth);
  addItem<InitPart>(fullWidth);
  addItem<InitSound>(fullWidth);
  addItem<RandomizePart>(fullWidth);
}

void DualSoundEditMenu::initLayer()
{
  auto fullWidth = Rect { 0, 0, 256, 12 };
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Single);
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Split);
  addItem<CopyFX<VoiceGroup::I, VoiceGroup::II>>(fullWidth);
  addItem<CopyFX<VoiceGroup::II, VoiceGroup::I>>(fullWidth);
  addItem<PartLabelItem>(fullWidth);
  addItem<InitPart>(fullWidth);
  addItem<InitSound>(fullWidth);
  addItem<RandomizePart>(fullWidth);
}