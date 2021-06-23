#pragma once
#include <presets/PresetPartSelection.h>
#include "BooleanSetting.h"

class DirectLoadSetting : public BooleanSetting
{
 public:
  explicit DirectLoadSetting(Settings& settings);

  DirectLoadSetting(const DirectLoadSetting& other) = delete;
  DirectLoadSetting& operator=(const DirectLoadSetting&) = delete;

  bool set(tEnum m) override;
  void load(const Glib::ustring& text, Initiator initiator) override;

  void toggleWithinLoadToPart(PresetPartSelection* part);
  void toggleWithoutLoadToPart();

  void enableWithExplicitLoadToPart(Preset* pPreset, VoiceGroup from, VoiceGroup to);

  void enableWithoutLoadToPart();

 private:
  std::function<void(void)> m_currentOnDirectLoadWasEnabledCB;
  bool m_isLoading = false;
};
