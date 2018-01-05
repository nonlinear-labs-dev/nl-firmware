#pragma once

#include <presets/PresetBank.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>

class MultiLineLabel;
class Label;
class PresetBank;

class PresetInfoContent : public ControlWithChildren, public Scrollable
{
  private:
    typedef ControlWithChildren super;

  public:
    PresetInfoContent ();
    virtual ~PresetInfoContent ();

    void setPosition (const Rect &rect) override;
    const Rect &getPosition () const override;
    void setDirty () override;

  private:
    void onBankChanged (shared_ptr<PresetBank> bank);
    void onPresetSelectionChanged ();
    void fixLayout();
    bool fillFromPreset (const Preset *preset);
    bool fillDefaults ();
    Preset *getCurrentPreset ();
    void onPresetChanged();
    void connectToPreset (Preset* preset);
    Glib::ustring localizeIsoTime(Glib::ustring unformated_time);

    sigc::connection m_bankConnection;
    sigc::connection m_presetConnection;

    Label *m_storeTime;
    Label *m_deviceName;
    Label *m_softwareVersion;
    MultiLineLabel *m_name;
    MultiLineLabel *m_comment;

    Label *m_nameLabel;
    Label *m_commentLabel;
    Label *m_storeTimeLabel;
    Label *m_deviceNameLabel;
    Label *m_softwareVersionLabel;
};

