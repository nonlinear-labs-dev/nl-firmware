#pragma once
#include <glibmm/ustring.h>
#include <presets/PresetManager.h>
#include <nltools/messaging/Message.h>
class Settings;

class SettingsUseCases
{
 public:
  explicit SettingsUseCases(Settings& s);
  void setMappingsToHighRes();
  void setMappingsToClassicMidi();

  static void panicAudioEngine();

  void updateRoutingAspect(int entry, int aspect, bool value);
  void setAllRoutingEntries(bool state);
  void setRoutingAspectsForEntry(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex index, bool b);

  void setGlobalLocal(bool state);

  void dicePassphrase();
  void defaultPassphrase();
  void setSettingFromWebUI(const Glib::ustring& key, const Glib::ustring& value, PresetManager& pm);


 private:
  Settings& m_settings;
};
