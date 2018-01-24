#pragma once

#include <io/Sender.h>
#include "io/RingBuffer.h"

class FileIOSender : public Sender
{
  public:
    FileIOSender(const char *path);
    virtual ~FileIOSender();

    void send(tMessage msg) override;

  private:
    bool sendChunk(Glib::IOCondition condition);

    Glib::RefPtr<Glib::IOChannel> m_channel;
    Glib::RefPtr<Glib::IOSource> m_watch;
    RingBuffer<char, 8192> m_buffer;
};

