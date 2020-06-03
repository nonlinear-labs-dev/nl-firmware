#pragma once

#include <io/Sender.h>
#include <glibmm/refptr.h>

class FileIOSender : public Sender
{
 public:
  FileIOSender(const char *path);
  virtual ~FileIOSender();

  void send(tMessage msg) override;
  void write(const char *bytes, size_t numBytes);

 private:
  void write(const char *bytes, size_t numBytes, size_t numTry);
  Glib::RefPtr<Glib::IOChannel> m_channel;
  const char *m_path = nullptr;
};
