#include "RibbonModeLabel.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/RibbonParameter.h>

RibbonModeLabel::RibbonModeLabel(const ParameterId& paramID, const Rect& rect)
    : Label(rect)
{
  m_param = Application::get().getPresetManager()->getEditBuffer()->findAndCastParameterByID<RibbonParameter>(paramID);
  if(m_param)
  {
    m_param->onParameterChanged(sigc::hide(sigc::mem_fun(this, &RibbonModeLabel::setDirty)));
  }
}

StringAndSuffix RibbonModeLabel::getText() const
{
  switch(m_param->getRibbonTouchBehaviour())
  {
    case RibbonTouchBehaviour::ABSOLUTE:
      return { "a", 0 };
    case RibbonTouchBehaviour::RELATIVE:
      return { "r", 0 };
    default:
    case RibbonTouchBehaviour::NUM_TOUCH_BEHAVIOURS:
      return { "", 0 };
  }
}
