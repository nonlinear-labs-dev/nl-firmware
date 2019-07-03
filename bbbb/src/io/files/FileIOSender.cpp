#include <io/files/FileIOSender.h>
#include <logging/Log.h>

FileIOSender::FileIOSender(const char *path)
    : m_path(path)
{
  nltools::Log::notify("open file for writing: ", path);

  try
  {
    m_channel = Glib::IOChannel::create_from_file(path, "w+");
    m_channel->set_encoding();
  }
  catch(...)
  {
    nltools::Log::error("could not open file for writing: ", path);
  }
}

FileIOSender::~FileIOSender()
{
}

void FileIOSender::send(tMessage msg)
{
  if(m_channel)
  {
    try
    {
      gsize len = 0;
      auto data = reinterpret_cast<const char *>(msg->get_data(len));
      gsize bytesWritten = 0;
      m_channel->write(data, static_cast<gssize>(len), bytesWritten);
      m_channel->flush();
    }
    catch(Glib::Error &err)
    {
      nltools::Log::error("Exception: ", m_path, " -> ", err.what());
    }
  }
}
