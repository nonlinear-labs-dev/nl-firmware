#include "Application.h"
#include "LowerRibbon.h"
#include "parameters/Parameter.h"
#include <parameters/RibbonParameter.h>
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"
#include "device-settings/BaseUnitUIMode.h"
#include "device-settings/SelectedRibbonsSetting.h"
#include <groups/HardwareSourcesGroup.h>
#include <glib.h>
#include <cmath>
#include <nltools/messaging/Message.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

static Parameter *getParameter()
{
  if(Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->get() == SelectedRibbons::Ribbon1_2)
  {
    return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
        HardwareSourcesGroup::getLowerRibbonParameterID());
  }
  else
  {
    return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
        HardwareSourcesGroup::getLowerRibbon4ParameterID());
  }
}

static Parameter *getSendParameter()
{
  if(Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->get() == SelectedRibbons::Ribbon1_2)
  {
    return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
        HardwareSourcesGroup::getRibbon2SendID());
  }
  else
  {
    return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
        HardwareSourcesGroup::getRibbon4SendID());
  }
}

LowerRibbon::LowerRibbon()
{
  initLEDs();

  m_parameterSelectionChangedSignal = Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &LowerRibbon::onParamSelectionChanged), {});
}

void LowerRibbon::reconnect()
{
  if(auto p = getParameter())
    m_paramConnection = p->onParameterChanged(sigc::mem_fun(this, &LowerRibbon::onParamValueChanged));
  if(auto p = getSendParameter())
    m_sendConnection = p->onParameterChanged(sigc::mem_fun(this, &LowerRibbon::onSendValueChanged));
}

int LowerRibbon::posToLedID(int pos) const
{
  return pos * 2;
}

void LowerRibbon::onParamSelectionChanged(const Parameter *old, const Parameter *newP)
{
  reconnect();
}

void LowerRibbon::onParamValueChanged(const Parameter *param)
{
  auto ribbonParameter = dynamic_cast<const RibbonParameter *>(param);
  auto sendParameter = ribbonParameter->getSendParameter();
  auto ribbonMode = ribbonParameter->getRibbonReturnMode();

  bool bipol = ribbonMode == RibbonReturnMode::RETURN;

  auto paramVal = param->getControlPositionValue();

  if(sendParameter->isLocalEnabled())
  {
    if(!bipol)
    {
      setLEDsForValueUniPolar(paramVal);
    }
    else
    {
      setLEDsForValueBiPolar(paramVal);
    }
  }
}

void LowerRibbon::onSendValueChanged(const Parameter *param)
{
  auto sendParameter = dynamic_cast<const HardwareSourceSendParameter *>(param);
  auto mode = sendParameter->getReturnMode();

  bool bipol = mode == ReturnMode::Center;

  auto paramVal = param->getControlPositionValue();

  if(!sendParameter->isLocalEnabled())
  {
    if(!bipol)
    {
      setLEDsForValueUniPolar(paramVal);
    }
    else
    {
      setLEDsForValueBiPolar(paramVal);
    }
  }
}

void LowerRibbon::indicateBlockingMainThread(bool onOff)
{
  m_indicateBlockingMainThread = onOff;

  if(m_indicateBlockingMainThread)
  {
    for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
    {
      setLEDState(i, (i % 2 == 0) ? FourStateLED::State::Bright : FourStateLED::State::Off);
    }
  }
  else
  {
    onParamValueChanged(getParameter());
  }
}
