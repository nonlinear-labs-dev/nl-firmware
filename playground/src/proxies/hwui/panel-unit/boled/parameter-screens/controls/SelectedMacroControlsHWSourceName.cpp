#include "SelectedMacroControlsHWSourceName.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/MacroControlParameter.h"

SelectedMacroControlsHWSourceName::SelectedMacroControlsHWSourceName(const Rect &rect)
    : super(rect)
    , m_hwParamID{ ParameterId::invalid() }
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedMacroControlsHWSourceName::onParameterSelected)));
}

SelectedMacroControlsHWSourceName::~SelectedMacroControlsHWSourceName()
{
}

void SelectedMacroControlsHWSourceName::onParameterSelected(Parameter *newOne)
{
  m_mcChanged.disconnect();
  m_mcChanged = newOne->onParameterChanged(sigc::mem_fun(this, &SelectedMacroControlsHWSourceName::onMCChanged));
}

void SelectedMacroControlsHWSourceName::onMCChanged(const Parameter *param)
{
  if(auto mc = dynamic_cast<const MacroControlParameter *>(param))
  {
    auto hwSourceID = mc->getUiSelectedHardwareSource();

    if(m_hwParamID != hwSourceID)
    {
      m_hwParamID = hwSourceID;

      if(hwSourceID.getNumber() > 0)
      {
        if(auto hwParam = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
               { hwSourceID.getNumber(), VoiceGroup::Global }))
        {
          setText(hwParam->getLongName());
        }
      }
    }
  }
}
