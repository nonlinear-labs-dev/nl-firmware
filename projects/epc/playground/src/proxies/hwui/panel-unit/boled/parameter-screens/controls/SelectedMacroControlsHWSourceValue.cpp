#include "SelectedMacroControlsHWSourceValue.h"
#include <proxies/hwui/HWUI.h>
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/ModulationRoutingParameter.h"
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>

SelectedMacroControlsHWSourceValue::SelectedMacroControlsHWSourceValue(const Rect &rect)
    : super(rect)
    , m_hwParamID { ParameterId::invalid() }
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onParameterSelected)), vg);

  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onModifiersChanged)));
}

SelectedMacroControlsHWSourceValue::~SelectedMacroControlsHWSourceValue() = default;

void SelectedMacroControlsHWSourceValue::onParameterSelected(Parameter *newOne)
{
  m_mcChanged.disconnect();
  if(auto mc = dynamic_cast<MacroControlParameter*>(newOne))
  {
    m_mcChanged = newOne->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onMCChanged));
  }
  else if(auto modRouter = dynamic_cast<ModulationRoutingParameter*>(newOne))
  {
    auto mc = modRouter->getTargetParameter();
    m_mcChanged = mc->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onMCChanged));
  }
}

ParameterId SelectedMacroControlsHWSourceValue::getHWSourceID(const Parameter *param) const
{
  if(auto mc = dynamic_cast<const MacroControlParameter *>(param))
    return mc->getUiSelectedHardwareSource();
  return ParameterId::invalid();
}

void SelectedMacroControlsHWSourceValue::onMCChanged(const Parameter *param)
{
  auto hwSourceID = getHWSourceID(param);

  if(m_hwParamID != hwSourceID)
  {
    m_hwParamID = hwSourceID;
    m_hwChanged.disconnect();

    if(hwSourceID.getNumber() > 0)
    {
      if(auto hwParam = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(hwSourceID))
      {
        m_hwChanged = hwParam->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::updateText));
      }
    }
  }
}

void SelectedMacroControlsHWSourceValue::updateText(const Parameter *param)
{
  if(!param)
  {
    setText("");
    return;
  }

  auto str = param->getDisplayString();

  if(isHighlight() && Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE))
  {
    setText({ str, " F" });
  }
  else
  {
    setText(str);
  }
}

void SelectedMacroControlsHWSourceValue::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C103);
}

void SelectedMacroControlsHWSourceValue::onModifiersChanged()
{
  updateText(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(m_hwParamID));
}

SelectedModRouterMacroControlValue::SelectedModRouterMacroControlValue(const Rect &rect)
    : Label(rect)
    , m_mcId(ParameterId::invalid())
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedModRouterMacroControlValue::onParameterSelected)), vg);

  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &SelectedModRouterMacroControlValue::onModifiersChanged)));
}

SelectedModRouterMacroControlValue::~SelectedModRouterMacroControlValue()
{
}

void SelectedModRouterMacroControlValue::onParameterSelected(Parameter *newOne)
{
  m_mcChanged.disconnect();
  if(auto mc = dynamic_cast<MacroControlParameter*>(newOne))
  {
    m_mcChanged = newOne->onParameterChanged(sigc::mem_fun(this, &SelectedModRouterMacroControlValue::updateText));
    m_mcId = mc->getID();
  }
  else if(auto modRouter = dynamic_cast<ModulationRoutingParameter*>(newOne))
  {
    auto mc = modRouter->getTargetParameter();
    m_mcChanged = mc->onParameterChanged(sigc::mem_fun(this, &SelectedModRouterMacroControlValue::updateText));
    m_mcId = mc->getID();
  }
}

void SelectedModRouterMacroControlValue::onModifiersChanged()
{
  updateText(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(m_mcId));
}

void SelectedModRouterMacroControlValue::updateText(const Parameter *param)
{
  if(!param)
  {
    setText("");
    return;
  }

  auto str = param->getDisplayString();

  if(isHighlight() && Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE))
  {
    setText({ str, " F" });
  }
  else
  {
    setText(str);
  }
}

void SelectedModRouterMacroControlValue::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C103);
}