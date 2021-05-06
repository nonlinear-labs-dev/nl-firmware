#include "Wifi.h"

Wifi::Wifi()
    : m_currentPassphrase("88888888")
    , m_currentSSID("NL-C15-Unit-00000")
    , m_connectionUp(false)
{
    nltools::Log::info("First synching...");
    syncCredentials();
    Glib::MainContext::get_default()->signal_timeout().connect_seconds(sigc::mem_fun(this, &Wifi::syncCredentials), 2);
}

void Wifi::setNewSSID(const Glib::ustring& _newSSID)
{
    m_newSSID = _newSSID;
}

void Wifi::setNewPassphrase(const Glib::ustring &_newPassphrase)
{
    m_newPassphrase = _newPassphrase;
}

bool Wifi::syncCredentials()
{
    if (m_currentPassphrase != m_newPassphrase)
    {
        if (checkConnectionStatus())
        {
            m_currentPassphrase = m_newPassphrase;
            updatePassphrase();
            reloadConnection();
        }
    }

    if (m_currentSSID != m_newSSID)
    {
        if (checkConnectionStatus())
        {
            m_currentSSID = m_newSSID;
            updateSSID();
            reloadConnection();
        }
    }
    return true;
}


bool Wifi::checkConnectionStatus()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "show", "--active", "|", "grep", "C15" },
                                 [&](auto) {m_connectionUp = true; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
//                                 [&](auto) {nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, em_connectionUp = false; });
    return m_connectionUp;
}

void Wifi::updatePassphrase()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_currentPassphrase},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}


void Wifi::updateSSID()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_currentSSID},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void Wifi::reloadConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                 [&](auto) { enableConnection(); },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

//    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "up", "C15" },
//                                 [](auto) {},
//                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

//    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "reload" },
//                                 [](auto) {},
//                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

//    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "systemctl", "restart", "NetworkManager" },
//                                 [](auto) {},
//                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void Wifi::enableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void Wifi::disableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}
