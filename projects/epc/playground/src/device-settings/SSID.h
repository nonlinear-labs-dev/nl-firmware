#pragma once

#include <nltools/messaging/Message.h>
#include "Setting.h"
#include "EpcWifi.h"
#include "device-info/DeviceInformationItem.h"

class Settings;

class SSID : public DeviceInformationItem
{
 public:
  explicit SSID(DeviceInformation* parent, const HardwareFeatures& hwFeatures);
  ~SSID() override;

  Glib::ustring getDisplayString() const override;

  Glib::ustring get() const override;

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

 private:
  const bool m_hasEPCWifi = false;
  void updateSSID(const Glib::ustring& str);
  Glib::ustring m_ssid;
};
