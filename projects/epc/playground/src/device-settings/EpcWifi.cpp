#include "EpcWifi.h"

EpcWifi::EpcWifi()
    : m_currentEpcWifiState(false)
    , m_newEpcWifiState(false)
    , m_busy(false)
{
    Glib::MainContext::get_default()->signal_timeout().connect_seconds(sigc::mem_fun(this, &EpcWifi::syncCredentials), 2);
}

EpcWifi::~EpcWifi() = default;

void EpcWifi::setNewSSID(const Glib::ustring& _newSSID)
{
    m_newSSID = _newSSID;
}

void EpcWifi::setNewPassphrase(const Glib::ustring& _newPassphrase)
{
    m_newPassphrase = _newPassphrase;
}

void EpcWifi::setNewWifiState(bool _state)
{
    m_newEpcWifiState = _state;
}

bool EpcWifi::syncCredentials()
{
    if (m_busy)
        return true;

    updateCredentials(false);
    return true;
}

void EpcWifi::updateCredentials(bool _reload)
{
    m_busy = true;
    if (m_currentPassphrase != m_newPassphrase){
        nltools::Log::debug("NM: Updating Passphrase ...");
        updatePassphrase();
    }
    else if (m_currentSSID != m_newSSID){
        nltools::Log::debug("NM: Updating SSID ...");
        updateSSID();
    }
    else if (m_currentEpcWifiState != m_newEpcWifiState ){
        nltools::Log::debug("NM: Updating EPC WiFi switch ...");
        updateWifiSwitch();
    }
    else if (_reload){
        nltools::Log::debug("NM: Reloading ...");
        reloadConnection();
    }
    else
        m_busy = false;
}

void EpcWifi::updateWifiSwitch()
{
    if (m_newEpcWifiState)
        enableConnection();
    else
        disableConnection();
}

void EpcWifi::updatePassphrase()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_newPassphrase},
                                [this, p=m_newPassphrase](auto) { m_currentPassphrase = p;
                                            updateCredentials(true); },
                                     [this](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                                m_busy = false; });
}


void EpcWifi::updateSSID()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_newSSID},
                                [this, p=m_newSSID](auto) { m_currentSSID = p;
                                            updateCredentials(true); },
                                [this](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_busy = false;});
}

void EpcWifi::reloadConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                [this](auto) { enableConnection(); },
                                [this](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_busy = false; });
}

void EpcWifi::enableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                [this](auto) { m_currentEpcWifiState = true;
                                            m_busy = false;},
                                [this](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                             m_busy = false; });
}

void EpcWifi::disableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [this](auto) { m_currentEpcWifiState = false;
                                             m_busy = false;},
                                 [this](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_busy = false; });;
}
