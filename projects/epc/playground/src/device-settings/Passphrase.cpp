#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include "Passphrase.h"
#include "Settings.h"

Passphrase::Passphrase(Settings& parent)
    : Setting(parent)
{
}

Passphrase::~Passphrase() = default;

void Passphrase::load(const Glib::ustring& password, Initiator initiator)
{
  if(!password.empty())
  {
    updatePassword(password);
  }
}

Glib::ustring Passphrase::save() const
{
  return m_password;
}

constexpr auto Passphrase::getMinPasswordLength()
{
  return 8;
}

constexpr auto Passphrase::getMaxPasswordLength()
{
  return nltools::msg::WiFi::SetWiFiPasswordMessage::maxSize;
}

bool Passphrase::isValidPassword(const Glib::ustring& newPassword)
{
  if(newPassword.length() >= 8 && newPassword.length() <= getMaxPasswordLength())
  {
    return newPassword.is_ascii();
  }
  return false;
}

void Passphrase::dice()
{
  static const std::string dict = "abcdefghkmnpqrstuvwxyzABCDEFGHKMNPQRSTUVWXYZ123456789";

  std::string r;

  for(int i = 0; i < 8; i++)
    r += dict[g_random_int_range(0, dict.length())];

  updatePassword(r);
}

void Passphrase::resetToDefault()
{
  updatePassword("88888888");
}

void Passphrase::updatePassword(const Glib::ustring& password)
{
  if(isValidPassword(password))
  {
    if(m_password != password)
    {
      m_password = password;

      const auto maxLen = getMaxPasswordLength();
      auto shortened = m_password.size() <= maxLen ? m_password : m_password.substr(0, maxLen);
      auto passwordMsg = nltools::msg::WiFi::SetWiFiPasswordMessage(shortened);
      nltools::msg::send(nltools::msg::EndPoint::BeagleBone, passwordMsg);
      notify();
    }
  }
}

Glib::ustring Passphrase::getDisplayString() const
{
  return m_password;
}
