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

 private:
  std::unique_ptr<Expiration> m_expiration = nullptr;
  Signal<void, bool> m_screenSaverSignal;

  std::chrono::minutes m_timeout;
};
