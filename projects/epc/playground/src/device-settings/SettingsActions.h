#pragma once

#include <presets/PresetManager.h>
#include "http/RPCActionManager.h"

class Settings;

class SettingsActions : public RPCActionManager
{
 private:
  typedef RPCActionManager super;

 public:
  explicit SettingsActions(Settings& settings, PresetManager& pm);
  ~SettingsActions() override;

  SettingsActions(const SettingsActions& other) = delete;
  SettingsActions& operator=(const SettingsActions&) = delete;
};
