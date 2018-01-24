#pragma once

#include "io/Receiver.h"
#include <glibmm.h>
#include <giomm.h>

class UDPReceiver : public Receiver
{
    using super = Receiver;

  public:
    UDPReceiver(int port);
    virtual ~UDPReceiver();

  private:
    bool onDatagram(Glib::IOCondition condition);

    Glib::RefPtr<Gio::Socket> m_socket;
    Glib::RefPtr<Glib::IOChannel> m_channel;
    Glib::RefPtr<Glib::IOSource> m_watch;
};

