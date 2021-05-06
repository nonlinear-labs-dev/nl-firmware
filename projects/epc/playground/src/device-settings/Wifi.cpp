#include "Wifi.h"

Wifi::Wifi()
{
//    waitForConnection();
}

void Wifi::setPassphrase(const Glib::ustring& _newPassphrase)
{
//    waitForConnection();
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", _newPassphrase},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
    reloadConnection();
}


void Wifi::setSSID(const Glib::ustring& _newSSID)
{
//    waitForConnection();
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", _newSSID},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
    reloadConnection();
}

void Wifi::reloadConnection()
{
//    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "down", "C15" },
//                                 [](auto) {},
//                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

//    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "up", "C15" },
//                                 [](auto) {},
//                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "connection", "reload" },
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "systemctl", "restart", "NetworkManager" },
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void Wifi::enableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });

}

void Wifi::disableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"},
                                 [](auto) {},
                                 [&](const std::string& e) { nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e); });
}

void Wifi::waitForConnection()
{
    nltools::Log::info("Waiting on NM ...");
    auto done = false;
    int counter = 0;
    while (done == false) {
        SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nm-online", "-x" },
                                     [&](auto) { done = true; },
                                     [&](const std::string& e) { nltools::Log::warning(counter, e); });
    counter++;
    }
}
