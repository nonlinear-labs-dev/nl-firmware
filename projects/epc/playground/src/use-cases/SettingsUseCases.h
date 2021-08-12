#pragma once
#include <glibmm/ustring.h>
class Settings;

class SettingsUseCases
{
 public:
  explicit SettingsUseCases(Settings* s);
  void setMappingsToHighRes();
  void setMappingsToClassicMidi();

  void updateRoutingAspect(int entry, int aspect, bool value);
  static void panicAudioEngine();

  void setAllRoutingEntries(bool state);

  void setGlobalLocal(bool state);

  void dicePassphrase();
  void defaultPassphrase();
  void setSettingFromWebUI(const Glib::ustring& key, const Glib::ustring& value);

 private:
  Settings* m_settings;
};
