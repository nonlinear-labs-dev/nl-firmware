#include <tools/TextSplitter.h>
#include "ParameterNameLabel.h"
#include "Application.h"
#include <proxies/hwui/HWUI.h>
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"
#include "parameter_declarations.h"
#include "parameters/ModulationRoutingParameter.h"
#include "parameters/MacroControlParameter.h"
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>

ParameterNameLabel::ParameterNameLabel(const Rect &pos)
    : Label(pos)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &ParameterNameLabel::onParameterSelected)), vg);

  Application::get().getPresetManager()->getEditBuffer()->onPresetLoaded(
      sigc::mem_fun(this, &ParameterNameLabel::onPresetLoaded));

  Application::get().getPresetManager()->getEditBuffer()->onRecallValuesChanged(
      sigc::mem_fun(this, &ParameterNameLabel::onPresetLoaded));
}

ParameterNameLabel::~ParameterNameLabel()
{
}

void ParameterNameLabel::onParameterSelected(Parameter *param)
{
  if(param)
  {
    m_connection.disconnect();
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterNameLabel::onParameterChanged));
  }
}

Glib::ustring removeLastChar(const Glib::ustring &s)
{
  if(!s.empty())
    return s.substr(0, s.size() - 1);
  return s;
}

Glib::ustring addStar(const Glib::ustring &s)
{
  return s + "*";
}

void ParameterNameLabel::handleMCParameterName(const Parameter *pParameter)
{
  const auto changed = pParameter->isChangedFromLoaded();
  auto name = pParameter->getLongName();
  name = truncateMCName(changed, name);
  setText({ name, changed ? 1u : 0u });
}

void ParameterNameLabel::handleSendParameterName(const Parameter *p)
{
  const auto changed = p->isChangedFromLoaded();
  auto name = p->getShortName();
  setText({ name + (changed ? "*" : ""), changed ? 1u : 0u });
}

Glib::ustring ParameterNameLabel::truncateMCName(const bool changed, const Glib::ustring &name) const
{
  Glib::ustring ret = name;

  if(getFont()->getStringWidth(ret + (changed ? "*" : "")) > getWidth())
  {
    ret = removeLastChar(ret);

    while(getFont()->getStringWidth(Glib::ustring(ret + ".." + (changed ? "*" : ""))) > getWidth())
    {
      ret = removeLastChar(ret);
      if(ret.empty())
        break;
    }

    ret += "..";
  }

  if(changed)
    ret = addStar(ret);

  return ret;
}

void ParameterNameLabel::handleParameterName(const Parameter *pParameter)
{
  const auto changed = pParameter->isChangedFromLoaded();
  setText({ pParameter->getLongName() + (changed ? "*" : ""), changed ? 1u : 0u });
}

void ParameterNameLabel::onParameterChanged(const Parameter *param)
{
  auto id = param->getID().getNumber();

  if(id == C15::PID::MC_A || id == C15::PID::MC_B || id == C15::PID::MC_C || id == C15::PID::MC_D
     || id == C15::PID::MC_E || id == C15::PID::MC_F)
  {
    handleMCParameterName(param);
  }
  else if(id == C15::PID::Pedal_1_Send || id == C15::PID::Pedal_2_Send || id == C15::PID::Pedal_3_Send
          || id == C15::PID::Pedal_4_Send || id == C15::PID::Ribbon_1_Send || id == C15::PID::Ribbon_2_Send
          || id == C15::PID::Aftertouch_Send || id == C15::PID::Bender_Send)
  {
    handleSendParameterName(param);
  }
  else
  {
    handleParameterName(param);
  }
}

void ParameterNameLabel::setFontColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C255);
  else
    fb.setColor(FrameBufferColors::C128);
}

std::shared_ptr<Font> ParameterNameLabel::getFont() const
{
  return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());
}

int ParameterNameLabel::getFontHeight() const
{
  return 9;
}

void ParameterNameLabel::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C103);
}

void ParameterNameLabel::onPresetLoaded()
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  const auto param = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);
  onParameterChanged(param);
}

void ParameterNameLabelForMCOfModulationRouter::onParameterSelected(Parameter *param)
{
  if(auto modRouter = dynamic_cast<ModulationRoutingParameter*>(param))
  {
    ParameterNameLabel::onParameterSelected(modRouter->getTargetParameter());
  }
  else
  {
    ParameterNameLabel::onParameterSelected(param);
  }
}
