#pragma once

#include <proxies/hwui/DFBLayout.h>

class Label;
class PresetBank;
class Setting;
class DirectLoadIndicator;

class BanksLayout : public DFBLayout
{
  private:
    typedef DFBLayout super;

  public:
    BanksLayout ();
    virtual ~BanksLayout ();

  private:
    void onBankSelected(shared_ptr<PresetBank> bank);
    void onAutoLoadSettingChanged(const Setting *s);
    void onBankChanged();

    Label *m_number;
    Label *m_name;
    sigc::connection m_bankconnection;
    DirectLoadIndicator *m_directLoad;
    shared_ptr<PresetBank> m_bank;

  void updateFromBank(const shared_ptr<PresetBank> &bank) const;
};

