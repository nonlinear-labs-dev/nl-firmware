#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupMasterParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include "DualVoiceGroupMasterAndSplitPointLayout.h"

DualVoiceGroupMasterAndSplitPointLayout::DualVoiceGroupMasterAndSplitPointLayout()
    : ModulateableParameterSelectLayout2()
{
}

DualVoiceGroupMasterAndSplitPointLayout::~DualVoiceGroupMasterAndSplitPointLayout()
{
  m_connectionEditBuffer.disconnect();
  m_connectionVoiceGroup.disconnect();
}

Carousel *DualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *DualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}
