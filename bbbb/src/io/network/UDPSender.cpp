#include <io/network/UDPSender.h>

UDPSender::UDPSender(const std::string &addressWithPort)
{
  using namespace Gio;

  auto colon = addressWithPort.find(':');
  auto ip = addressWithPort.substr(0, colon);
  auto port = addressWithPort.substr(colon + 1);

  m_socket = Socket::create(SocketFamily::SOCKET_FAMILY_IPV4, SocketType::SOCKET_TYPE_DATAGRAM, SocketProtocol::SOCKET_PROTOCOL_UDP);
  m_address = InetSocketAddress::create(InetAddress::create(ip.c_str()), std::stoi(port));
}

UDPSender::~UDPSender()
{
}

void UDPSender::send(tMessage msg)
{ 
  gsize len = 0;
  auto data = reinterpret_cast<const char*>(msg->get_data(len));

  try
  {
    m_socket->send_to(m_address, data, len);
  }
  catch(...)
  {
    TRACE("Could not send UDP datagram");
  }
}


bool UDPSender::sendChunk(Glib::IOCondition condition)
{
  auto constexpr maxLen = 1024;
  char data[maxLen];
  auto bytesRead = m_buffer.pull(data, maxLen);

  try
  {
    gsize bytesWritten = m_socket->send_to(m_address, data, bytesRead);
  
    if(bytesWritten < bytesRead)
    {
      m_buffer.unpull(bytesRead - bytesWritten);
    }
    else
    {
      m_watch->destroy();
      m_watch.reset();
    }
  }
  catch(...)
  {
    TRACE("Could not send UDP datagram");
  }
  return true;
}
