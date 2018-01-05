#pragma once

#include "playground.h"
#include <atomic>
#include <sigc++/signal.h>

using namespace Glib;
using namespace std;
using namespace sigc;

template<typename tFirst, typename ... tArgs>
  class Signal : public sigc::signal<tFirst, tArgs...>
  {
    private:
      typedef sigc::signal<tFirst, tArgs...> super;

    public:

      Signal () :
          m_initCallbackScheduled (false)
      {
      }

      virtual ~Signal ()
      {
        m_initCallbackConnection.disconnect ();
      }

      sigc::connection connectAndInit (const typename super::slot_type &slot, const tArgs &... args)
      {
        auto cb = std::bind (&super::slot_type::operator(), slot, args...);
        scheduleInitCallback (cb);
        return super::connect (slot);
      }

      typename super::result_type send (tArgs ... args)
      {
        m_initRecords.clear ();
        return this->emit (args...);
      }

      static void registerTests ();

    private:
      Signal (const Signal &other);
      Signal &operator= (const Signal &);

      typedef function<void ()> tCallback;
      typedef tCallback tRecord;
      typedef list<tCallback> tInitRecords;
      tInitRecords m_initRecords;
      atomic<bool> m_initCallbackScheduled;
      sigc::connection m_initCallbackConnection;

      using super::emit;
      using super::operator();

      void scheduleInitCallback (tCallback cb)
      {
        m_initRecords.push_back (cb);

        if (!m_initCallbackScheduled.exchange (true))
        {
          m_initCallbackConnection = MainContext::get_default ()->signal_idle ().connect (mem_fun (this, &Signal::doTheCallbacks), Glib::PRIORITY_HIGH);
        }
      }

      bool doTheCallbacks ()
      {
        m_initCallbackScheduled.exchange (false);

        tInitRecords initRecords;
        swap (initRecords, m_initRecords);

        m_initCallbackConnection.disconnect ();

        for (const auto & r : initRecords)
          r ();

        return false;
      }
  };

