#pragma once

#include <nltools/system/SpawnAsyncCommandLine.h>

class Wifi
{
    public:
    Wifi();

    void setSSID(const std::string _newSSID);
    void setPassphrase(std::string _newPassphrase);
    void reloadConnection();
    void disableConnection();
    void enableConnection();
};
