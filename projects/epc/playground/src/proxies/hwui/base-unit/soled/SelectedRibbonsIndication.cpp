#include "SelectedRibbonsIndication.h"
#include "Application.h"
#include "device-settings/Settings.h"
#include <device-settings/SelectedRibbonsSetting.h>
#include <proxies/hwui/FrameBuffer.h>

SelectedRibbonsIndication::SelectedRibbonsIndication(const Rect& pos)
    : Control(pos)
{
  Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &SelectedRibbonsIndication::onSettingChanged));
}

void SelectedRibbonsIndication::onSettingChanged(const Setting* s)
{
  if(auto setting = dynamic_cast<const SelectedRibbonsSetting*>(s))
  {
    m_isRibbon1_2_selected = setting->get() == SelectedRibbons::Ribbon1_2;
  }
}

bool SelectedRibbonsIndication::redraw(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C43);
  fb.fillRect(getPosition());

  fb.setColor(FrameBufferColors::C255);

  auto l = getPosition().getLeft();
  auto top = getPosition().getTop();

  fb.drawHorizontalLine(l + 0, top + 0, 9);
  fb.drawHorizontalLine(l + 0, top + 4, 9);
  fb.drawHorizontalLine(l + 0, top + 8, 9);
  fb.drawHorizontalLine(l + 0, top + 12, 9);

  if(m_isRibbon1_2_selected)
  {
     fb.drawVerticalLine(l + 11, top + 0, 5);
  }
  else
  {
    fb.drawVerticalLine(l + 11, top + 8, 5);
  }

  return Control::redraw(fb);
}
