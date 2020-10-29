#pragma once

class PresetManager;
class Preset;
class Bank;

class PresetManagerUseCases
{
 public:
  explicit PresetManagerUseCases(PresetManager* pm);

  //Store Actions
  void overwritePreset(Preset* preset);
  void overwritePresetWithPreset(Preset* target, Preset* source);
  void insertPreset(Bank* bank, size_t pos);
  void appendPreset(Bank* bank);
  void createBankAndStoreEditBuffer();

 private:
  PresetManager* m_presetManager;
};
