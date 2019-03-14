#pragma once

#include "MidiInput.h"
#include <giomm.h>
#include <thread>

class AlsaMidiInput : public MidiInput
{
 public:
  using Callback = std::function<void(const MidiEvent &)>;

  AlsaMidiInput(const std::string &deviceName, Callback cb);
  ~AlsaMidiInput() override;

  void start() override;
  void stop() override;

 private:
  void open(const std::string &deviceName);
  void close();
  void doBackgroundWork();
  void prioritizeThread();
  void setThreadAffinity();

  Glib::RefPtr<Gio::Cancellable> m_cancellable;
  snd_rawmidi_t *m_handle = nullptr;
  std::thread m_bgThread;
  bool m_run = true;
};
