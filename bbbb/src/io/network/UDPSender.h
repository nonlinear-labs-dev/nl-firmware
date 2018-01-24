#pragma once

#include <io/Sender.h>
#include "io/RingBuffer.h"

class UDPSender : public Sender
{
  public:
    UDPSender(const std::string &addressWithPort);
    virtual ~UDPSender();

    void send(tMessage msg) override;

  private:
    bool sendChunk(Glib::IOCondition condition);

    Glib::RefPtr<Gio::Socket> m_socket;
    Glib::RefPtr<Gio::SocketSource> m_watch;

    RingBuffer<char, 8192> m_buffer;
};

