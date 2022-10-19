#include <xml/FileInStream.h>
#include <device-settings/DebugLevel.h>
#include <tools/FileSystem.h>
#include <giomm/file.h>
#include "tools/StringTools.h"

static bool doesZipFileExist(const Glib::ustring &fileName)
{
  return FileSystem::doesFileExist(fileName + ".zip");
}

static bool doesGzFileExist(const Glib::ustring &fileName)
{
  return FileSystem::doesFileExist(fileName + ".gz");
}

FileInStream::FileInStream(const Glib::ustring &fileName, bool tryZip)
{
  auto file = Gio::File::create_for_path(fileName);

  if(tryZip)
  {
    const auto isZip = StringTools::hasEnding(fileName, ".zip");
    const auto isGZ = StringTools::hasEnding(fileName, ".gz");
    const auto existsAsIs = FileSystem::doesFileExist(fileName);
    const auto zipOrGZExists = existsAsIs && (isZip || isGZ);

    if(doesZipFileExist(fileName))
      file = Gio::File::create_for_path(fileName + ".zip");
    else if(doesGzFileExist(fileName))
      file = Gio::File::create_for_path(fileName + ".gz");
    else if(!zipOrGZExists)
      tryZip = false;
  }

  if(GFileInputStream *fileStream = g_file_read(file->gobj(), nullptr, nullptr))
  {
    if(tryZip)
    {
      auto decompressor = g_zlib_decompressor_new(GZlibCompressorFormat::G_ZLIB_COMPRESSOR_FORMAT_GZIP);
      auto converter = g_converter_input_stream_new(G_INPUT_STREAM(fileStream), G_CONVERTER(decompressor));
      g_filter_input_stream_set_close_base_stream(G_FILTER_INPUT_STREAM(converter), true);
      m_stream = g_data_input_stream_new(G_INPUT_STREAM(converter));
      g_filter_input_stream_set_close_base_stream(G_FILTER_INPUT_STREAM(m_stream), true);
      g_object_unref(converter);
      g_object_unref(decompressor);
    }
    else
    {
      m_stream = g_data_input_stream_new(G_INPUT_STREAM(fileStream));
    }

    g_object_unref(fileStream);
  }
  else
  {
    DebugLevel::warning("Could not open file stream:", fileName);
  }

  if(!m_stream)
  {
    m_eof = true;
    DebugLevel::warning("FileInStream has no stream for file:", fileName);
  }
}

FileInStream::~FileInStream()
{
  if(m_stream)
    g_object_unref(m_stream);
}

Glib::ustring FileInStream::read()
{
  if(!m_stream)
  {
    m_eof = true;
    return "";
  }

  try
  {
    Glib::ustring str;
    GError *error = nullptr;

    if(char *tmp = g_data_input_stream_read_line_utf8(m_stream, 0, 0, &error))
    {
      str = tmp;
      str += '\n';
      g_free(tmp);
    }

    if(error)
    {
      nltools::Log::error(__PRETTY_FUNCTION__, __LINE__, "error:", error->message);
      g_error_free(error);
    }

    m_eof = str.empty();
    return str;
  }
  catch(...)
  {
    nltools::Log::error("exception on reading file");
    m_eof = true;
    return "";
  }
}

std::vector<uint8_t> FileInStream::readAll()
{
  std::vector<uint8_t> ret;

  while(m_stream)
  {
    gsize chunkSize = 8192;
    uint8_t data[chunkSize];
    gsize bytesRead = 0;
    GError *error = nullptr;

    g_input_stream_read_all(G_INPUT_STREAM(m_stream), (void *) data, chunkSize, &bytesRead, nullptr, &error);

    if(error)
    {
      DebugLevel::error(__PRETTY_FUNCTION__, __LINE__, error->message);
      g_error_free(error);
    }

    if(!bytesRead)
    {
      break;
    }

    ret.insert(ret.end(), data, data + bytesRead);
  }

  m_eof = true;
  return ret;
}

bool FileInStream::eof() const
{
  return m_eof;
}
