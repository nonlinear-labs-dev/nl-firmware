#include "PhysicalControlBehaviorLabel.h"
#include "parameters/RibbonParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/PhysicalControlParameter.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>
#include <proxies/hwui/HWUI.h>

PhysicalControlBehaviorLabel::PhysicalControlBehaviorLabel(const Rect &pos)
    : Label(pos)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &PhysicalControlBehaviorLabel::setParameter)), vg);
}

void PhysicalControlBehaviorLabel::setParameter(Parameter *param)
{
  if(param)
  {
    m_paramConnection.disconnect();
    m_paramConnection
        = param->onParameterChanged(sigc::mem_fun(this, &PhysicalControlBehaviorLabel::onParamValueChanged));
  }
}

void PhysicalControlBehaviorLabel::onParamValueChanged(const Parameter *param)
{
  if(auto p = dynamic_cast<const PhysicalControlParameter *>(param))
  {
    if(p->hasBehavior())
    {
      setText(p->getCurrentBehavior());
    }
  }
}

void PhysicalControlBehaviorLabel::setBackgroundColor(FrameBuffer &fb) const
{
  if(isVisible())
    Control::setBackgroundColor(fb);
  else
    fb.setColor(FrameBufferColors::Transparent);
}

PhysicalControlModeLabel::PhysicalControlModeLabel(const Rect &pos)
    : Label(pos)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &PhysicalControlModeLabel::setParameter)), vg);
}

void PhysicalControlModeLabel::setBackgroundColor(FrameBuffer &fb) const
{
  if(isVisible())
    Control::setBackgroundColor(fb);
  else
    fb.setColor(FrameBufferColors::Transparent);
}

void PhysicalControlModeLabel::setParameter(Parameter *param)
{
  if(param)
  {
    m_paramConnection.disconnect();
    m_paramConnection = param->onParameterChanged(sigc::mem_fun(this, &PhysicalControlModeLabel::onParameterChanged));
  }
}

void PhysicalControlModeLabel::onParameterChanged(const Parameter *param)
{
  if(auto ribbon = dynamic_cast<const RibbonParameter *>(param))
  {
    switch(ribbon->getRibbonTouchBehaviour())
    {
      case RibbonTouchBehaviour::ABSOLUTE:
        setText("Absolute");
        return;
      case RibbonTouchBehaviour::RELATIVE:
        setText("Relative");
        return;
      default:
        setText("");
        return;
    }
  }
}
