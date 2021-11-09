#include "PhysicalControlSlider.h"
#include <parameters/PhysicalControlParameter.h>
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/GlobalLocalEnableSetting.h>

PhysicalControlSlider::PhysicalControlSlider(const Rect &rect)
    : SelectedParameterDotSlider(rect)
{
  auto settings = Application::get().getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->onChange(sigc::hide(sigc::mem_fun(this, &PhysicalControlSlider::setDirty)));
  settings->getSetting<RoutingSettings>()->onChange(sigc::hide(sigc::mem_fun(this, &PhysicalControlSlider::setDirty)));
}

PhysicalControlParameter* getPhysicalControlParameter(Parameter* p)
{
  return dynamic_cast<PhysicalControlParameter*>(p);
}

HardwareSourceSendParameter* getSendParameter(Parameter* p)
{
  return dynamic_cast<HardwareSourceSendParameter*>(p);
}

void PhysicalControlSlider::setParameter(Parameter *param)
{
  Slider::setParameter(param);
  onParameterSelectionChanged(param);
}

void PhysicalControlSlider::onParameterSelectionChanged(Parameter *p)
{
  nltools::Log::error("Selected:", p->getID().toString());

  auto hwSrc = getPhysicalControlParameter(p);
  auto hwSnd = getSendParameter(p);

  if(hwSrc != nullptr && hwSnd == nullptr)
    hwSnd = hwSrc->getSendParameter();

  if(hwSnd != nullptr && hwSrc == nullptr)
    hwSrc = hwSnd->getSiblingParameter();

  if(hwSnd && hwSrc)
  {
    m_hw = hwSrc;
    m_snd = hwSnd;

    m_hwParamConnection.disconnect();
    m_sndParamConnection.disconnect();

    m_hwParamConnection = m_hw->onParameterChanged(sigc::hide(sigc::mem_fun(this, &PhysicalControlSlider::setDirty)));
    m_sndParamConnection = m_snd->onParameterChanged(sigc::hide(sigc::mem_fun(this, &PhysicalControlSlider::setDirty)));
  }

  setDirty();
}

bool PhysicalControlSlider::redraw(FrameBuffer &fb)
{
  if(isLocalDisabledForThisParameter())
    return redrawLocalDisabled(fb);
  else
    return DotSlider::redraw(fb);
}

bool PhysicalControlSlider::redrawLocalDisabled(FrameBuffer &buffer)
{
  auto p = getParameter();
  auto hwSrc = getPhysicalControlParameter(p);
  auto hwSnd = getSendParameter(p);

  if(hwSrc != nullptr && hwSnd == nullptr)
    hwSnd = hwSrc->getSendParameter();

  if(hwSnd != nullptr && hwSrc == nullptr)
    hwSrc = hwSnd->getSiblingParameter();

  if(!hwSnd || !hwSrc)
    return false;

  const auto pos = getPosition();
  const auto left = Rect{pos.getLeft(), pos.getTop(), pos.getWidth() / 2, pos.getHeight()};
  const auto right = Rect{pos.getCenter().getX(), pos.getTop(), pos.getWidth() / 2, pos.getHeight()};

  DotSlider sliderSend(hwSnd, left);
  sliderSend.setValue(hwSnd->getValue().getQuantizedClipped(), hwSnd->isBiPolar());
  sliderSend.redraw(buffer);
  DotSlider sliderHW(hwSrc, right);
  sliderHW.setValue(hwSrc->getValue().getQuantizedClipped(), hwSrc->isBiPolar());
  sliderHW.redraw(buffer);
  return true;
}

bool PhysicalControlSlider::isLocalDisabledForThisParameter()
{
  if(auto hwParam = dynamic_cast<const PhysicalControlParameter*>(getParameter()))
  {
    return !hwParam->isLocalEnabled();
  }
  else if(auto sendParam = dynamic_cast<const HardwareSourceSendParameter*>(getParameter()))
  {
    return !sendParam->isLocalEnabled();
  }
  return false;
}

void PhysicalControlSlider::drawSliderBackground(FrameBuffer &fb)
{
  super::drawSliderBackground(fb);

  if(auto p = dynamic_cast<PhysicalControlParameter *>(getParameter()))
  {
    auto returnMode = p->getReturnMode();
    const auto &r = getPosition();
    const int centerMarkWidth = 6;

    switch(returnMode)
    {
      case ReturnMode::Center:
        fb.setColor(FrameBufferColors::C128);
        fb.fillRect(r.getCenter().getX() - centerMarkWidth / 2, r.getTop(), centerMarkWidth, r.getHeight());
        break;

      case ReturnMode::Zero:
        fb.setColor(FrameBufferColors::C128);
        fb.fillRect(r.getLeft(), r.getTop(), centerMarkWidth, r.getHeight());
        break;

      case ReturnMode::None:
      default:
        break;
    }
  }
}
