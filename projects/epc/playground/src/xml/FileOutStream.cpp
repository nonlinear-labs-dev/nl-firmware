#include <http/UndoScope.h>
#include <fcntl.h>
#include <gio/gunixoutputstream.h>
#include <glib/gstdio.h>
#include <Options.h>
#include <sys/stat.h>
#include <unistd.h>
#include <xml/FileOutStream.h>
#include <tools/FileSystem.h>

#include <utility>
#include <nltools/logging/Log.h>

FileOutStream::FileOutStream(Glib::ustring fileName, bool zip)
    : m_filename(std::move(fileName))
{
  auto tmpFileName = getTmpFileName();
  g_unlink(tmpFileName.c_str());

  m_fileHandle = open(tmpFileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  auto fileStream = g_unix_output_stream_new(m_fileHandle, FALSE);
  auto buffered = g_buffered_output_stream_new(fileStream);

  if(zip)
  {
    auto compressor = g_zlib_compressor_new(GZlibCompressorFormat::G_ZLIB_COMPRESSOR_FORMAT_GZIP, 6);
    auto converter = g_converter_output_stream_new(buffered, G_CONVERTER(compressor));
    m_stream = g_data_output_stream_new(G_OUTPUT_STREAM(converter));
    g_object_unref(converter);
    g_object_unref(compressor);
  }
  else
  {
    m_stream = g_data_output_stream_new(buffered);
  }

  g_object_unref(buffered);
  g_object_unref(fileStream);
}

FileOutStream::~FileOutStream()
{
  if(m_autoCommit)
    commit();

  g_unlink(getTmpFileName().c_str());
}

Glib::ustring FileOutStream::getTmpFileName()
{
  return m_filename + "_tmp";
}

void FileOutStream::setAutoCommit(bool ac)
{
  m_autoCommit = ac;
}

void FileOutStream::commit()
{
  if(m_stream && m_fileHandle >= 0)
  {
    GError *error = nullptr;
    g_output_stream_close(G_OUTPUT_STREAM(m_stream), nullptr, &error);

    if(error)
    {
      nltools::Log::error(__PRETTY_FUNCTION__, __LINE__, error->message);
      g_error_free(error);
    }

    g_object_unref(m_stream);

    fsync(m_fileHandle);
    close(m_fileHandle);
    m_fileHandle = -1;

    auto oldName = getTmpFileName();
    FileSystem::rename(oldName, m_filename);
  }
}

void FileOutStream::implWrite(const Glib::ustring &str)
{
  if(m_stream && m_fileHandle > 0)
  {
    GError *error = nullptr;

    g_data_output_stream_put_string(m_stream, str.c_str(), nullptr, &error);

    if(error)
    {
      nltools::Log::error(error->message);
      g_error_free(error);
    }
  }
}

void FileOutStream::implWrite(const void *buf, size_t numBytes)
{
  if(m_stream && m_fileHandle > 0)
  {
    GError *error = nullptr;

    gsize bytesWritten = 0;

    if(!g_output_stream_write_all(G_OUTPUT_STREAM(m_stream), buf, numBytes, &bytesWritten, nullptr, &error))
    {
      if(error)
      {
        nltools::Log::error(error->message);
        g_error_free(error);
      }
      throw std::runtime_error("writing to file failed");
    }
  }
}
