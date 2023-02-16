#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/AnimatedGenericItem.h>
#include <nltools/Types.h>
#include "proxies/hwui/descriptive-layouts/ConditionRegistry.h"

class ConvertToSoundTypeItem : public AnimatedGenericItem
{
 public:
  explicit ConvertToSoundTypeItem(const Rect& rect, SoundType toType);

 private:
  std::unique_ptr<ConditionRegistry::ConditionRegistryScopedLock> m_lock;
};

class ConvertToSoundTypeItemWithFX : public AnimatedGenericItem
{
 public:
  explicit ConvertToSoundTypeItemWithFX(const Rect& rect, SoundType convertTo);

 private:
  std::unique_ptr<ConditionRegistry::ConditionRegistryScopedLock> m_lock;
};