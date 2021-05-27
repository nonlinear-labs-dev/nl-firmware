#pragma once

#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/logging/Log.h>
#include <nltools/Types.h>
#include <glibmm.h>

class EpcWifi
{
    private:
    Glib::ustring m_currentPassphrase, m_newPassphrase;
    Glib::ustring m_currentSSID, m_newSSID;
    enum StatusInFlight { vacant, busy, check, update, reload, error };
    StatusInFlight m_status;
    bool syncCredentials();
    void checkConnectionStatus();
    void updateSSID();
    void updatePassphrase();
    void updateCredentials();
    void reloadConnection();

    public:
    EpcWifi();
    ~EpcWifi();

    void setNewPassphrase(const Glib::ustring& _newPassphrase);
    void setNewSSID(const Glib::ustring& _newSSID);
    void disableConnection();
    void enableConnection();
};
