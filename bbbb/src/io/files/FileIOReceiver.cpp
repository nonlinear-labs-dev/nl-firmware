#include <io/files/FileIOReceiver.h>

FileIOReceiver::FileIOReceiver(const char *path, size_t blockSize) :
    m_cancel(Gio::Cancellable::create()),
    m_blockSize(blockSize)
{
  TRACE(__PRETTY_FUNCTION__ << " " << path);
  auto file = Gio::File::create_for_path(path);
  file->read_async(sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onFileOpened), file), m_cancel);
}

FileIOReceiver::~FileIOReceiver()
{
  m_cancel->cancel();
}

void FileIOReceiver::onFileOpened(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::File> buttonsFile)
{
  try
  {
    TRACE(__PRETTY_FUNCTION__ << " " << __LINE__);
    auto stream = buttonsFile->read_finish(result);
    readStream(stream);
  }
  catch(Gio::Error &error)
  {
    TRACE("Could not read from button input stream");
  }
}

void FileIOReceiver::readStream(Glib::RefPtr<Gio::FileInputStream> stream)
{
  TRACE(__PRETTY_FUNCTION__ << " " << __LINE__);
  stream->read_bytes_async(m_blockSize, sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onStreamRead), stream), m_cancel);
}

void FileIOReceiver::onStreamRead(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::FileInputStream> stream)
{
  TRACE(__PRETTY_FUNCTION__ << " " << __LINE__);
  Glib::RefPtr<Glib::Bytes> bytes = stream->read_bytes_finish(result);
  onDataReceived(bytes);
  readStream(stream);
}
