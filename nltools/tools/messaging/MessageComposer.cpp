#include "MessageComposer.h"
#include "Message.h"

namespace nltools
{
  namespace msg
  {

    MessageComposer::MessageComposer(Senders s, Receivers r, MessageType t)
        : m_sender(s)
        , m_receiver(r)
        , m_msgType(t)
    {
    }

    MessageComposer::~MessageComposer() = default;

    void MessageComposer::add(const uint16_t &v)
    {
    }

    void MessageComposer::add(const float &v)
    {
    }

    void MessageComposer::send()
    {
      nltools::msg::send(m_sender, m_receiver, std::move(m_theMessage));
    }
  }
}
