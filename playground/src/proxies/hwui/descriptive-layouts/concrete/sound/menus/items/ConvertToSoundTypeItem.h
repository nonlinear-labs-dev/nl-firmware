#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/AnimatedGenericItem.h>
#include <nltools/Types.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/SoundEditMenu.h>

class ConvertToSoundTypeItem : public AnimatedGenericItem
{
 public:
  explicit ConvertToSoundTypeItem(const Rect& rect, SoundType toType);
};