#pragma once

#include "InStream.h"

struct _GDataInputStream;
using GDataInputStream = struct _GDataInputStream;

class FileInStream : public InStream
{
 public:
  FileInStream(const Glib::ustring &fileName, bool tryZip);
  virtual ~FileInStream();

  Glib::ustring read() override;
  std::vector<uint8_t> readAll() override;
  bool eof() const;

 private:
  GDataInputStream *m_stream = nullptr;
  bool m_eof = false;
};
