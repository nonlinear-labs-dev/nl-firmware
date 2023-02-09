#pragma once

#include "Setting.h"
#include <nltools/messaging/Message.h>
#include "EpcWifi.h"

class Settings;

class Passphrase : public Setting
{
 public:
  explicit Passphrase(Settings& parent);
  ~Passphrase() override;
  Glib::ustring getDisplayString() const override;

  void dice();
  void updatePassword(const Glib::ustring& password);
  void resetToDefault();
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;

  static bool isValidPassword(const Glib::ustring& newPassword);

  constexpr static auto getMaxPasswordLength();
  constexpr static auto getMinPasswordLength();

 private:
  Glib::ustring m_password = "88888888";
};
