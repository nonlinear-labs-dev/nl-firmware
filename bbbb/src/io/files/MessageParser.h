#pragma once

#include "bbbb.h"

class MessageParser
{
  public:
    MessageParser();
    virtual ~MessageParser();

    struct NLMessage
    {
      NLMessage();
      ~NLMessage();

      uint16_t type;
      uint16_t length;
      std::vector<uint16_t> params;
    };

    Glib::RefPtr<Glib::Bytes> getMessage() const;
    gsize parse(gconstpointer buffer, gsize numBytes);
    bool isFinished() const;

    static gsize getNumInitialBytesNeeded();
    gsize getMessageLength() const;

  private:
    bool hasReadHeader () const;
    gsize getNumHeaderBytesMissing () const;

    gsize parseHeader(gconstpointer buffer, gsize numBytes);
    gsize parseBody(gconstpointer buffer, gsize numBytes);

    NLMessage m_msg;
    gsize m_numBytesRead;
};

