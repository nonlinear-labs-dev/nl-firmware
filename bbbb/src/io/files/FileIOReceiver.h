#pragma once

#include "io/Receiver.h"

class FileIOReceiver : public Receiver
{
  public:
    FileIOReceiver(const char *path);
    virtual ~FileIOReceiver();

  private:
    bool onChunk(Glib::IOCondition condition);

    Glib::RefPtr<Glib::IOChannel> m_channel;
    Glib::RefPtr<Glib::IOSource> m_watch;
};

