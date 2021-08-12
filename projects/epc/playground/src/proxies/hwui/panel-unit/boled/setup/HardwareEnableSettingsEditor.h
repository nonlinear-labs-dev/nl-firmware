#pragma once
#include "MenuEditor.h"
#include <device-settings/midi/RoutingSettings.h>

class FrameBuffer;

class HardwareEnableSettingsEditor : public MenuEditor
{
 public:
  explicit HardwareEnableSettingsEditor(RoutingSettings::tRoutingIndex id);
  void setPosition(const Rect& r) override;
  void drawBackground(FrameBuffer& fb) override;

 protected:
  void incSetting(int inc) override;
  const std::vector<Glib::ustring>& getDisplayStrings() const override;
  int getSelectedIndex() const override;

 private:
  const RoutingSettings::tRoutingIndex m_id;
};