#include <io/files/FileIOReceiver.h>

FileIOReceiver::FileIOReceiver(const char *path)
{
  m_channel = Glib::IOChannel::create_from_file(path, "r+");
  m_channel->set_flags(Glib::IO_FLAG_NONBLOCK);
  m_channel->set_encoding();
  m_watch = m_channel->create_watch(Glib::IOCondition::IO_IN);
  m_watch->connect(sigc::mem_fun(this, &FileIOReceiver::onChunk));
  m_watch->attach();
}

FileIOReceiver::~FileIOReceiver()
{
}

bool FileIOReceiver::onChunk(Glib::IOCondition condition)
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
