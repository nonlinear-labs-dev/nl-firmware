#pragma once
#include "MenuEditor.h"
#include "SetupLabel.h"
#include <device-settings/midi/RoutingSettings.h>

class FrameBuffer;
class Label;
class LabelStyleable;
class AspectList;

class RoutingsEditor : public MenuEditor
{
 public:
  typedef RoutingSettings::tRoutingIndex tID;
  typedef RoutingSettings::tAspectIndex tAspect;

  explicit RoutingsEditor(tID id, tID& parentSelection);
  void setPosition(const Rect& r) override;
  void drawBackground(FrameBuffer& fb) override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

 protected:
  void incSetting(int inc) override;
  const std::vector<Glib::ustring>& getDisplayStrings() const override;
  int getSelectedIndex() const override;

  void stepEntry(int inc);
  void stepAspect(int inc);

  void onModifiersChanged(ButtonModifiers modifiers);

 private:
  LabelStyleable* m_entryLabel;
  LabelStyleable* m_aspectLabel;
  LabelStyleable* m_valueLabel;
  AspectList* m_aspectList;

  tID m_id;
  tID& m_parentSelection;
  tAspect m_aspect = tAspect::SEND_PRIMARY;

  void update();
  static const Glib::ustring& getTextFor(tID index);
  static const Glib::ustring& getTextFor(tAspect aspect);
  const Glib::ustring& getValueText() const;
};