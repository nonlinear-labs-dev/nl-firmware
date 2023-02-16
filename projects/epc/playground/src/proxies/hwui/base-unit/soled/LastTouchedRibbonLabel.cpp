#include "LastTouchedRibbonLabel.h"
#include "device-settings/BaseUnitUIMode.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <Application.h>
#include <parameters/RibbonParameter.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

LastTouchedRibbonLabel::LastTouchedRibbonLabel(const ParameterId& id, const Rect& pos)
    : Label(pos)
{
  setText({ "<", 0 });
  m_param = Application::get().getPresetManager()->getEditBuffer()->findAndCastParameterByID<RibbonParameter>(id);
  Application::get().getPlaycontrollerProxy()->onRibbonTouched(
      sigc::mem_fun(this, &LastTouchedRibbonLabel::onRibbonTouched));
  Application::get().getSettings()->getSetting<BaseUnitUIMode>()->onChange(
      sigc::mem_fun(this, &LastTouchedRibbonLabel::onBaseUnitModeChanged));
}

void LastTouchedRibbonLabel::onRibbonTouched(int ribbon)
{
  isCurrentParamLastSelected = m_param->getID().getNumber() == ribbon;
  setVisible(isCurrentParamLastSelected && !isInEditMode);
}

void LastTouchedRibbonLabel::onBaseUnitModeChanged(const Setting* s)
{
  if(auto modeSetting = dynamic_cast<const BaseUnitUIMode*>(s))
  {
    isInEditMode = modeSetting->get() == BaseUnitUIModes::ParameterEdit;
    setVisible(isCurrentParamLastSelected && !isInEditMode);
  }
}
