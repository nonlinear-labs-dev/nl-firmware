#include <proxies/hwui/panel-unit/boled/preset-screens/controls/SoundInfoContent.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/HeadlineLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/InfoLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModuleCaption.h>
#include "SoundInfoLayout.h"

SoundInfoLayout::SoundInfoLayout() : InfoLayout()
{
}

void SoundInfoLayout::addModuleCaption()
{
  addControl(new ModuleCaption(Rect(0, 0, 64, 13)));
}

void SoundInfoLayout::addHeadline()
{
  addControl(new HeadlineLabel(Rect(64, 0, 128, 13)));
}

void SoundInfoLayout::addInfoLabel()
{
  addControl(new InfoLabel(Rect(192, 0, 64, 13)));
}

Scrollable *SoundInfoLayout::createScrollableContent()
{
  return new SoundInfoContent();
}
