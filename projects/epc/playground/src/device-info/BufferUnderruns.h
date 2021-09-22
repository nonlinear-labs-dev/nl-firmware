#pragma once

#include "DeviceInformationItem.h"

class BufferUnderruns : public DeviceInformationItem
{
 public:
  explicit BufferUnderruns(DeviceInformation *parent);
  ~BufferUnderruns();

  virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  Glib::ustring get() const override;

 private:
  sigc::connection m_msgConnection;
  uint64_t m_numUnderruns = 0;
};
