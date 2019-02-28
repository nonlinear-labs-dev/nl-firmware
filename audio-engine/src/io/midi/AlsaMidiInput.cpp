#include "AlsaMidiInput.h"
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
    std::cerr << "Could not open midi device" << std::endl;
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

void AlsaMidiInput::doBackgroundWork()
{
  struct sched_param param;
  param.sched_priority = 50;
  if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &param))
    std::cerr << "Could not set thread priority - consider 'sudo setcap 'cap_sys_nice=eip' <application>'" << std::endl;

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

    while(m_run && snd_rawmidi_read(m_handle, &byte, 1) == 1)
    {
      snd_seq_event_t event;

      if(snd_midi_event_encode_byte(parser, byte, &event) == 1)
        if(event.type != SND_SEQ_EVENT_NONE)
          getCallback()(event);
    }
  }
}
