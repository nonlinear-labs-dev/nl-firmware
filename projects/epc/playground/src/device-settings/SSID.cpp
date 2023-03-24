#include <device-settings/DeviceName.h>
#include <device-settings/Settings.h>
#include <device-settings/SSID.h>
#include <proxies/hwui/HardwareFeatures.h>
#include <Application.h>
#include <xml/Writer.h>

SSID::SSID(DeviceInformation *parent, const HardwareFeatures &hwFeatures)
    : DeviceInformationItem(parent)
    , m_hasEPCWifi(hwFeatures.hasEPCWiFi())
{
  Application::get().getSettings()->getSetting<DeviceName>()->onChange(
      [=](const Setting *s)
      {
        static const std::string dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-_";

        std::string ssid = "NL-C15-" + s->save();

        for(char &it : ssid)
          if(dict.find(it) == std::string::npos)
            it = '_';

        updateSSID(ssid);
      });
}

void SSID::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("ssid", get());
}

Glib::ustring SSID::get() const
{
  return m_ssid;
}

SSID::~SSID() = default;

Glib::ustring SSID::getDisplayString() const
{
  return m_ssid;
}

void SSID::updateSSID(const Glib::ustring &str)
{
  if(m_ssid != str)
  {
    m_ssid = str;
    auto suffix = (m_hasEPCWifi ? "_BBB" : "");
    auto ssidMsg = nltools::msg::WiFi::SetWiFiSSIDMessage(m_ssid + suffix);
    nltools::msg::send(nltools::msg::EndPoint::BeagleBone, ssidMsg);
    DeviceInformationItem::onChange();
  }
}
