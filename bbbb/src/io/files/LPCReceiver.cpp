#include <io/files/LPCReceiver.h>
#include "Application.h"
#include "Options.h"
#include "MessageParser.h"

LPCReceiver::LPCReceiver() :
    super(Application::get().getOptions()->getLPCReceiveFileName(), 1),
    m_parser(std::make_unique<MessageParser>())
{
}

LPCReceiver::~LPCReceiver()
{
}

void LPCReceiver::onDataReceived(Glib::RefPtr<Glib::Bytes> bytes)
{
  gsize numBytes = 0;
  auto data = reinterpret_cast<const uint8_t*>(bytes->get_data(numBytes));
  parseAndForward(data, numBytes);
}

void LPCReceiver::parseAndForward(const uint8_t *data, gsize numBytes)
{
  if(numBytes)
  {
    auto numBytesRead = m_parser->parse(data, numBytes);

    if(m_parser->isFinished())
    {
      super::onDataReceived(m_parser->getMessage());
      m_parser = std::make_unique<MessageParser>();
    }

    parseAndForward(data + numBytesRead, numBytes - numBytesRead);
  }
}
