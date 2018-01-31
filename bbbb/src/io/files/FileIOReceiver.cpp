#include <io/files/FileIOReceiver.h>

FileIOReceiver::FileIOReceiver(const char *path, size_t blockSize) :
    m_cancel(Gio::Cancellable::create()),
    m_blockSize(blockSize)
{
  TRACE("open file for reading: " << path);
  auto file = Gio::File::create_for_path(path);
  file->read_async(sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onFileOpened), file), m_cancel);
}

FileIOReceiver::~FileIOReceiver()
{
  m_cancel->cancel();
}

void FileIOReceiver::onFileOpened(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::File> file)
{
  try
  {
    readStream(file->read_finish(result));
  }
  catch(...)
  {
    TRACE("Could not open file for reading: " << file->get_path());
  }
}

void FileIOReceiver::setBlockSize(size_t blockSize)
{
  m_blockSize = blockSize;
}

void FileIOReceiver::readStream(Glib::RefPtr<Gio::FileInputStream> stream)
{
  stream->read_bytes_async(m_blockSize, sigc::bind(sigc::mem_fun(this, &FileIOReceiver::onStreamRead), stream), m_cancel);
}

void FileIOReceiver::onStreamRead(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::FileInputStream> stream)
{
  onDataReceived(stream->read_bytes_finish(result));
  readStream(stream);
}
