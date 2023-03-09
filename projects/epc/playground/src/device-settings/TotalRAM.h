#pragma once
#include "Setting.h"
#include <device-info/DeviceInformationItem.h>

class DeviceInformation;

class TotalRAM : public DeviceInformationItem
{
 public:
  explicit TotalRAM(DeviceInformation* s);

  Glib::ustring getDisplayString() const override;
  Glib::ustring get() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

 private:
  Glib::ustring m_display;
};
