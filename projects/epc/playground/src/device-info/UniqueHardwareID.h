#pragma once
#include "DeviceInformationItem.h"
#include <limits>

class UniqueHardwareID : public DeviceInformationItem
{
 public:
  explicit UniqueHardwareID(DeviceInformation* p);
  Glib::ustring get() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void onIDReceived(uint64_t id);

 private:
  uint64_t m_id = std::numeric_limits<uint64_t>::max();
  sigc::connection m_connection;
};
