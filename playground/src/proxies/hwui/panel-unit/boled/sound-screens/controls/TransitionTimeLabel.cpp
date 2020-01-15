#include <Application.h>
#include <device-settings/TransitionTime.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/TransitionTimeLabel.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>

TransitionTimeLabel::TransitionTimeLabel(const Rect &rect)
    : super(rect)
{
  Application::get().getSettings()->getSetting<TransitionTime>()->onChange(
      sigc::mem_fun(this, &TransitionTimeLabel::onSettingChanged));
  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &TransitionTimeLabel::onModifiersChanged)));
}

void TransitionTimeLabel::onSettingChanged(const Setting *s)
{
  auto displayString = (((const TransitionTime *) s)->getDisplayString());

  if(isHighlight() && Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE))
  {
    setText({ displayString, " F" });
  }
  else
  {
    setText(displayString);
  }
}

void TransitionTimeLabel::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C103);
}

void TransitionTimeLabel::onModifiersChanged()
{
  onSettingChanged(Application::get().getSettings()->getSetting<TransitionTime>().get());
}
