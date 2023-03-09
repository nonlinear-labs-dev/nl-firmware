#pragma once
#include <thread>
#include <nltools/threading/Throttler.h>
#include <device-info/DeviceInformationItem.h>

class DeviceInformation;

class UsedRAM : public DeviceInformationItem, public sigc::trackable
{
 public:
  explicit UsedRAM(DeviceInformation* parent);
  void load(const Glib::ustring& text);

  Glib::ustring getDisplayString() const override;
  Glib::ustring get() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

 private:
  Glib::ustring m_displayString;
  void scheduleReload();

  Throttler m_scheduleThrottler;
};
