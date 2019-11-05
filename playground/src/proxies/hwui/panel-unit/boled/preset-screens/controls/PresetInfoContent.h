#pragma once

#include <proxies/hwui/panel-unit/boled/InfoContent.h>

class Preset;
class Uuid;

class PresetInfoContent : public InfoContent
{
 private:
  typedef InfoContent super;

 public:
  PresetInfoContent();
  virtual ~PresetInfoContent();

  void fillContents() override;

 private:
  void onBankChanged(const Uuid &selectedBank);
  void onPresetSelectionChanged();

  void fillFromPreset(const Preset *preset);
  bool fillDefaults();
  static Preset *getCurrentPreset();
  void onPresetChanged();
  void connectToPreset(Preset *preset);

  sigc::connection m_bankConnection;
  sigc::connection m_presetConnection;
};
