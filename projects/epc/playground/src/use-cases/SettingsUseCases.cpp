#include "SettingsUseCases.h"
#include "device-settings/ScreenSaverTimeoutSetting.h"
#include <device-settings/Settings.h>
#include <device-settings/midi/mappings/PedalCCMapping.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <device-settings/midi/mappings/AftertouchCCMapping.h>
#include <device-settings/midi/mappings/BenderCCMapping.h>
#include <device-settings/midi/mappings/Enable14BitSupport.h>
#include <device-settings/midi/mappings/EnableHighVelocityCC.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <nltools/messaging/Message.h>
#include <device-settings/Passphrase.h>
#include <device-settings/SyncSplitSettingUseCases.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <presets/PresetManager.h>

SettingsUseCases::SettingsUseCases(Settings& s)
    : m_settings { s }
    , m_focusAndModeSetting { *s.getSetting<FocusAndModeSetting>() }
{
}

void SettingsUseCases::setMappingsToHighRes()
{
  static auto fourteenBitSupport = m_settings.getSetting<Enable14BitSupport>();
  static auto velCC = m_settings.getSetting<EnableHighVelocityCC>();
  static auto p1 = m_settings.getSetting<PedalCCMapping<1>>();
  static auto p2 = m_settings.getSetting<PedalCCMapping<2>>();
  static auto p3 = m_settings.getSetting<PedalCCMapping<3>>();
  static auto p4 = m_settings.getSetting<PedalCCMapping<4>>();
  static auto r1 = m_settings.getSetting<RibbonCCMapping<1>>();
  static auto r2 = m_settings.getSetting<RibbonCCMapping<2>>();
  static auto r3 = m_settings.getSetting<RibbonCCMapping<3>>();
  static auto r4 = m_settings.getSetting<RibbonCCMapping<4>>();
  static auto at = m_settings.getSetting<AftertouchCCMapping>();
  static auto be = m_settings.getSetting<BenderCCMapping>();

  fourteenBitSupport->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  velCC->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  p1->set(PedalCC::CC20);
  p2->set(PedalCC::CC21);
  p3->set(PedalCC::CC22);
  p4->set(PedalCC::CC23);
  r1->set(RibbonCC::CC24);
  r2->set(RibbonCC::CC25);
  r3->set(RibbonCC::CC26);
  r4->set(RibbonCC::CC27);
  at->set(AftertouchCC::CC28);
  be->set(BenderCC::Pitchbend);
}

void SettingsUseCases::setMappingsToClassicMidi()
{
  static auto fourteenBitSupport = m_settings.getSetting<Enable14BitSupport>();
  static auto velCC = m_settings.getSetting<EnableHighVelocityCC>();
  static auto p1 = m_settings.getSetting<PedalCCMapping<1>>();
  static auto p2 = m_settings.getSetting<PedalCCMapping<2>>();
  static auto p3 = m_settings.getSetting<PedalCCMapping<3>>();
  static auto p4 = m_settings.getSetting<PedalCCMapping<4>>();
  static auto r1 = m_settings.getSetting<RibbonCCMapping<1>>();
  static auto r2 = m_settings.getSetting<RibbonCCMapping<2>>();
  static auto r3 = m_settings.getSetting<RibbonCCMapping<3>>();
  static auto r4 = m_settings.getSetting<RibbonCCMapping<4>>();
  static auto at = m_settings.getSetting<AftertouchCCMapping>();
  static auto be = m_settings.getSetting<BenderCCMapping>();

  fourteenBitSupport->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  velCC->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  p1->set(PedalCC::CC20);
  p2->set(PedalCC::CC21);
  p3->set(PedalCC::CC22);
  p4->set(PedalCC::CC23);
  r1->set(RibbonCC::CC24);
  r2->set(RibbonCC::CC25);
  r3->set(RibbonCC::CC26);
  r4->set(RibbonCC::CC27);
  at->set(AftertouchCC::ChannelPressure);
  be->set(BenderCC::Pitchbend);
}

void SettingsUseCases::updateRoutingAspect(int entry, int aspect, bool value)
{
  if(auto s = m_settings.getSetting<RoutingSettings>())
  {
    if(entry < (int) RoutingSettings::tRoutingIndex::LENGTH && aspect < (int) RoutingSettings::tAspectIndex::LENGTH)
    {
      s->setState(static_cast<RoutingSettings::tRoutingIndex>(entry),
                  static_cast<RoutingSettings::tAspectIndex>(aspect), value);
    }
  }
}

void SettingsUseCases::toggleRibbonSelection()
{
  if(m_settings.getSetting<SelectedRibbonsSetting>()->get() == SelectedRibbons::Ribbon1_2)
    setRibbonSelection(SelectedRibbons::Ribbon3_4);
  else
    setRibbonSelection(SelectedRibbons::Ribbon1_2);
}

void SettingsUseCases::setRibbonSelection(SelectedRibbons ribbonsToSelect)
{
  m_settings.getSetting<SelectedRibbonsSetting>()->set(ribbonsToSelect);
}

void SettingsUseCases::panicAudioEngine()
{
  using namespace nltools::msg;
  PanicAudioEngine msg {};
  send<PanicAudioEngine>(EndPoint::AudioEngine, msg);
}

void SettingsUseCases::setRoutingAspectsForEntry(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex index, bool b)
{
  if(auto s = m_settings.getSetting<RoutingSettings>())
  {
    s->setAllAspectsForIndex(index, b);
  }
}

void SettingsUseCases::setAllRoutingEntries(bool state)
{
  if(auto s = m_settings.getSetting<RoutingSettings>())
  {
    s->setAllValues(state);
  }
}

void SettingsUseCases::setGlobalLocal(bool state)
{
  if(auto s = m_settings.getSetting<GlobalLocalEnableSetting>())
  {
    s->set(state ? BooleanSettings::BOOLEAN_SETTING_TRUE : BooleanSettings::BOOLEAN_SETTING_FALSE);
  }
}

void SettingsUseCases::dicePassphrase()
{
  if(auto passwd = m_settings.getSetting<Passphrase>())
  {
    passwd->dice();
  }
}

void SettingsUseCases::defaultPassphrase()
{
  if(auto passwd = m_settings.getSetting<Passphrase>())
  {
    passwd->resetToDefault();
  }
}

void SettingsUseCases::setSettingFromWebUI(const Glib::ustring& key, const Glib::ustring& value, PresetManager& pm)
{
  if(key == "SyncSplit")
  {
    SyncSplitSettingUseCases useCase(*m_settings.getSetting<SplitPointSyncParameters>(), pm);
    useCase.updateFromWebUI(value);
    return;
  }

  if(auto s = m_settings.getSetting(key))
  {
    s->setSetting(Initiator::EXPLICIT_WEBUI, value);
  }
}

void SettingsUseCases::setFocusAndMode(const FocusAndMode& focusAndMode)
{
  m_focusAndModeSetting.set(focusAndMode);
}

void SettingsUseCases::setUIFocus(const UIFocus& focus)
{
  m_focusAndModeSetting.set(focus);
}

void SettingsUseCases::setUIMode(const UIMode& mode)
{
  m_focusAndModeSetting.set(mode);
}

void SettingsUseCases::setUIDetail(const UIDetail& detail)
{
  auto focusAndMode = m_focusAndModeSetting.getState();
  focusAndMode.detail = detail;
  m_focusAndModeSetting.set(focusAndMode);
}

void SettingsUseCases::freezeFocusAndMode()
{
  m_focusAndModeSetting.setFocusAndModeFreeze(true);
}

void SettingsUseCases::thawFocusAndMode()
{
  m_focusAndModeSetting.setFocusAndModeFreeze(false);
}

void SettingsUseCases::refreshScreenSaverTimeout()
{
  m_settings.getSetting<ScreenSaverTimeoutSetting>()->endAndReschedule();
}
