#pragma once

#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/system/SpawnCommandLine.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>

class Wifi
{
    private:
    Glib::ustring m_currentPassphrase, m_newPassphrase;
    Glib::ustring m_currentSSID, m_newSSID;
    bool m_connectionUp;
    bool syncCredentials();
    bool checkConnectionStatus();
    void updateSSID();
    void updatePassphrase();
    void reloadConnection();

    public:
    Wifi();

    void setNewPassphrase(const Glib::ustring& _newPassphrase);
    void setNewSSID(const Glib::ustring& _newSSID);
    void disableConnection();
    void enableConnection();
};
