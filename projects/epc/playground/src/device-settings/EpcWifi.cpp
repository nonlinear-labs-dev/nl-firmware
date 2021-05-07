#include "EpcWifi.h"

EpcWifi::EpcWifi()
    : m_currentPassphrase("88888888")
    , m_currentSSID("NL-C15-Unit-00000")
    , m_connectionUp(false)
{
    syncCredentials();
    Glib::MainContext::get_default()->signal_timeout().connect_seconds(sigc::mem_fun(this, &EpcWifi::syncCredentials), 2);
}

EpcWifi::~EpcWifi() = default;

void EpcWifi::setNewSSID(const Glib::ustring& _newSSID)
{
    m_newSSID = _newSSID;
}

void EpcWifi::setNewPassphrase(const Glib::ustring &_newPassphrase)
{
    m_newPassphrase = _newPassphrase;
}

bool EpcWifi::syncCredentials()
{
    if (checkConnectionStatus())
    {
        if (m_currentPassphrase != m_newPassphrase)
        {
                m_currentPassphrase = m_newPassphrase;
                updatePassphrase();
                reloadConnection();
        }

        if (m_currentSSID != m_newSSID)
        {
            m_currentSSID = m_newSSID;
            updateSSID();
            reloadConnection();
        }
    }
    return true;
}


bool EpcWifi::checkConnectionStatus()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "show", "--active" },
                                 [&](const std::string& s) {
                                    std::size_t pos = s.find("C15");
                                    if (pos != std::string::npos)
                                        m_connectionUp = true; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                     m_connectionUp = false; });
    return m_connectionUp;
}

void EpcWifi::updatePassphrase()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_currentPassphrase},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}


void EpcWifi::updateSSID()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_currentSSID},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void EpcWifi::reloadConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                 [&](auto) { enableConnection(); },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void EpcWifi::enableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void EpcWifi::disableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}
