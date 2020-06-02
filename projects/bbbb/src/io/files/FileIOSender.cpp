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
  if(m_channel)
  {
    try
    {
      gsize numBytesWritten = 0;
      auto ret = m_channel->write(bytes, static_cast<gssize>(numBytes), numBytesWritten);

      switch(ret)
      {
        case Glib::IO_STATUS_NORMAL:
          break;

        case Glib::IO_STATUS_ERROR:
          nltools::Log::error("An Glib::IO_STATUS_ERROR Error Occured!", __FILE__, __PRETTY_FUNCTION__);
          break;
        case Glib::IO_STATUS_EOF:
          nltools::Log::error("An Glib::IO_STATUS_EOF Error Occured!", __FILE__, __PRETTY_FUNCTION__);
          break;
        case Glib::IO_STATUS_AGAIN:
          nltools::Log::error(m_path, "Resource temporarily unavailable.", __FILE__, __PRETTY_FUNCTION__);
          break;
      }

      if(numBytes != numBytesWritten)
        nltools::Log::error("BBBB", __FILE__, __PRETTY_FUNCTION__, "Not all Bytes written! numBytes: ", numBytes,
                            " written: ", numBytesWritten);
      m_channel->flush();
    }
    catch(const Glib::IOChannelError &channelError)
    {
      nltools::Log::error("Exception: ", m_path, " -> ", channelError.what());
    }
    catch(const Glib::ConvertError &convertError)
    {
      nltools::Log::error("Exception: ", m_path, " -> ", convertError.what());
    }
    catch(Glib::Error &err)
    {
      nltools::Log::error("Exception: ", m_path, " -> ", err.what());
    }
  }
}
