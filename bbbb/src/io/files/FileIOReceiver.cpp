#include <io/files/FileIOReceiver.h>

FileIOReceiver::FileIOReceiver(const char *path, size_t blockSize) :
    m_cancel(Gio::Cancellable::create()),
    m_blockSize(blockSize)
{
  auto file = Gio::File::create_for_path(path);
  file->read_async(sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onFileOpened), file), m_cancel);
}

FileIOReceiver::~FileIOReceiver()
{
}

void FileIOReceiver::onFileOpened(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::File> buttonsFile)
{
  try
  {
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
  stream->read_bytes_async(m_blockSize, sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onButtonsFileRead), stream), m_cancel);
}

void FileIOReceiver::onButtonsFileRead(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::FileInputStream> stream)
{
  TRACE(__PRETTY_FUNCTION__);
  Glib::RefPtr<Glib::Bytes> bytes = stream->read_bytes_finish(result);
  onDataReceived(bytes);
  readStream(stream);
}
