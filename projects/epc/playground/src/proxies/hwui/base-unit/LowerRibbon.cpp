#include "Application.h"
#include "LowerRibbon.h"
#include "parameters/Parameter.h"
#include <parameters/RibbonParameter.h>
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"
#include <groups/HardwareSourcesGroup.h>
#include <glib.h>
#include <cmath>
#include <nltools/messaging/Message.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

static Parameter *getParameter()
{
  return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
      HardwareSourcesGroup::getLowerRibbonParameterID());
}

LowerRibbon::LowerRibbon()
{
  initLEDs();
  getParameter()->onParameterChanged(sigc::mem_fun(this, &LowerRibbon::onParamValueChanged));
  nltools::msg::receive<nltools::msg::UpdateLocalDisabledRibbonValue>(
      nltools::msg::EndPoint::Playground, sigc::mem_fun(this, &LowerRibbon::onRibbonValueMessage));
}

int LowerRibbon::posToLedID(int pos) const
{
  return pos * 2;
}

void LowerRibbon::onParamValueChanged(const Parameter *param)
{
  auto ribbonParameter = dynamic_cast<const RibbonParameter *>(param);
  auto ribbonMode = ribbonParameter->getRibbonReturnMode();

  bool bipol = ribbonMode == RibbonReturnMode::RETURN;

  auto paramVal = param->getControlPositionValue();

  if(!bipol)
  {
    setLEDsForValueUniPolar(paramVal);
  }
  else
  {
    setLEDsForValueBiPolar(paramVal);
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

void LowerRibbon::onRibbonValueMessage(const nltools::msg::UpdateLocalDisabledRibbonValue &msg)
{
  static auto playcontroller = Application::get().getPlaycontrollerProxy();

  if(msg.ribbonId == nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Ribbon2)
  {
    playcontroller->notifyRibbonTouch(HardwareSourcesGroup::getLowerRibbonParameterID().getNumber());
    setLEDsForValueUniPolar(msg.position);
  }
}
