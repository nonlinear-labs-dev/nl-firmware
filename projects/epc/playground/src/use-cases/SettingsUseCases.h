#pragma once
class Settings;

class SettingsUseCases
{
 public:
  explicit SettingsUseCases(Settings* s);
  void setMappingsToHighRes();
  void setMappingsToClassicMidi();

  void updateHWSourceEnable(int hw, int aspect, bool value);

 private:
  Settings* m_settings;
};
