#pragma once
#include <nltools/enums/EnumTools.h>
#include "EpcWifi.h"
#include "NLEnumSetting.h"

ENUM(WifiSettings, int, Disabled, Enabled)

class WifiSetting : public NLEnumSetting<WifiSettings>, public sigc::trackable
{
 public:
  typedef NLEnumSetting<WifiSettings> super;
  explicit WifiSetting(UpdateDocumentContributor& settings);

  bool set(tEnum m) override;
  bool persistent() const override;
  void load(const Glib::ustring& text, Initiator initiator) override;

 private:
  bool isLoading() const;
  void enableDisableBBBWifi(tEnum state);

  sigc::connection m_pollConnection;

  bool m_isLoading = false;
  bool m_didSettingLoad = false;
  bool m_connectionToBBBEstablished = false;
  void setupBBBWifiIfBBBConnectedAndSettingLoaded();
};
