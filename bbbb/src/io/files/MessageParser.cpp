#include "MessageParser.h"
#include <string.h>

MessageParser::NLMessage::NLMessage () :
    type (0),
    length (0)
{
}

MessageParser::NLMessage::~NLMessage ()
{
}

MessageParser::MessageParser () :
    m_numBytesRead (0)
{
}

MessageParser::~MessageParser ()
{
}

bool MessageParser::hasReadHeader () const
{
  return m_numBytesRead >= getNumInitialBytesNeeded ();
}

gsize MessageParser::getNumHeaderBytesMissing () const
{
  return getNumInitialBytesNeeded () - m_numBytesRead;
}

gsize MessageParser::parse (gconstpointer buffer, gsize numBytes)
{
  if(auto missingForHeader = parseHeader(buffer, numBytes))
    return missingForHeader;

  if(auto missingForBody = parseBody(buffer, numBytes))
    return missingForBody;

  return 0;
}

gsize MessageParser::parseHeader(gconstpointer buffer, gsize numBytes)
{
  if(hasReadHeader())
    return 0;

  gsize headerBytesMissing = getNumHeaderBytesMissing ();
  gsize missingBytesAvail = std::min (headerBytesMissing, numBytes);

  uint8_t *rawMsg = reinterpret_cast<uint8_t*> (&m_msg);
  memcpy (rawMsg + m_numBytesRead, buffer, missingBytesAvail);
  m_numBytesRead += missingBytesAvail;

  if (m_numBytesRead == getNumInitialBytesNeeded ())
  {
    m_msg.params.resize (m_msg.length, 0);
  }

  return headerBytesMissing - missingBytesAvail;
}

gsize MessageParser::parseBody (gconstpointer buffer, gsize numBytes)
{
  gsize numBytesToReadAtAll = getMessageLength();
  gsize missingBytes = numBytesToReadAtAll - m_numBytesRead;
  gsize missingBytesAvail = std::min(missingBytes, numBytes);

  gsize posInBody = m_numBytesRead - getNumInitialBytesNeeded ();
  uint8_t *rawMsgBody = reinterpret_cast<uint8_t*> (m_msg.params.data());
  memcpy (rawMsgBody + posInBody, buffer, missingBytesAvail);
  m_numBytesRead += missingBytesAvail;

  return missingBytes - missingBytesAvail;
}

gsize MessageParser::getNumInitialBytesNeeded ()
{
  return sizeof(NLMessage::type) + sizeof(NLMessage::length);
}

Glib::RefPtr<Glib::Bytes> MessageParser::getMessage () const
{
  auto len = getMessageLength();
  auto buf = g_new(uint8_t, len);

  memcpy(buf, &m_msg.type, 2);
  memcpy(buf + 2, &m_msg.length, 2);
  memcpy(buf + 4, m_msg.params.data(), 2 * m_msg.length);

  return Glib::Bytes::create(buf, len);
}

gsize MessageParser::getMessageLength() const
{
  g_assert(hasReadHeader());
  gsize numBytesInBody = m_msg.length * sizeof(uint16_t);
  gsize numBytesToReadAtAll = getNumInitialBytesNeeded() + numBytesInBody;
  return numBytesToReadAtAll;
}

bool MessageParser::isFinished() const
{
  if(!hasReadHeader())
    return false;

  gsize numBytesToReadAtAll = getMessageLength();
  return m_numBytesRead >= numBytesToReadAtAll;
}
