#include <proxies/hwui/descriptive-layouts/ConditionRegistry.h>
#include <parameters/ModulateableParameter.h>
#include "ParameterConditions.h"
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

using namespace DescriptiveLayouts;

namespace conditiondetail
{
  const EditBuffer &getEditBuffer()
  {
    return *Application::get().getPresetManager()->getEditBuffer();
  }

  const HWUI *getHWUI()
  {
    return Application::get().getHWUI();
  }

  const Parameter *getCurrentParameter()
  {
    return getEditBuffer().getSelected(Application::get().getVGManager()->getCurrentVoiceGroup());
  }

  const ModulateableParameter *getModulateableParameter()
  {
    return dynamic_cast<const ModulateableParameter *>(getCurrentParameter());
  }
}

bool ParameterConditions::IsParameterModulateable::check() const
{
  return conditiondetail::getModulateableParameter() != nullptr;
}

bool ParameterConditions::HasNoMcSelected::check() const
{
  if(auto parameter = conditiondetail::getModulateableParameter())
  {
    return parameter->getModulationSource() == MacroControls::NONE;
  }
  return true;
}

ParameterConditions::ParameterCondition::ParameterCondition()
{
  m_paramChangedConnection = Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &ParameterCondition::onParameterSelectionChanged),
      Application::get().getVGManager()->getCurrentVoiceGroup());
}

ParameterConditions::ParameterCondition::~ParameterCondition()
{
  m_paramChangedConnection.disconnect();
  m_paramConnection.disconnect();
}

void ParameterConditions::ParameterCondition::onParameterSelectionChanged(const Parameter *oldParam,
                                                                          Parameter *newParam)
{
  m_paramConnection.disconnect();

  if(newParam)
  {
    m_paramConnection = newParam->onParameterChanged(sigc::mem_fun(this, &ParameterCondition::onParameterChanged));
  }
}

void ParameterConditions::ParameterCondition::onParameterChanged(const Parameter *param)
{
  update();
}
