#pragma once
#include <thread>
#include "Setting.h"
#include <nltools/threading/Throttler.h>

class UsedRAM : public Setting, public sigc::trackable
{
 public:
  explicit UsedRAM(UpdateDocumentContributor& parent);
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  bool persistent() const override;
  void init() override;

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;
  Glib::ustring m_displayString;
  void scheduleReload();

  Throttler m_scheduleThrottler;
};
