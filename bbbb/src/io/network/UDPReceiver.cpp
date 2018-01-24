#include <io/network/UDPReceiver.h>
#include "bbbb.h"

UDPReceiver::UDPReceiver(int port)
{
  using namespace Gio;

  m_socket = Socket::create(SocketFamily::SOCKET_FAMILY_IPV4, SocketType::SOCKET_TYPE_DATAGRAM, SocketProtocol::SOCKET_PROTOCOL_UDP);
  m_socket->bind(InetSocketAddress::create(InetAddress::create_any(SocketFamily::SOCKET_FAMILY_IPV4), port), true);
  m_channel = Glib::IOChannel::create_from_fd(m_socket->get_fd());
  m_watch = m_channel->create_watch(Glib::IOCondition::IO_IN);
  m_watch->attach(Glib::MainContext::get_default());
  m_watch->connect(sigc::mem_fun(this, &UDPReceiver::onDatagram));
}

UDPReceiver::~UDPReceiver()
{
}

bool UDPReceiver::onDatagram(Glib::IOCondition condition)
{
  constexpr auto maxLength = 8192;
  char buffer[maxLength];
  gsize bytesReceived = 0;
  m_channel->read(buffer, maxLength, bytesReceived);

  if(bytesReceived)
  {
    auto bytes = Glib::Bytes::create(buffer, bytesReceived);
    onDataReceived(bytes);
  }

  return true;
}
