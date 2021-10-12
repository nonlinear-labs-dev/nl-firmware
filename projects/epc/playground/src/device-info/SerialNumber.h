#pragma once
#include "DeviceInformationItem.h"

class SerialNumber : public DeviceInformationItem
{
 public:
  explicit SerialNumber(DeviceInformation* p);
  Glib::ustring get() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void onIDReceived(uint64_t id);
 private:
  uint64_t m_id;
  sigc::connection m_connection;
};
