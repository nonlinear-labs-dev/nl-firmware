#pragma once

#include <http/RPCActionManager.h>
#include <presets/EditBuffer.h>
#include "HWUI.h"
class HWUIActions : public RPCActionManager
{
 public:
  HWUIActions(HWUI& hwui, EditBuffer& eb);
};
