#include "LastTouchedRibbonLabel.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <Application.h>
#include <parameters/RibbonParameter.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

LastTouchedRibbonLabel::LastTouchedRibbonLabel(const ParameterId& id, const Rect& pos)
: Label(pos)
{
  setText({"<", 0});
  m_param = Application::get().getPresetManager()->getEditBuffer()->findAndCastParameterByID<RibbonParameter>(id);
  Application::get().getPlaycontrollerProxy()->onRibbonTouched(sigc::mem_fun(this, &LastTouchedRibbonLabel::onRibbonTouched));
}

void LastTouchedRibbonLabel::onRibbonTouched(int ribbon)
{
  setVisible(m_param->getID().getNumber() == ribbon);
}
