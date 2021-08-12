#include "SettingsUseCases.h"
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

SettingsUseCases::SettingsUseCases(Settings *s)
    : m_settings { s }
{
}

void SettingsUseCases::setMappingsToHighRes()
{
  auto fourteenBitSupport = m_settings->getSetting<Enable14BitSupport>();
  auto velCC = m_settings->getSetting<EnableHighVelocityCC>();
  auto p1 = m_settings->getSetting<PedalCCMapping<1>>();
  auto p2 = m_settings->getSetting<PedalCCMapping<2>>();
  auto p3 = m_settings->getSetting<PedalCCMapping<3>>();
  auto p4 = m_settings->getSetting<PedalCCMapping<4>>();
  auto r1 = m_settings->getSetting<RibbonCCMapping<1>>();
  auto r2 = m_settings->getSetting<RibbonCCMapping<2>>();
  auto at = m_settings->getSetting<AftertouchCCMapping>();
  auto be = m_settings->getSetting<BenderCCMapping>();

  fourteenBitSupport->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  velCC->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  p1->set(PedalCC::CC20);
  p2->set(PedalCC::CC21);
  p3->set(PedalCC::CC22);
  p4->set(PedalCC::CC23);
  r1->set(RibbonCC::CC24);
  r2->set(RibbonCC::CC25);
  at->set(AftertouchCC::CC26);
  be->set(BenderCC::Pitchbend);
}

void SettingsUseCases::setMappingsToClassicMidi()
{
  auto fourteenBitSupport = m_settings->getSetting<Enable14BitSupport>();
  auto velCC = m_settings->getSetting<EnableHighVelocityCC>();
  auto p1 = m_settings->getSetting<PedalCCMapping<1>>();
  auto p2 = m_settings->getSetting<PedalCCMapping<2>>();
  auto p3 = m_settings->getSetting<PedalCCMapping<3>>();
  auto p4 = m_settings->getSetting<PedalCCMapping<4>>();
  auto r1 = m_settings->getSetting<RibbonCCMapping<1>>();
  auto r2 = m_settings->getSetting<RibbonCCMapping<2>>();
  auto at = m_settings->getSetting<AftertouchCCMapping>();
  auto be = m_settings->getSetting<BenderCCMapping>();

  fourteenBitSupport->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  velCC->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  p1->set(PedalCC::CC20);
  p2->set(PedalCC::CC21);
  p3->set(PedalCC::CC22);
  p4->set(PedalCC::CC23);
  r1->set(RibbonCC::CC24);
  r2->set(RibbonCC::CC25);
  at->set(AftertouchCC::ChannelPressure);
  be->set(BenderCC::Pitchbend);
}

void SettingsUseCases::updateRoutingAspect(int entry, int aspect, bool value)
{
  if(auto s = m_settings->getSetting<RoutingSettings>())
  {
    if(entry < (int) RoutingSettings::tRoutingIndex::LENGTH && aspect < (int) RoutingSettings::tAspectIndex::LENGTH)
    {
      s->setState(static_cast<RoutingSettings::tRoutingIndex>(entry),
                  static_cast<RoutingSettings::tAspectIndex>(aspect), value);
    }
  }
}

void SettingsUseCases::panicAudioEngine()
{
  using namespace nltools::msg;
  PanicAudioEngine msg {};
  send<PanicAudioEngine>(EndPoint::AudioEngine, msg);
}

void SettingsUseCases::setAllRoutingEntries(bool state)
{
  if(auto s = m_settings->getSetting<RoutingSettings>())
  {
    s->setAllValues(state);
  }
}

void SettingsUseCases::setGlobalLocal(bool state)
{
  if(auto s = m_settings->getSetting<GlobalLocalEnableSetting>())
  {
    s->set(state ? BooleanSettings::BOOLEAN_SETTING_TRUE : BooleanSettings::BOOLEAN_SETTING_FALSE);
  }
}

void SettingsUseCases::dicePassphrase()
{
  if(auto passwd = m_settings->getSetting<Passphrase>())
  {
    passwd->dice();
  }
}

void SettingsUseCases::defaultPassphrase()
{
  if(auto passwd = m_settings->getSetting<Passphrase>())
  {
    passwd->resetToDefault();
  }
}

void SettingsUseCases::setSettingFromWebUI(const Glib::ustring &key, const Glib::ustring& value)
{
  if(key == "SyncSplit")
  {
    auto useCases = SyncSplitSettingUseCases::get();
    useCases.updateFromWebUI(value);
    return;
  }

  if(auto s = m_settings->getSetting(key))
  {
    s->setSetting(Initiator::EXPLICIT_WEBUI, value);
  }
}
