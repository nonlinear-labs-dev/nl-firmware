#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>

SelectedParameterValue::SelectedParameterValue(const Rect &rect)
    : super(rect)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedParameterValue::onParameterSelected)));

  Application::get().getHWUI()->onModifiersChanged(sigc::mem_fun(this, &SelectedParameterValue::onModifiersChanged));

  m_voiceGroupSelectionConnection = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &SelectedParameterValue::onVoiceGroupSelectionChanged));
}

SelectedParameterValue::~SelectedParameterValue()
{
  m_voiceGroupSelectionConnection.disconnect();
}

void SelectedParameterValue::onModifiersChanged(ButtonModifiers mods)
{
  onParamValueChanged(Application::get().getPresetManager()->getEditBuffer()->getSelected());
}

void SelectedParameterValue::onParameterSelected(Parameter *parameter)
{
  m_paramValueConnection.disconnect();

  if(parameter)
    m_paramValueConnection
        = parameter->onParameterChanged(sigc::mem_fun(this, &SelectedParameterValue::onParamValueChanged));
}

void SelectedParameterValue::onParamValueChanged(const Parameter *param)
{
  setDirty();
}

bool SelectedParameterValue::redraw(FrameBuffer &fb)
{
  auto amount = Application::get().getPresetManager()->getEditBuffer()->getSelected()->getDisplayString();

  if(Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE))
  {
    setText({ amount, " F" });
  }
  else
  {
    setText(amount);
  }

  return super::redraw(fb);
}

void SelectedParameterValue::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C128);
}

void SelectedParameterValue::onVoiceGroupSelectionChanged(VoiceGroup v)
{
  setDirty();
}
