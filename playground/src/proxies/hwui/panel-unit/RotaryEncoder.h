#pragma once

#include "playground.h"
#include "proxies/hwui/HWUIEnums.h"
#include <tools/Throttler.h>
#include <chrono>

class RotaryEncoder
{
  public:
    class Receiver
    {
      public:
        virtual bool onRotary (int inc, ButtonModifiers modifiers) = 0;
    };

    RotaryEncoder ();
    virtual ~RotaryEncoder ();

    typedef signed char tIncrement;

    void fake (tIncrement amount);

    sigc::connection onRotaryChanged (function<void (tIncrement)> slot);

    static void registerTests ();

  private:
    void open ();
    void onRotaryFileOpened (Glib::RefPtr<Gio::AsyncResult>& result, RefPtr<Gio::File> rotaryFile);
    void readRotary (Glib::RefPtr<Gio::FileInputStream> stream);
    void onRotaryFileRead (Glib::RefPtr<Gio::AsyncResult>& result, Glib::RefPtr<Gio::FileInputStream> stream);
    tIncrement speedUp (tIncrement inc);

    Signal<void, tIncrement> m_signalRotaryChanged;
    RefPtr<Gio::Cancellable> m_readersCancel;
    Throttler m_throttler;
    int m_accumulatedIncs = 0;

    sigc::connection m_stress;
};
