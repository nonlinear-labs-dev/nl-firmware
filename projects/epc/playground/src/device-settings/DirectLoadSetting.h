#pragma once
#include <presets/PresetPartSelection.h>
#include "BooleanSetting.h"

class DirectLoadSetting : public BooleanSetting
{
 public:
  explicit DirectLoadSetting(Settings& settings);

  DirectLoadSetting(const DirectLoadSetting& other) = delete;
  DirectLoadSetting& operator=(const DirectLoadSetting&) = delete;

  void load(const Glib::ustring& text, Initiator initiator) override;

 private:
  void toggle() override;
  bool set(tEnum e) override;

  friend class DirectLoadUseCases;
};
