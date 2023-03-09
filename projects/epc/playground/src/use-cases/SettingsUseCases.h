#pragma once
#include <glibmm/ustring.h>
#include <presets/PresetManager.h>
#include <nltools/messaging/Message.h>
#include "proxies/hwui/HWUIEnums.h"
#include "device-settings/FocusAndModeSetting.h"
#include "device-settings/SelectedRibbonsSetting.h"

class Settings;

class SettingsUseCases
{
 public:
  explicit SettingsUseCases(Settings& s);

  //Focus And Mode
  void setFocusAndMode(const FocusAndMode& focusAndMode);
  void setUIFocus(const UIFocus& focus);
  void setUIMode(const UIMode& mode);
  void setUIDetail(const UIDetail& detail);
  void freezeFocusAndMode();
  void thawFocusAndMode();

  void toggleRibbonSelection();
  void setRibbonSelection(SelectedRibbons ribbonsToSelect);

  //Screensaver
  void refreshScreenSaverTimeout();

  //AE
  static void panicAudioEngine();

  //MIDI
  void setMappingsToHighRes();
  void setMappingsToClassicMidi();

  void updateRoutingAspect(int entry, int aspect, bool value);
  void setAllRoutingEntries(bool state);
  void setRoutingAspectsForEntry(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex index, bool b);

  void setGlobalLocal(bool state);

  //Device Settings
  void dicePassphrase();
  void defaultPassphrase();

  //General
  void setSettingFromWebUI(const Glib::ustring& key, const Glib::ustring& value, PresetManager& pm);

  void factoryDefault();

 private:
  Settings& m_settings;
  FocusAndModeSetting& m_focusAndModeSetting;
};
