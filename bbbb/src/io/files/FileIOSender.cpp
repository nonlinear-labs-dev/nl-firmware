#include <io/files/FileIOSender.h>

FileIOSender::FileIOSender(const char *path)
{
  TRACE("open file for writing: " << path);

  try
  {
    m_channel = Glib::IOChannel::create_from_file(path, "w+");
    m_channel->set_flags(Glib::IO_FLAG_NONBLOCK);
    m_channel->set_encoding();
  }
  catch(...)
  {
    TRACE("Could not open file for writing: " << path);
  }
}

FileIOSender::~FileIOSender()
{
}

void FileIOSender::send(tMessage msg)
{
  if(m_channel)
  {
    gsize len = 0;
    auto data = reinterpret_cast<const char*>(msg->get_data(len));
    gsize bytesWritten = 0;
    m_channel->write(data, len, bytesWritten);
    m_channel->flush();
  }
}
