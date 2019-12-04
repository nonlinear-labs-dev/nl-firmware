#pragma once

#include "playground.h"
#include <giomm.h>

class MessageComposer
{
 public:
  MessageComposer(uint16_t type);
  virtual ~MessageComposer();

  void operator<<(const uint16_t &value);
  void operator<<(const gint16 &value);

  typedef Glib::Bytes tBuffer;
  typedef Glib::RefPtr<tBuffer> tBufferPtr;
  virtual tBufferPtr flush();

  virtual bool canReplace(const MessageComposer *other) const;

 private:
  Glib::RefPtr<Gio::DataOutputStream> m_stream;
};
