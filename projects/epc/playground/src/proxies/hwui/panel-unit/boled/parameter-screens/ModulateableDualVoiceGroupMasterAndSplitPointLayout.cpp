#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupMasterParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/SplitPointOverlapIndicator.h>
#include "ModulateableDualVoiceGroupMasterAndSplitPointLayout.h"
#include "SplitPointSyncIndicator.h"
#include <proxies/hwui/controls/Slider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SplitPointEditMenu.h>

Carousel *ModulateableDualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *ModulateableDualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

Carousel *UnmodulateableDualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *UnmodulateableDualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

SplitPointParameterLayout::SplitPointParameterLayout()
    : ModulateableDualVoiceGroupMasterAndSplitPointLayout()
{
  addControl(new SplitPointOverlapIndicator({ BIG_SLIDER_X - 4, 23, 1, 8 }));
  addControl(new SplitPointSyncIndicator({ BIG_SLIDER_X + BIG_SLIDER_WIDTH + 1, 23, 9, 8 }));
}

SplitPointParameterEditLayout::SplitPointParameterEditLayout()
{
}

ButtonMenu *SplitPointParameterEditLayout::createMenu(const Rect &rect)
{
  return new SplitPointEditMenu(rect);
}

ModuleCaption *SplitPointParameterEditLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}
