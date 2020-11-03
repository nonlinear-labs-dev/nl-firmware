#pragma once
#include <nltools/threading/Expiration.h>
#include "Setting.h"

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

  void incDec(int inc);
  void endAndReschedule();

 private:
  void sendState(bool state);

  std::unique_ptr<Expiration> m_expiration = nullptr;
  Signal<void, bool> m_screenSaverSignal;

  size_t selectedIndex;
  std::chrono::minutes m_timeout;
  const std::array<int, 6> s_logTimeOuts = { 0, 1, 5, 20, 60, 180 };
};
