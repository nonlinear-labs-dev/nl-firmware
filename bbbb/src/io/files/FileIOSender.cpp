#include <io/files/FileIOSender.h>

FileIOSender::FileIOSender(const char *path)
{
  m_channel = Glib::IOChannel::create_from_file(path, "w+");
  m_channel->set_flags(Glib::IO_FLAG_NONBLOCK);
  m_channel->set_encoding();
}

FileIOSender::~FileIOSender()
{
}

void FileIOSender::send(tMessage msg)
{
  gsize len = 0;
  
  auto data = reinterpret_cast<const char*>(msg->get_data(len));
  TRACE(__PRETTY_FUNCTION__ << " --- " << len);
  gsize bytesWritten = 0;
  m_channel->write(data, len, bytesWritten);
  TRACE(__PRETTY_FUNCTION__ << " --- written " << bytesWritten);
  
  m_channel->flush();
  /*
  m_buffer.push(data, len);

  if(!m_watch)
  {
    m_watch = m_channel->create_watch(Glib::IOCondition::IO_OUT);
    m_watch->connect(sigc::mem_fun(this, &FileIOSender::sendChunk));
    m_watch->attach();
    }*/
}

bool FileIOSender::sendChunk(Glib::IOCondition condition)
{
  auto constexpr maxLen = 1024;
  char data[maxLen];
  auto bytesRead = m_buffer.pull(data, maxLen);

  gsize bytesWritten = 0;
  m_channel->write(data, bytesRead, bytesWritten);
  m_channel->flush();

  if(bytesWritten < bytesRead)
  {
    m_buffer.unpull(bytesRead - bytesWritten);
  }
  else
  {
    m_watch->destroy();
    m_watch.reset();
  }
  return true;
}
