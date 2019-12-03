#include "SelectedMacroControlsHWSourceValue.h"
#include <proxies/hwui/HWUI.h>
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/MacroControlParameter.h"

SelectedMacroControlsHWSourceValue::SelectedMacroControlsHWSourceValue(const Rect &rect)
    : super(rect)
    , m_hwParamID{ ParameterId::invalid() }
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onParameterSelected)));

  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onModifiersChanged)));
}

SelectedMacroControlsHWSourceValue::~SelectedMacroControlsHWSourceValue() = default;

void SelectedMacroControlsHWSourceValue::onParameterSelected(Parameter *newOne)
{
  m_mcChanged.disconnect();
  m_mcChanged = newOne->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::onMCChanged));
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
      if(auto hwParam = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
             { hwSourceID.getNumber(), VoiceGroup::Global }))
      {
        m_hwChanged = hwParam->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceValue::updateText));
      }
    }
  }
}

void SelectedMacroControlsHWSourceValue::updateText(const Parameter *param)
{
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
  fb.setColor(FrameBuffer::Colors::C103);
}

void SelectedMacroControlsHWSourceValue::onModifiersChanged()
{
  updateText(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(m_hwParamID));
}
