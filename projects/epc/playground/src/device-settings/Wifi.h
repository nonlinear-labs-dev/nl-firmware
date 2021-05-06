#pragma once

#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/system/SpawnCommandLine.h>
#include <nltools/logging/Log.h>

class Wifi
{
    public:
    Wifi();

    void setSSID(const Glib::ustring& _newSSID);
    void setPassphrase(const Glib::ustring& _newPassphrase);
    void reloadConnection();
    void disableConnection();
    void enableConnection();

    void waitForConnection();
};
