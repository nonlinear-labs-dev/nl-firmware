#pragma once
#include <nltools/threading/Expiration.h>
#include "Setting.h"
#include <proxies/hwui/HWUIEnums.h>
#include <nltools/messaging/Message.h>

class Layout;

class ScreenSaverTimeoutSetting : public Setting, public sigc::trackable
{
 public:
  explicit ScreenSaverTimeoutSetting(UpdateDocumentContributor& parent);

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  void init() override;

  sigc::connection onScreenSaverStateChanged(sigc::slot<void, bool> s);
  const std::vector<Glib::ustring>& getDisplayStrings() const;
  int getSelectedIndex() const;

  void incDec(int inc, ButtonModifiers m);
  void incDec(int inc);
  void sendState(bool state);
  void endAndReschedule();

 private:
  void onLayoutInstalled(Layout* l);

  Expiration m_expiration;
  Signal<void, bool> m_screenSaverSignal;

  int m_selectedIndex = 2;
  std::chrono::minutes m_timeout;
};
