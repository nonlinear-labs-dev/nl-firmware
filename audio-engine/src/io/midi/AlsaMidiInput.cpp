#include "AlsaMidiInput.h"
#include "io/Log.h"
#include <iostream>

AlsaMidiInput::AlsaMidiInput(const std::string &deviceName, Callback cb)
    : MidiInput(cb)
    , m_cancellable(Gio::Cancellable::create())
{
  open(deviceName);
}

AlsaMidiInput::~AlsaMidiInput()
{
  close();
}

void AlsaMidiInput::open(const std::string &deviceName)
{
  if(snd_rawmidi_open(&m_handle, nullptr, deviceName.c_str(), SND_RAWMIDI_NONBLOCK))
    Log::error("Could not open midi device");
}

void AlsaMidiInput::start()
{
  m_run = true;
  m_bgThread = std::thread([=]() { doBackgroundWork(); });
}

void AlsaMidiInput::stop()
{
  m_run = false;

  m_cancellable->cancel();

  if(m_bgThread.joinable())
    m_bgThread.join();
}

void AlsaMidiInput::close()
{
  if(auto h = std::exchange(m_handle, nullptr))
    snd_rawmidi_close(h);
}

void AlsaMidiInput::prioritizeThread()
{
  struct sched_param param;
  param.sched_priority = 50;

  if(auto r = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param))
    Log::warning("Could not set thread priority - consider 'sudo setcap 'cap_sys_nice=eip' <application>'");
}

void AlsaMidiInput::setThreadAffinity()
{
  int coreID = 1;

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(coreID, &set);
  if(sched_setaffinity(0, sizeof(cpu_set_t), &set) < 0)
    Log::warning("Could not set thread affinity");
}

void AlsaMidiInput::doBackgroundWork()
{
  prioritizeThread();
  setThreadAffinity();

  uint8_t byte;
  snd_midi_event_t *parser = nullptr;

  snd_midi_event_new(128, &parser);

  int numPollFDs = snd_rawmidi_poll_descriptors_count(m_handle);

  pollfd pollFileDescriptors[numPollFDs + 1];
  numPollFDs = snd_rawmidi_poll_descriptors(m_handle, pollFileDescriptors, numPollFDs);
  pollFileDescriptors[numPollFDs].fd = m_cancellable->get_fd();
  pollFileDescriptors[numPollFDs].events = POLLIN;

  while(true)
  {
    poll(pollFileDescriptors, numPollFDs + 1, -1);

    if(!m_run)
      break;

    MidiEvent e;
    int rawIdx = 0;

    while(m_run && snd_rawmidi_read(m_handle, &byte, 1) == 1)
    {
      if(rawIdx < 3)
        e.raw[rawIdx++] = byte;

      snd_seq_event_t event;

      if(snd_midi_event_encode_byte(parser, byte, &event) == 1)
      {
        if(event.type != SND_SEQ_EVENT_NONE)
        {
          getCallback()(e);
        }

        rawIdx = 0;
      }
    }
  }
}
