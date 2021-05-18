#pragma once
#include <nltools/threading/Expiration.h>
#include "Setting.h"
#include <proxies/hwui/HWUIEnums.h>

class Layout;

class ScreenSaverTimeoutSetting : public Setting, public sigc::trackable
{
 public:
  explicit ScreenSaverTimeoutSetting(UpdateDocumentContributor& parent);
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

  sigc::connection onScreenSaverStateChanged(sigc::slot<void, bool> s);
  void init() override;
  bool persistent() const override;

  int getSelectedIndex() const;
  const std::vector<Glib::ustring>& getDisplayStrings() const;

  void incDec(int inc, ButtonModifiers m);
  void incDec(int inc);
  void sendState(bool state);
  void endAndReschedule();

 private:
  std::unique_ptr<Expiration> m_expiration = nullptr;
  Signal<void, bool> m_screenSaverSignal;

  int selectedIndex = 2;
  std::chrono::minutes m_timeout;
  const std::array<int, 6> s_logTimeOuts = { 0, 1, 5, 20, 60, 180 };
  const std::vector<Glib::ustring> s_displayStrings
      = { "Disabled", "1 Minute", "5 Minutes", "20 Minutes", "1 Hour", "3 Hours" };

  void onLayoutInstalled(Layout* l);
};
