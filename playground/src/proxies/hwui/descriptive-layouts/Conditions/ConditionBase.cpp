#include <proxies/hwui/descriptive-layouts/ConditionRegistry.h>
#include "ConditionBase.h"

void ConditionBase::onConditionChanged() const
{
 #warning"adlerauge"
  ConditionRegistry::get().onConditionChanged();
}
