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

  // Raw bytes buffer for snd_raw_midi_read()
  // The larger this buffer the more events MIDI events can be extraced in one go from a burst
  // of data. This also increases process granularity
  constexpr unsigned BUF_SIZE = 8192;
  uint8_t *buffer = new uint8_t[BUF_SIZE];

  snd_seq_event_t event;

  // parsers to encode/decode byte stream <--> midi event
  // these parsers run asynchronous to the main buffer data chunks --> must be global
  snd_midi_event_t *encoder = nullptr;
  snd_midi_event_t *decoder = nullptr;
  snd_midi_event_new(1024, &encoder);  // temporary buffer for encoding into midi events
  snd_midi_event_new(128, &decoder);   // re-decoded midi bytes per event to transmit to app (3, currently)

  snd_midi_event_no_status(decoder, 1);  // force full-qualified midi events (no "running status")
  snd_rawmidi_nonblock(handle, 1);       // make reads non-blocking

  int numPollFDs = snd_rawmidi_poll_descriptors_count(handle);
  pollfd pollFileDescriptors[numPollFDs + 1];
  numPollFDs = snd_rawmidi_poll_descriptors(handle, pollFileDescriptors, numPollFDs);
  pollFileDescriptors[numPollFDs].fd = m_cancelPipe[0];
  pollFileDescriptors[numPollFDs].events = POLLIN;

  while(!m_quit)
  {
    auto readResult = snd_rawmidi_read(handle, buffer, BUF_SIZE);  // try read a block of bytes, non-blocking

    if(readResult == -EAGAIN)  // nothing remaining or no data
    {
      auto pollResult = poll(pollFileDescriptors, numPollFDs + 1, -1);  // sleep until data is coming in
      if(pollResult == POLLPRI || pollResult == POLLRDHUP || pollResult == POLLERR || pollResult == POLLHUP
         || pollResult == POLLNVAL)
      {
        nltools::Log::error("Polling the midi input file descriptor failed. Terminating.");
        break;
      }
      continue;  // reading
    }

    if(readResult < 0)
    {
      nltools::Log::error("Could not read from midi input file descriptor =>", snd_strerror(readResult));
      break;
    }

    auto remaining = readResult;  // # of bytes
    auto currentBuffer = buffer;  // position in buffer

    while(remaining > 0)  // process a chunk of bytes
    {
      auto consumed = snd_midi_event_encode(encoder, currentBuffer, remaining, &event);  // encode next
      if(consumed <= 0)
      {
        nltools::Log::error("Could not encode stream into midi event =>", snd_strerror(consumed));
        break;
      }

      remaining -= consumed;
      currentBuffer += consumed;

      if(event.type < SND_SEQ_EVENT_SONGPOS)  // event complete and relevant for us
      {
        Midi::SimpleMessage msg;
        snd_midi_event_reset_decode(decoder);
        // reconvert to bytes
        msg.numBytesUsed = std::min(3l, snd_midi_event_decode(decoder, msg.rawBytes.data(), 3, &event));
        send(EndPoint::ExternalMidiOverIPClient, msg);
      }
    }
  }

  snd_midi_event_free(encoder);
  snd_midi_event_free(decoder);
  delete[] buffer;
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
