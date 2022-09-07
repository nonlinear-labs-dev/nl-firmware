#pragma once
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <sigc++/slot.h>
#include <tools/Signal.h>

class RecorderManager : public sigc::trackable
{
 public:
  RecorderManager();
  sigc::connection subscribeToNotifyNoRecorderUIsLeftAndStillPlaying(sigc::slot<void(void)> s);
  static void stopRecorderPlayback();

 private:
  Signal<void> m_signal;
};

