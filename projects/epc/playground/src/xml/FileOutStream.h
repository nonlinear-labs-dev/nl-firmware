#pragma once

#include "OutStream.h"

struct _GDataOutputStream;
typedef struct _GDataOutputStream GDataOutputStream;

class FileOutStream : public OutStream
{
 public:
  FileOutStream(Glib::ustring fileName, bool zip);
  ~FileOutStream() override;

  void commit();

 protected:
  void setAutoCommit(bool ac);

 private:
  void implWrite(const Glib::ustring &str) override;
  void implWrite(const void *buf, size_t numBytes) override;

  Glib::ustring getTmpFileName();

  Glib::ustring m_filename;
  GDataOutputStream *m_stream = nullptr;
  int m_fileHandle = -1;

  bool m_autoCommit = true;
};
