#include <io/network/UDPReceiver.h>
#include "bbbb.h"

UDPReceiver::UDPReceiver(int port)
{
  using namespace Gio;

  m_socket = Socket::create(SocketFamily::SOCKET_FAMILY_IPV4, SocketType::SOCKET_TYPE_DATAGRAM, SocketProtocol::SOCKET_PROTOCOL_UDP);
  m_socket->bind(InetSocketAddress::create(InetAddress::create_any(SocketFamily::SOCKET_FAMILY_IPV4), port), true);
  m_watch = m_socket->create_source(Glib::IOCondition::IO_IN);
  m_watch->attach(Glib::MainContext::get_default());
  m_watch->connect(sigc::mem_fun(this, &UDPReceiver::onDatagram));
}

UDPReceiver::~UDPReceiver()
{
}

bool UDPReceiver::onDatagram(Glib::IOCondition condition)
{
  constexpr auto maxLength = 65507;
  char buffer[maxLength];
  gsize bytesReceived = m_socket->receive(buffer, maxLength);

  if(bytesReceived)
  {
    auto bytes = Glib::Bytes::create(buffer, bytesReceived);
    onDataReceived(bytes);
  }

  return true;
}
