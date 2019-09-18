#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include "ConditionBase.h"

namespace HWUIConditions
{
  class HWUICondition : public ConditionBase
  {
   public:
   public:
    HWUICondition();
    virtual ~HWUICondition();

   protected:
    void onModifiersChanged(const ButtonModifiers& mod);

    ButtonModifiers m_modifiers;
    sigc::connection m_hwuiConnection;
  };

  class IsFineActive : public HWUICondition
  {
   public:
    bool check() const override;
  };
};
