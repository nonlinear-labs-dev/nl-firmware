#include "EpcWifi.h"

EpcWifi::EpcWifi()
    : m_currentPassphrase("88888888")
    , m_currentSSID("NL-C15-Unit-00000")
    , m_connectionUp(false)
    , m_status(m_statusInFlight::check)
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
    switch (m_status)
    {
    case m_statusInFlight::vacant:
        if (m_currentPassphrase != m_newPassphrase)
                updatePassphrase();

        if (m_currentSSID != m_newSSID)
                updateSSID();
        break;

    case m_statusInFlight::check:
        checkConnectionStatus();
        break;

    case m_statusInFlight::reload:
        reloadConnection();
        break;

    case m_statusInFlight::inProcess:

        break;

    case m_statusInFlight::error:

        break;
    }
}

void EpcWifi::checkConnectionStatus()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "show", "--active" },
                                 [&](const std::string& s) {
                                    std::size_t pos = s.find("C15");
                                    if (pos == std::string::npos)
                                        m_connectionUp == false;
                                    m_connectionUp = true; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::updatePassphrase()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_currentPassphrase},
                                 [&](auto) {m_currentPassphrase = m_newPassphrase;
                                            if (m_currentSSID == m_newSSID && m_connectionUp == true)
                                                m_status = m_statusInFlight::reload;
                                            m_status = m_statusInFlight::vacant; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}


void EpcWifi::updateSSID()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_currentSSID},
                                 [&](auto) { m_currentSSID = m_newSSID;
                                            if (m_currentPassphrase == m_newPassphrase && m_connectionUp == true)
                                                m_status = m_statusInFlight::reload;
                                            m_status = m_statusInFlight::vacant; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::reloadConnection()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                 [&](auto) { enableConnection(); },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::enableConnection()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                 [&](auto) { m_status = m_statusInFlight::vacant; }, },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                           m_status = m_statusInFlight::error; });
}

void EpcWifi::disableConnection()
{
    m_status = m_statusInFlight::inProcess;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [&](auto) { m_status = m_statusInFlight::vacant; }, },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                           m_status = m_statusInFlight::error; });;
}
