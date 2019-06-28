#pragma once

#include <functional>

namespace nltools
{
  namespace msg
  {
    class Message;

    class Receiver
    {
     public:
      using Callback = std::function<void(const Message *)>;
      Receiver(Callback cb);

     protected:
      void onMessageReceived(const Message *msg) const;

     private:
      Callback m_cb;
    };
  }
}
