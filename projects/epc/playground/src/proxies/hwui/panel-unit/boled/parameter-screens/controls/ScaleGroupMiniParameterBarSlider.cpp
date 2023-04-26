#include <groups/ScaleGroup.h>
#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ScaleGroupMiniParameterBarSlider.h>
#include "Application.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/FrameBuffer.h>

ScaleGroupMiniParameterBarSlider::ScaleGroupMiniParameterBarSlider(Parameter *param, const Rect &rect)
    : super(param, rect)
{
  m_connection
      = param->getParentGroup()->onGroupChanged(sigc::mem_fun(this, &ScaleGroupMiniParameterBarSlider::setDirty));
}

void ScaleGroupMiniParameterBarSlider::drawBackground(FrameBuffer &fb)
{
  if(isHighlight())
  {
    fb.setColor(FrameBufferColors::C179);
  }
  else
  {
    fb.setColor(FrameBufferColors::C77);
  }
  Slider::drawBackground(fb);
}

ScaleGroupMiniParameterBarSlider::~ScaleGroupMiniParameterBarSlider()
{
  m_connection.disconnect();
}

tDisplayValue ScaleGroupMiniParameterBarSlider::getDrawValue() const
{
  const auto id = ParameterId { C15::PID::Scale_Base_Key, VoiceGroup::Global };
  auto &eb = *Application::get().getPresetManager()->getEditBuffer();
  if(const auto param = eb.findParameterByID(id))
  {
    return param->getControlPositionValue();
  }
  return 0.0;
}

void ScaleGroupMiniParameterBarSlider::setSliderColor(FrameBuffer &fb)
{
  fb.setColor(FrameBufferColors::C255);
}

bool ScaleGroupMiniParameterBarSlider::redraw(FrameBuffer &fb)
{
  auto r = getPosition();
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C77);
  fb.fillRect(r.getX(), r.getTop(), r.getWidth(), r.getHeight());
  return BarSlider::redraw(fb);
}
