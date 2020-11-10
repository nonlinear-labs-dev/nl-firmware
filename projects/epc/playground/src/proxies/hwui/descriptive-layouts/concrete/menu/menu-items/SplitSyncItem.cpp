#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include "SplitSyncItem.h"
#include "LeftAligned9pxCaptionLabel.h"
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/CaptionLabel.h>

void SplitSyncItem::doAction()
{
  auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  setting->setState(!setting->get());
}

SplitSyncItem::SplitSyncItem(const Rect& r)
    : BasicItemWithValueText("Sync Split", "", r)
{

  m_connection = Application::get().getSettings()->getSetting<SplitPointSyncParameters>()->onChange(
      sigc::mem_fun(this, &SplitSyncItem::onSettingChanged));

  m_enterLabel->setText({ "" });
}

void SplitSyncItem::onSettingChanged(const Setting* s)
{
  if(auto split = dynamic_cast<const SplitPointSyncParameters*>(s))
  {
    if(split->get())
    {
      m_valueLabel->setText("Enabled");
    }
    else
    {
      m_valueLabel->setText("Disabled");
    }
  }
}

bool SplitSyncItem::redraw(FrameBuffer& fb)
{
  return BasicItem::redraw(fb) | drawEnterIndication(fb);
}
