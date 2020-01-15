#include <io/Receiver.h>
#include <giomm.h>

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::setCallback(Callback cb)
{
  m_callback = cb;
}

void Receiver::onDataReceived(Glib::RefPtr<Glib::Bytes> bytes)
{
  m_callback(bytes);
}
