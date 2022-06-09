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
#include <sigc++/adaptors/hide.h>
#include "device-settings/Settings.h"
#include "device-settings/SplitPointSyncParameters.h"
#include "use-cases/EditBufferUseCases.h"
#include "use-cases/SettingsUseCases.h"

Carousel *ModulateableDualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *ModulateableDualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

bool ModulateableDualVoiceGroupMasterAndSplitPointLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(auto modP = dynamic_cast<ModulateableParameter*>(getCurrentParameter()))
  {
    if(modP->getModulationSource() == MacroControls::NONE)
    {
      if(i == Buttons::BUTTON_C && down)
      {
        SettingsUseCases sus(*Application::get().getSettings());
        sus.setFocusAndMode({UIFocus::Sound, UIMode::Select, UIDetail::Init});
        return true;
      }
    }
  }

  return ModulateableParameterSelectLayout2::onButton(i, down, modifiers);
}

Carousel *UnmodulateableDualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *UnmodulateableDualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

bool UnmodulateableDualVoiceGroupMasterAndSplitPointLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(i == Buttons::BUTTON_C && down)
  {
    SettingsUseCases sus(*Application::get().getSettings());
    sus.setFocusAndMode({UIFocus::Sound, UIMode::Select, UIDetail::Init});
    return true;
  }
  return UnmodulateableParameterSelectLayout2::onButton(i, down, modifiers);
}

SplitPointParameterLayout::SplitPointParameterLayout()
    : ModulateableDualVoiceGroupMasterAndSplitPointLayout()
{
  setMode(Mode::ParameterValue);

  auto sync = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  m_connection = sync->onChange([this](const Setting *s) { fixValueControl(); });
}

SplitPointParameterLayout::~SplitPointParameterLayout()
{
  m_connection.disconnect();
}

SplitPointParameterEditLayout::SplitPointParameterEditLayout()
{
  auto sync = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  m_connection = sync->onChange([this](const Setting *s) { fixValueControl(); });
}

void SplitPointParameterLayout::setMode(ModulateableParameterSelectLayout2::Mode desiredMode)
{
  ModulateableParameterSelectLayout2::setMode(desiredMode);
  if(desiredMode == Mode::ParameterValue)
  {
    fixValueControl();
  }
  else
  {
    if(auto selectedSynced = findControlOfType<SelectedParameterValue>())
      remove(selectedSynced.get());

    if(auto selectedNonSynced = findControlOfType<SplitParameterValue>())
      remove(selectedNonSynced.get());
  }
}

Control *SplitPointParameterLayout::createParameterValueControl()
{
  auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();

  if(!setting->get() && getMode() == Mode::ParameterValue)
    return new SplitParameterValue(Rect(90, 33, 76, 12));
  else
    return ParameterLayout2::createParameterValueControl();
}

void SplitPointParameterLayout::fixValueControl()
{
  auto selectedSynced = findControlOfType<SelectedParameterValue>();
  auto selectedNonSynced = findControlOfType<SplitParameterValue>();

  if(selectedSynced)
    remove(selectedSynced.get());

  if(selectedNonSynced)
    remove(selectedNonSynced.get());

  addControl(createParameterValueControl());

  setDirty();
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

  setDirty();
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
  m_connection.disconnect();
}
