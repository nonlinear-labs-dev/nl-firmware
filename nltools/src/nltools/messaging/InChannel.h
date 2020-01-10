#pragma once

#include <functional>
#include <nltools/GlibFwd.h>

namespace nltools
{
  namespace msg
  {
    using SerializedMessage = Glib::RefPtr<Glib::Bytes>;

    class InChannel
    {
     public:
      using Callback = std::function<void(const SerializedMessage &)>;

      InChannel(Callback cb);

      virtual ~InChannel();

     protected:
      void onMessageReceived(const SerializedMessage &) const;

     private:
      Callback m_cb;
    };
  }
}
