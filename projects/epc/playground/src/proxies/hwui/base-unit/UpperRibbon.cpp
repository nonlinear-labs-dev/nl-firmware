#include "Application.h"
#include "UpperRibbon.h"
#include "parameters/Parameter.h"
#include <parameters/RibbonParameter.h>
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"
#include "device-settings/BaseUnitUIMode.h"
#include "device-settings/Settings.h"
#include "groups/HardwareSourcesGroup.h"
#include <proxies/hwui/HWUI.h>
#include <nltools/messaging/Message.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

UpperRibbon::UpperRibbon()
{
  initLEDs();

  m_parameterSelectionChangedSignal = Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &UpperRibbon::onParamSelectionChanged), {});
  m_settingChangedSignal = Application::get().getSettings()->getSetting<BaseUnitUIMode>()->onChange(
      sigc::mem_fun(this, &UpperRibbon::onSettingChanged));
}

UpperRibbon::~UpperRibbon()
{
  m_parameterSelectionChangedSignal.disconnect();
  m_settingChangedSignal.disconnect();
}

void UpperRibbon::onParamSelectionChanged(Parameter* oldOne, Parameter* newOne)
{
  reconnect();
}

void UpperRibbon::onSettingChanged(const Setting* setting)
{
  reconnect();
}

Parameter* getSendParameter()
{
  return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(HardwareSourcesGroup::getRibbon1SendID());
}

void UpperRibbon::reconnect()
{
  m_paramConnection.disconnect();

  if(auto p = getResponsibleParameter())
    m_paramConnection = p->onParameterChanged(sigc::mem_fun(this, &UpperRibbon::onParamValueChanged));

  if(auto p = getSendParameter())
    m_sendConnection = p->onParameterChanged(sigc::mem_fun(this, &UpperRibbon::onSendValueChanged));
}

Parameter* UpperRibbon::getResponsibleParameter()
{
  auto s = Application::get().getSettings()->getSetting<BaseUnitUIMode>();

  if(s->get() == BaseUnitUIModes::ParameterEdit)
    return Application::get().getPresetManager()->getEditBuffer()->getSelected(
        Application::get().getHWUI()->getCurrentVoiceGroup());

  return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
      HardwareSourcesGroup::getUpperRibbonParameterID());
}

void UpperRibbon::onParamValueChanged(const Parameter* param)
{
  bool bipol = false;

  auto paramVal = param->getControlPositionValue();
  auto s = Application::get().getSettings()->getSetting<BaseUnitUIMode>();

  if(s->get() == BaseUnitUIModes::ParameterEdit)
  {
    bipol = param->isBiPolar();
  }
  else  // BASE_UNIT_UI_MODE_PLAY
  {
    if(auto ribbonParameter = dynamic_cast<const RibbonParameter*>(param))
    {
      bipol = ribbonParameter->getRibbonReturnMode() == RibbonReturnMode::RETURN;
    }
  }

  if(!bipol)
    setLEDsForValueUniPolar(paramVal);
  else
    setLEDsForValueBiPolar(paramVal);
}

void UpperRibbon::onSendValueChanged(const Parameter* param)
{
  auto sendParam = dynamic_cast<const HardwareSourceSendParameter*>(param);
  auto editParam = getResponsibleParameter();

  if(!sendParam->isLocalEnabled())
  {
    bool bipol = false;

    tControlPositionValue paramVal = 0;
    auto s = Application::get().getSettings()->getSetting<BaseUnitUIMode>();

    if(s->get() == BaseUnitUIModes::ParameterEdit)
    {
      bipol = editParam->isBiPolar();
      paramVal = editParam->getControlPositionValue();
    }
    else  // BASE_UNIT_UI_MODE_PLAY
    {
      bipol = sendParam->getReturnMode() == ReturnMode::Center;
      paramVal = sendParam->getControlPositionValue();
    }

    if(!bipol)
      setLEDsForValueUniPolar(paramVal);
    else
      setLEDsForValueBiPolar(paramVal);
  }
}

int UpperRibbon::posToLedID(int pos) const
{
  return 2 * pos + 1;
}