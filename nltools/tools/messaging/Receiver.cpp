#include "Receiver.h"

namespace nltools
{
  namespace msg
  {

    Receiver::Receiver(Receiver::Callback cb)
        : m_cb(cb)
    {
    }

    void Receiver::onMessageReceived(const Message *msg) const
    {
      m_cb(msg);
    }
  }
}
