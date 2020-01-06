#pragma once

#include <io/files/FileIOReceiver.h>

class MessageParser;

class LPCReceiver : public FileIOReceiver
{
  using super = FileIOReceiver;

 public:
  LPCReceiver();
  virtual ~LPCReceiver();

 protected:
  void onDataReceived(Glib::RefPtr<Glib::Bytes> bytes) override;

 private:
  std::unique_ptr<MessageParser> m_parser;
};