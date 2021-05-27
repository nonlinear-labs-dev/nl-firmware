#include "EpcWifi.h"

EpcWifi::EpcWifi()
    : m_currentPassphrase("88888888")
    , m_currentSSID("NL-C15-Unit-00000")
    , m_status(StatusInFlight::vacant)
{
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
    switch(m_status){
    case StatusInFlight::vacant:
        nltools::Log::info("NM: Vacant ...");
        if (m_currentPassphrase != m_newPassphrase || m_currentSSID != m_newSSID)
            m_status = StatusInFlight::check;
        break;

    case StatusInFlight::check:
        nltools::Log::info("NM: Checking connection activity ...");
        checkConnectionStatus();
        break;

    case StatusInFlight::update:
        if (m_currentPassphrase != m_newPassphrase){
            nltools::Log::info("NM: Updating Passphrase ...");
            updatePassphrase();
        }
        else if (m_currentSSID != m_newSSID){
            nltools::Log::info("NM: Updating SSID ...");
            updateSSID();
        }
        else
            m_status = StatusInFlight::vacant;
        break;

    case StatusInFlight::reload:
        nltools::Log::info("NM: Reloading connection ...");
        reloadConnection();
        break;

    case StatusInFlight::busy:
        nltools::Log::info("NM: busy...");
        break;

    case StatusInFlight::error:
        nltools::Log::error("NM: FAIL!");
        m_status = StatusInFlight::vacant;
        break;
    }
    return true;
}

void EpcWifi::checkConnectionStatus()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "show", "--active" },
                                [&](const std::string& s) { std::size_t pos = s.find("C15");
                                                            if (pos != std::string::npos)
                                                                m_status = StatusInFlight::update;
                                                            else
                                                                m_status = StatusInFlight::vacant; },
                                [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = StatusInFlight::error; });
}

void EpcWifi::updatePassphrase()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_newPassphrase},
                                [&](auto) { m_currentPassphrase = m_newPassphrase;
                                            if (m_currentSSID == m_newSSID)
                                                m_status = StatusInFlight::reload;
                                            else
                                                m_status = StatusInFlight::update; },
                                     [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                                m_status = StatusInFlight::error; });
}


void EpcWifi::updateSSID()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_newSSID},
                                [&](auto) { m_currentSSID = m_newSSID;
                                            if (m_currentPassphrase == m_newPassphrase)
                                                m_status = StatusInFlight::reload;
                                            else
                                                m_status = StatusInFlight::update; },
                                [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = StatusInFlight::error; });
}

void EpcWifi::reloadConnection()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                [&](auto) { enableConnection(); },
                                [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = StatusInFlight::error; });
}

void EpcWifi::enableConnection()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                [&](auto) { m_status = StatusInFlight::vacant; },
                                [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = StatusInFlight::error; });
}

void EpcWifi::disableConnection()
{
    m_status = StatusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [&](auto) { m_status = StatusInFlight::vacant; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = StatusInFlight::error; });;
}
