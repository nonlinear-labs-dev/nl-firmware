#include "Wifi.h"

Wifi::Wifi()
{

}

void Wifi::setPassphrase(std::string _newPassphrase)
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk ", _newPassphrase}, [](auto) {}, [](auto) {});
}


void Wifi::setSSID(std::string _newSSID)
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "modify", "C15", "wifi.ssid", _newSSID}, [](auto) {}, [](auto) {});
}

void Wifi::reloadConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "systemctl", "restart", "NetworkManager"}, [](auto) {}, [](auto) {});

}

void Wifi::enableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "up", "C15"}, [](auto) {}, [](auto) {});

}

void Wifi::disableConnection()
{
    SpawnAsyncCommandLine::spawn(std::vector<std::string> { "nmcli", "con", "down", "C15"}, [](auto) {}, [](auto) {});

}
