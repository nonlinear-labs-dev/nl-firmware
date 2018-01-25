#pragma once

#include <io/Sender.h>

class FileIOSender : public Sender
{
  public:
    FileIOSender(const char *path);
    virtual ~FileIOSender();

    void send(tMessage msg) override;

  private:
    Glib::RefPtr<Glib::IOChannel> m_channel;
};

