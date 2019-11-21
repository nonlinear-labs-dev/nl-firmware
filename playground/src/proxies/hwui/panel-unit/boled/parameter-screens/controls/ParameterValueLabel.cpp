#include "ParameterValueLabel.h"
#include <parameters/Parameter.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

ParameterValueLabel::ParameterValueLabel(const Parameter *param, const Rect &pos)
    : Label(pos)
{
  if(param)
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterValueLabel::updateText));

  m_vgSelectionConnection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(
      sigc::mem_fun(this, &ParameterValueLabel::updateVoiceGroup));
}

ParameterValueLabel::~ParameterValueLabel()
{
  m_connection.disconnect();
  m_vgSelectionConnection.disconnect();
}

void ParameterValueLabel::updateText(const Parameter *param)
{
  setText(param->getDisplayString());
}

void ParameterValueLabel::updateVoiceGroup()
{
  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  updateText(Application::get().getPresetManager()->getEditBuffer()->getSelected(vg));
}

void ParameterValueLabel::updateParameter(const Parameter *param)
{
  m_connection.disconnect();

  if(param)
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterValueLabel::updateText));
}
