#include <device-settings/DeviceName.h>
#include <device-settings/Settings.h>
#include <device-settings/SSID.h>
#include <proxies/hwui/HardwareFeatures.h>
#include <utility>

SSID::SSID(Settings &parent, const HardwareFeatures &hwFeatures)
    : Setting(parent)
    , m_hasEPCWifi(hwFeatures.hasEPCWiFi())
{
  parent.getSetting<DeviceName>()->onChange([=](const Setting *s) {
    static const std::string dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-_";

    std::string ssid = "NL-C15-" + s->save();

    for(char &it : ssid)
      if(dict.find(it) == std::string::npos)
        it = '_';

    updateSSID(ssid);
  });
}

SSID::~SSID() = default;

void SSID::load(const Glib::ustring &ssid, Initiator initiator)
{
  updateSSID(ssid);
}

Glib::ustring SSID::save() const
{
  return m_ssid;
}

bool SSID::persistent() const
{
  return true;
}

Glib::ustring SSID::getDisplayString() const
{
  return m_ssid;
}

void SSID::updateSSID(const Glib::ustring &str)
{
  m_ssid = str;
  auto suffix = (m_hasEPCWifi ? "_BBB" : "");
  auto ssidMsg = nltools::msg::WiFi::SetWiFiSSIDMessage(m_ssid + suffix);
  nltools::msg::send(nltools::msg::EndPoint::BeagleBone, ssidMsg);
  notify();
}
