#pragma once

#include "bbbb.h"
#include <vector>

class MessageParser
{
 public:
  MessageParser();
  virtual ~MessageParser();

  Glib::RefPtr<Glib::Bytes> getMessage() const;
  gsize parse(gconstpointer buffer, gsize numBytes);
  bool isFinished() const;

  static gsize getNumInitialBytesNeeded();
  gsize getMessageLength() const;

 private:
  bool hasReadHeader() const;
  gsize getNumHeaderBytesMissing() const;

  gsize parseHeader(gconstpointer buffer, gsize numBytes);
  gsize parseBody(gconstpointer buffer, gsize numBytes);

  struct __attribute__((packed)) NLMessageHeader
  {
    uint16_t type = 0;
    uint16_t length = 0;
  } m_header;

  std::vector<uint16_t> m_params;

  gsize m_numBytesRead = 0;
};
