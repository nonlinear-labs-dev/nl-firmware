#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupMasterParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include "ModulateableDualVoiceGroupMasterAndSplitPointLayout.h"
#include <proxies/hwui/controls/Slider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SplitPointEditMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SplitParameterValue.h>
#include <Application.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include "device-settings/Settings.h"
#include "device-settings/SplitPointSyncParameters.h"

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
  setMode(Mode::ParameterValue);
}

SplitPointParameterEditLayout::SplitPointParameterEditLayout()
{
  m_settingConnection = Application::get().getSettings()->getSetting<SplitPointSyncParameters>()->onChange(
      [this](const Setting *s) { fixValueControl(); });
}

void SplitPointParameterEditLayout::fixValueControl()
{
  auto selectedSynced = findControlOfType<SelectedParameterValue>();
  auto selectedNonSynced = findControlOfType<SplitParameterValue>();

  if(selectedSynced)
    remove(selectedSynced.get());

  if(selectedNonSynced)
    remove(selectedNonSynced.get());

  addControl(createParameterValueControl());
}

Control *SplitPointParameterLayout::createParameterValueControl()
{
  auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();

  if(!setting->get())
    return new SplitParameterValue(Rect(90, 33, 76, 12));
  else
    return ParameterLayout2::createParameterValueControl();
}

ButtonMenu *SplitPointParameterEditLayout::createMenu(const Rect &rect)
{
  return new SplitPointEditMenu(rect);
}

ModuleCaption *SplitPointParameterEditLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

Control *SplitPointParameterEditLayout::createParameterValueControl()
{
  auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();

  if(!setting->get())
    return new SplitParameterValue(Rect(90, 33, 76, 12));
  else
    return ParameterLayout2::createParameterValueControl();
}

SplitPointParameterEditLayout::~SplitPointParameterEditLayout()
{
  m_settingConnection.disconnect();
}
