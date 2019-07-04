#include "InChannel.h"

namespace nltools
{
  namespace msg
  {

    InChannel::InChannel(Callback cb)
        : m_cb(cb)
    {
    }

    void InChannel::onMessageReceived(const SerializedMessage &msg) const
    {
      m_cb(msg);
    }
  }
}
