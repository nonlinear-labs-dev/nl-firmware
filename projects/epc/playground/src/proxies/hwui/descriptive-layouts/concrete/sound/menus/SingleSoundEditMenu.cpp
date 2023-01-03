#include "SingleSoundEditMenu.h"

#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/InitSound.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/RandomizeItem.h>
#include <nltools/Types.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/ConvertToSoundTypeItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/CopyFX.h>

SingleSoundEditMenu::SingleSoundEditMenu(const Rect &r)
    : ScrollMenu(r)
{
  init();
}

int SingleSoundEditMenu::getDefaultItemHeight() const
{
  return 12;
}

void SingleSoundEditMenu::init()
{
  auto fullWidth = Rect { 0, 0, 256, 12 };
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Split);
  addItem<ConvertToSoundTypeItem>(fullWidth, SoundType::Layer);
  addItem<CopyFX<VoiceGroup::I, VoiceGroup::II>>(fullWidth);
  addItem<CopyFX<VoiceGroup::II, VoiceGroup::I>>(fullWidth);
  addItem<InitSound>(fullWidth);
  addItem<RandomizeItem>(fullWidth);
}
