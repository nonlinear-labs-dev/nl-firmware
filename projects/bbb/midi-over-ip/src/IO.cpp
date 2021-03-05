#include "IO.h"
#include <nltools/threading/Threading.h>
#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>

IO::IO(snd_rawmidi_t *handle)
    : handle(handle)
{
}

IO::IO(IO &&i)
    : handle(std::exchange(i.handle, nullptr))
{
}

IO::~IO()
{
  if(handle)
    snd_rawmidi_close(handle);
}

Input::Input(Input &&i)
    : IO(std::move(i))
    , m_bg(std::move(i.m_bg))
{
}

Input::Input(const std::string &name)
    : IO(open(name))
{
  pipe(m_cancelPipe);
  m_bg = std::async(std::launch::async, [=] { readMidi(); });
}

Input::~Input()
{
  m_quit = true;
  uint8_t v = 0;
  write(m_cancelPipe[1], &v, 1);
  m_bg.wait();
}

void Input::readMidi()
{
  using namespace nltools::msg;

  nltools::threading::setThisThreadPrio(nltools::threading::Priority::AlmostRealtime);

  uint8_t buffer[4096];
  snd_seq_event_t event;
  snd_midi_event_t *encoder = nullptr;
  snd_midi_event_t *decoder = nullptr;

  snd_midi_event_new(1024, &encoder);  // allocate only once
  snd_midi_event_new(1024, &decoder);  // allocate only once

  snd_midi_event_no_status(decoder, 1);

  snd_rawmidi_nonblock(handle, 1);

  int numPollFDs = snd_rawmidi_poll_descriptors_count(handle);

  pollfd pollFileDescriptors[numPollFDs + 1];
  numPollFDs = snd_rawmidi_poll_descriptors(handle, pollFileDescriptors, numPollFDs);
  pollFileDescriptors[numPollFDs].fd = m_cancelPipe[0];
  pollFileDescriptors[numPollFDs].events = POLLIN;

  while(!m_quit)
  {
    switch(poll(pollFileDescriptors, numPollFDs + 1, -1))
    {
      case POLLPRI:
      case POLLRDHUP:
      case POLLERR:
      case POLLHUP:
      case POLLNVAL:
        nltools::Log::error("Polling the midi input file descriptor failed. Terminating.");
        m_quit = true;
        break;
      default:
        break;
    }

    if(m_quit)
      goto _exit;

    while(true)
    {
      auto readResult = snd_rawmidi_read(handle, buffer, sizeof buffer);

      if(readResult == -EAGAIN)  // nothing remaining
        break;

      if(readResult < 0)
      {
        nltools::Log::error("Could not read from midi input file descriptor =>", snd_strerror(readResult));
        goto _exit;
      }

      for(unsigned i = 0; i < readResult; i++)
      {
        if(snd_midi_event_encode_byte(encoder, buffer[i], &event) == 1)
        {  // event fully decoded
          if(event.type < SND_SEQ_EVENT_SONGPOS)
          {  // is relevant event for us
            Midi::SimpleMessage msg;
            msg.numBytesUsed = std::min(3l, snd_midi_event_decode(decoder, msg.rawBytes.data(), 3, &event));
            send(EndPoint::ExternalMidiOverIPClient, msg);
          }
        }
      }
    }
  }

_exit:
  snd_midi_event_free(encoder);
  snd_midi_event_free(decoder);
}

snd_rawmidi_t *Input::open(const std::string &name)
{
  snd_rawmidi_t *h = nullptr;
  snd_rawmidi_open(&h, nullptr, name.c_str(), SND_RAWMIDI_NONBLOCK);
  return h;
}

Output::Output(Output &&i)
    : IO(std::move(i))
{
}

Output::Output(const std::string &name)
    : IO(open(name))
{
}

void Output::send(const nltools::msg::Midi::SimpleMessage &msg)
{
  if(auto res = snd_rawmidi_write(handle, msg.rawBytes.data(), msg.numBytesUsed))
  {
    if(size_t(res) != msg.numBytesUsed)
      nltools::Log::error("Could not write message into midi output device");
    else
      snd_rawmidi_drain(handle);
  }
}

snd_rawmidi_t *Output::open(const std::string &name)
{
  snd_rawmidi_t *h = nullptr;
  snd_rawmidi_open(nullptr, &h, name.c_str(), SND_RAWMIDI_NONBLOCK);
  return h;
}
