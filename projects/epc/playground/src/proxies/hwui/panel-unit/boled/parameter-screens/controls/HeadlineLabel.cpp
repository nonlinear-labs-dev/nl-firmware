#include "HeadlineLabel.h"
#include <proxies/hwui/Oleds.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>

HeadlineLabel::HeadlineLabel(const Rect &pos)
    : super(pos)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onSelectionChanged(sigc::mem_fun(this, &HeadlineLabel::updateText), vg);
  eb->getSelected(vg)->onParameterChanged(mem_fun(this, &HeadlineLabel::updateMc));
}

std::shared_ptr<Font> HeadlineLabel::getFont() const
{
  return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());
}

int HeadlineLabel::getFontHeight() const
{
  return 9;
}

void HeadlineLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C255);
}

bool HeadlineLabel::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C77);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}

void HeadlineLabel::updateText(Parameter *oldParam, Parameter *newParam)
{
  if(newParam)
    setText(StringAndSuffix { newParam->getLongName() });
}

void HeadlineLabel::updateMc(const Parameter *param)
{
  auto mc = dynamic_cast<const MacroControlParameter *>(param);

  if(mc != nullptr)
  {
    setText(StringAndSuffix { mc->getLongName() });
  }
}
