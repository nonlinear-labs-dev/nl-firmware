#pragma once

#include <io/files/FileIOReceiver.h>
#include <memory>

class MessageParser;

class PlaycontrollerReceiver : public FileIOReceiver
{
  using super = FileIOReceiver;

 public:
  PlaycontrollerReceiver();
  virtual ~PlaycontrollerReceiver();

 protected:
  void onDataReceived(Glib::RefPtr<Glib::Bytes> bytes) override;

 private:
  Glib::RefPtr<Glib::Bytes> interceptHeartbeat(Glib::RefPtr<Glib::Bytes> msg);

  std::unique_ptr<MessageParser> m_parser;
  uint64_t m_heartbeat = 0;
};
