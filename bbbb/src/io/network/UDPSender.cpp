#include <io/network/UDPSender.h>

UDPSender::UDPSender(const std::string &addressWithPort)
{
  using namespace Gio;

  auto colon = addressWithPort.find(':');
  auto ip = addressWithPort.substr(0, colon);
  auto port = addressWithPort.substr(colon + 1);

  m_socket = Socket::create(SocketFamily::SOCKET_FAMILY_IPV4, SocketType::SOCKET_TYPE_DATAGRAM, SocketProtocol::SOCKET_PROTOCOL_UDP);
  m_socket->connect(InetSocketAddress::create(InetAddress::create(ip.c_str()), std::stoi(port)));
}

UDPSender::~UDPSender()
{
}

void UDPSender::send(tMessage msg)
{
  gsize len = 0;
  auto data = reinterpret_cast<const char*>(msg->get_data(len));
  m_buffer.push(data, len);

  if(!m_watch)
  {
    m_watch = m_socket->create_source(Glib::IOCondition::IO_OUT);
    m_watch->connect(sigc::mem_fun(this, &UDPSender::sendChunk));
    m_watch->attach();
  }
}


bool UDPSender::sendChunk(Glib::IOCondition condition)
{
  auto constexpr maxLen = 1024;
  char data[maxLen];
  auto bytesRead = m_buffer.pull(data, maxLen);

  gsize bytesWritten = m_socket->send(data, bytesRead);

  if(bytesWritten < bytesRead)
  {
    m_buffer.unpull(bytesRead - bytesWritten);
  }
  else
  {
    m_watch->destroy();
    m_watch.reset();
  }
  return true;
}
