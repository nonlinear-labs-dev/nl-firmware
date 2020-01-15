#include <io/files/LPCReceiver.h>
#include "Application.h"
#include "Options.h"
#include "MessageParser.h"
#include <giomm.h>

LPCReceiver::LPCReceiver()
    : super("/dev/lpc_bb_driver", MessageParser::getNumInitialBytesNeeded())
    , m_parser(std::make_unique<MessageParser>())
{
}

LPCReceiver::~LPCReceiver() = default;

void LPCReceiver::onDataReceived(Glib::RefPtr<Glib::Bytes> bytes)
{
  gsize numBytes = 0;
  auto data = reinterpret_cast<const uint8_t*>(bytes->get_data(numBytes));

  if(auto numBytesMissing = m_parser->parse(data, numBytes))
  {
    setBlockSize(numBytesMissing);
  }
  else
  {
    g_assert(m_parser->isFinished());
    super::onDataReceived(m_parser->getMessage());
    m_parser = std::make_unique<MessageParser>();
    setBlockSize(MessageParser::getNumInitialBytesNeeded());
  }
}
