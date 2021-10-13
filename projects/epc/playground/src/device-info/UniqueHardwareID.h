#pragma once
#include "DeviceInformationItem.h"

class UniqueHardwareID : public DeviceInformationItem
{
 public:
  explicit UniqueHardwareID(DeviceInformation* p);
  Glib::ustring get() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void onIDReceived(uint64_t id);

 private:
  uint64_t m_id;
  sigc::connection m_connection;
};
