#include "EpcWifi.h"

EpcWifi::EpcWifi()
    : m_currentPassphrase("88888888")
    , m_currentSSID("NL-C15-Unit-00000")
    , m_connectionUp(false)
    , m_status(m_statusInFlight::vacant)
{
    syncCredentials();
    Glib::MainContext::get_default()->signal_timeout().connect_seconds(sigc::mem_fun(this, &EpcWifi::syncCredentials), 2);
//    Glib::MainContext::get_default()->signal_timeout().connect_seconds(sigc::mem_fun(this, &EpcWifi::checkConnectionStatus), 2);
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
    if (m_status == m_statusInFlight::vacant) {
        if (m_currentPassphrase != m_newPassphrase || m_currentSSID != m_newSSID)
            m_status = m_statusInFlight::check;
    }

    if (m_status == m_statusInFlight::check)
        checkConnectionStatus();

    if (m_status == m_statusInFlight::update){
        if (m_currentPassphrase != m_newPassphrase)
            updatePassphrase();
        else if (m_currentSSID != m_newSSID)
            updateSSID();
        else
            m_status = m_statusInFlight::vacant;
    }

    if (m_status == m_statusInFlight::reload)
        reloadConnection();

    if (m_status == m_statusInFlight::busy)
        nltools::Log::info("busy...");

    if (m_status == m_statusInFlight::error)
    {
        nltools::Log::error("FAIL!");
        m_status = m_statusInFlight::vacant;
    }

//    switch (m_status)
//    {
//    case m_statusInFlight::vacant:
//        nltools::Log::info("Checking credentials");
//        if (m_currentPassphrase != m_newPassphrase || m_currentSSID != m_newSSID)
//            m_status = m_statusInFlight::check;
//        break;

//    case m_statusInFlight::check:
//        nltools::Log::info("Checking connection!");
//        checkConnectionStatus();
//        break;

//    case m_statusInFlight::update:
//        nltools::Log::info("Trying update");
//        if (m_currentPassphrase != m_newPassphrase)
//            updatePassphrase();
//        else if (m_currentSSID != m_newSSID)
//            updateSSID();
//        break;

//    case m_statusInFlight::reload:
//        nltools::Log::info("Trying reload");
//        reloadConnection();
//        break;

//    case m_statusInFlight::busy:
//        nltools::Log::info("busy...");
//        break;

//    case m_statusInFlight::error:
//        nltools::Log::error("FAIL!");
//        m_status = m_statusInFlight::vacant;

//        break;
//    }
    return true;
}

void EpcWifi::checkConnectionStatus()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "show", "--active" },
                                 [&](const std::string& s) {
                                    std::size_t pos = s.find("C15");
                                    if (pos != std::string::npos)
                                        m_status = m_statusInFlight::update; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::updateCredentials()
{

}

void EpcWifi::updatePassphrase()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_newPassphrase},
                                 [&](auto) {m_currentPassphrase = m_newPassphrase;
                                            if (m_currentSSID == m_newSSID)
                                                m_status = m_statusInFlight::reload;
                                            else
                                                m_status = m_statusInFlight::update; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}


void EpcWifi::updateSSID()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", m_newSSID},
                                 [&](auto) { m_currentSSID = m_newSSID;
                                            if (m_currentPassphrase == m_newPassphrase)
                                                m_status = m_statusInFlight::reload;
                                            else
                                                m_status = m_statusInFlight::update; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::reloadConnection()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
                                 [&](auto) { enableConnection(); },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                            m_status = m_statusInFlight::error; });
}

void EpcWifi::enableConnection()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                 [&](auto) { m_status = m_statusInFlight::vacant; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                           m_status = m_statusInFlight::error; });
}

void EpcWifi::disableConnection()
{
    m_status = m_statusInFlight::busy;
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [&](auto) { m_status = m_statusInFlight::vacant; },
                                 [&](const std::string& e) { nltools::Log::warning(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                                           m_status = m_statusInFlight::error; });;
}
