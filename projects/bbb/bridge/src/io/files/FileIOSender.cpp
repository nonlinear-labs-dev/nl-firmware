#include <io/files/FileIOSender.h>
#include <nltools/logging/Log.h>
#include <glibmm/iochannel.h>
#include <glibmm/bytes.h>
#include <giomm.h>

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
  gsize len = 0;
  auto data = reinterpret_cast<const char *>(msg->get_data(len));
  write(data, len);
}

void FileIOSender::write(const char *bytes, size_t numBytes)
{
  write(bytes, numBytes, 0);
}

void FileIOSender::write(const char *bytes, size_t numBytes, size_t numTry)
{
  if(numTry == 1000)
  {
    nltools::Log::error("Could not write", numBytes, "bytes to file", m_path);
    return;
  }

  if(m_channel)
  {
    gsize numBytesWritten = 0;
    auto res = m_channel->write(bytes, static_cast<gssize>(numBytes), numBytesWritten);

    switch(res)
    {
      case Glib::IO_STATUS_NORMAL:
        break;

      case Glib::IO_STATUS_ERROR:
        nltools::Log::error("Glib::IO_STATUS_ERROR:", __PRETTY_FUNCTION__);
        break;

      case Glib::IO_STATUS_EOF:
        nltools::Log::error("Glib::IO_STATUS_EOF:", __PRETTY_FUNCTION__);
        break;

      case Glib::IO_STATUS_AGAIN:
        nltools::Log::warning("file io channel is busy, trying again...", numBytes, numBytesWritten);
        g_usleep(1000);
        return write(bytes, numBytes, numTry + 1);
    }

    if(numBytes != numBytesWritten)
      nltools::Log::error("Not all Bytes written:", __PRETTY_FUNCTION__, "(", numBytes, " != ", numBytesWritten, ")");

    m_channel->flush();
  }
}
