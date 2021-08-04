#pragma once
#include <glibmm/ustring.h>
class Settings;

class SettingsUseCases
{
 public:
  explicit SettingsUseCases(Settings* s);
  void setMappingsToHighRes();
  void setMappingsToClassicMidi();

  void updateHWSourceEnable(int hw, int aspect, bool value);
  void panicAudioEngine();
  void dicePassphrase();
  void defaultPassphrase();
  void setSettingFromWebUI(const Glib::ustring& key, const Glib::ustring& value);

 private:
  Settings* m_settings;
};
