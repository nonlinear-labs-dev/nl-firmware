#include "Slider.h"
#include "parameters/Parameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

Slider::Slider(Parameter *param, const Rect &rect)
    : super(rect)
    , m_value(0)
    , m_bipolar(false)
{
  m_voiceGroupSelectionConnection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(
      sigc::mem_fun(this, &Slider::voiceGroupSelectionChanged));

  setParameter(param);
}

Slider::Slider(const Rect &rect)
    : Slider(nullptr, rect)
{
}

Slider::~Slider()
{
}

void Slider::voiceGroupSelectionChanged()
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto vgSelection = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  if(auto param = getParameter())
  {
    auto id = param->getID();
    auto vg = param->getVoiceGroup();

    if(auto normalParameter = editBuffer->findParameterByID(id, vgSelection))
      setParameter(normalParameter);
    else if(auto globalParameter = editBuffer->findParameterByID(id, VoiceGroup::Global))
      setParameter(globalParameter);
  }
}

void Slider::setParameter(Parameter *param)
{
  if(m_param != param)
  {
    m_param = param;

    m_paramValueConnection.disconnect();

    if(param)
      m_paramValueConnection = param->onParameterChanged(sigc::mem_fun(this, &Slider::onParamValueChanged));
    else
      setValue(0, false);

    setDirty();
  }
}

Parameter *Slider::getParameter() const
{
  return m_param;
}

void Slider::onParamValueChanged(const Parameter *param)
{
  setValue(param->getControlPositionValue(), param->isBiPolar());
}

void Slider::setValue(tDisplayValue v, bool bipolar)
{
  if(m_value != v || m_bipolar != bipolar)
  {
    m_value = v;
    m_bipolar = bipolar;
    setDirty();
  }
}

tDisplayValue Slider::getValue() const
{
  return m_value;
}

void Slider::setSliderColor(FrameBuffer &fb)
{
  if(isHighlight())
    fb.setColor(FrameBuffer::Colors::C255);
  else
    fb.setColor(FrameBuffer::Colors::C103);
}

bool Slider::hasBorder() const
{
  return true;
}

bool Slider::isBiPolar() const
{
  return m_bipolar;
}
