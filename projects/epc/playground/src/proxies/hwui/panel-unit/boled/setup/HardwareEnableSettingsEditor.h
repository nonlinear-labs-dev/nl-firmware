#pragma once
#include "MenuEditor.h"
#include <device-settings/midi/RoutingSettings.h>

class FrameBuffer;
class Label;
class VariableCenterAlignedLabel;
class AspectList;

class HardwareEnableSettingsEditor : public MenuEditor
{
 public:
  explicit HardwareEnableSettingsEditor(RoutingSettings::tRoutingIndex id);
  void setPosition(const Rect& r) override;
  void drawBackground(FrameBuffer& fb) override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

 protected:
  void incSetting(int inc) override;
  const std::vector<Glib::ustring>& getDisplayStrings() const override;
  int getSelectedIndex() const override;

  void stepEntry(int inc);
  void stepAspect(int inc);

 private:
  VariableCenterAlignedLabel* m_entryLabel;
  VariableCenterAlignedLabel* m_aspectLabel;
  VariableCenterAlignedLabel* m_valueLabel;
  AspectList* m_aspectList;

  RoutingSettings::tRoutingIndex m_id;
  RoutingSettings::tAspectIndex m_aspect = RoutingSettings::tAspectIndex::SEND_PRIMARY;
  void update();
  static const Glib::ustring& getTextFor(RoutingSettings::tRoutingIndex index) ;
  static const Glib::ustring& getTextFor(RoutingSettings::tAspectIndex aspect) ;
  const Glib::ustring& getValueText() const;
};