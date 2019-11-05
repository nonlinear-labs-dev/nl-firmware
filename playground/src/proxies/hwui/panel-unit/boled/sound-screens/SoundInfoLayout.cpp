#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/HeadlineLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/InfoLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModuleCaption.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/SoundInfoContent.h>
#include <proxies/hwui/controls/LabelRegular8.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include "SoundInfoLayout.h"

SoundInfoLayout::SoundInfoLayout()
    : InfoLayout()
{
}

struct SoundInfoModuleCaption : public InvertedLabel
{
 public:
  explicit SoundInfoModuleCaption(const Rect& r)
      : InvertedLabel("Sound", r)
  {
  }
};

struct HeadlinePlaceholder : public Control
{
  HeadlinePlaceholder(const Rect& r)
      : Control(r)
  {
  }

  bool redraw(FrameBuffer& fb) override
  {
    fb.setColor(FrameBuffer::Colors::C77);
    fb.fillRect(getPosition());
    return true;
  }
};

void SoundInfoLayout::addModuleCaption()
{
  addControl(new SoundInfoModuleCaption(Rect(0, 0, 64, 13)));
}

void SoundInfoLayout::addHeadline()
{
  addControl(new HeadlinePlaceholder(Rect(64, 0, 128, 13)));
}

void SoundInfoLayout::addInfoLabel()
{
  addControl(new InfoLabel(Rect(192, 0, 64, 13)));
}

Scrollable* SoundInfoLayout::createScrollableContent()
{
  return new SoundInfoContent();
}
