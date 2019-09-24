#include "HWUIConditions.h"
#include "Application.h"
#include "proxies/hwui/HWUI.h"

namespace HWUIConditions
{
  HWUICondition::HWUICondition()
  {
    m_hwuiConnection
        = Application::get().getHWUI()->onModifiersChanged(sigc::mem_fun(this, &HWUICondition::onModifiersChanged));
    m_modifiers = Application::get().getHWUI()->getButtonModifiers();
  }

  HWUICondition::~HWUICondition()
  {
    m_hwuiConnection.disconnect();
  }

  void HWUICondition::onModifiersChanged(const ButtonModifiers &mod)
  {
    m_modifiers = mod;
    update();
  }

  bool IsFineActive::check() const
  {
    return m_modifiers[ButtonModifier::FINE];
  }
}
