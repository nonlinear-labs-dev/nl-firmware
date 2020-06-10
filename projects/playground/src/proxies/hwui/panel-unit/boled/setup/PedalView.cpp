#include <Application.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/PedalType.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalView.h>
#include <proxies/hwui/FrameBuffer.h>

PedalView::PedalView(std::shared_ptr<PedalType> s)
    : base_type("", Rect(0, 0, 0, 0))
{
  s->onChange(mem_fun(this, &PedalView::onSettingChanged));
}

PedalView::~PedalView()
{
}

bool PedalView::redraw(FrameBuffer &fb)
{
  auto ret = SetupLabel::redraw(fb);
  //Draw enter dots indication
  setFontColor(fb);
  fb.drawHorizontalLine(getPosition().getRight() - 5, getPosition().getBottom() - 3, 5);
  fb.setColor(FrameBufferColors::C43);
  fb.drawVerticalLine(getPosition().getRight() - 4, getPosition().getBottom() - 3, 1);
  fb.drawVerticalLine(getPosition().getRight() - 2, getPosition().getBottom() - 3, 1);
  return ret;
}

void PedalView::onSettingChanged(const Setting *s)
{
  auto v = dynamic_cast<const PedalType *>(s);
  setText(v->getDisplayString());
}
