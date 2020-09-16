#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include "ScreenSaverTimeControls.h"
#include <proxies/hwui/FrameBuffer.h>

//View
ScreenSaverTimeView::ScreenSaverTimeView()
    : SetupLabel({ 0, 0, 0, 0 })
{
  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->onChange(
      mem_fun(this, &ScreenSaverTimeView::onSettingChanged));
}

void ScreenSaverTimeView::onSettingChanged(const Setting* s)
{
  setText({ s->getDisplayString(), 0 });
}

//Editor
ScreenSaverTimeEditor::ScreenSaverTimeEditor()
    : super(Rect(0, 0, 0, 0))
{

  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->onChange(
      mem_fun(this, &ScreenSaverTimeEditor::onSettingChanged));
}

ScreenSaverTimeEditor::~ScreenSaverTimeEditor()
{
}

void ScreenSaverTimeEditor::onSettingChanged(const Setting* s)
{
  if(auto p = dynamic_cast<const ScreenSaverTimeoutSetting*>(s))
    setText(p->getDisplayString());
}

bool ScreenSaverTimeEditor::onRotary(int inc, ButtonModifiers modifiers)
{
  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->incDec(inc);
  return true;
}

void ScreenSaverTimeEditor::setFontColor(FrameBuffer& fb) const
{
  fb.setColor(FrameBufferColors::C255);
}

Font::Justification ScreenSaverTimeEditor::getJustification() const
{
  return Font::Justification::Left;
}

void ScreenSaverTimeEditor::setBackgroundColor(FrameBuffer& fb) const
{
  fb.setColor(FrameBufferColors::C103);
}

bool ScreenSaverTimeEditor::redraw(FrameBuffer& fb)
{
  auto ret = SetupLabel::redraw(fb);
  fb.setColor(FrameBufferColors::C179);
  fb.drawRect(getPosition());
  return ret;
}
