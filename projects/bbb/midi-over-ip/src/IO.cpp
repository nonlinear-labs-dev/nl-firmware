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

  // Raw bytes buffer for snd_raw_midi_read().
  // For large SysEx data coming in fast a fair amount of background buffer is really required
  constexpr unsigned RAWMIDI_BUF_SIZE = 32768;  // empirical value + safety margin

  // Maximum granularity of the main receive loop.
  // Typically, incoming data between polls will be in much smaller sized blocks.
  constexpr unsigned LOCAL_BUF_SIZE = 1024;  // non-critical value

  // Parser sizes
  constexpr unsigned ENCODER_SIZE = LOCAL_BUF_SIZE;  // byte stream --> midi events
  constexpr unsigned DECODER_SIZE = 128;             // midi event --> bytes

  uint8_t *buffer = new uint8_t[LOCAL_BUF_SIZE];
  snd_midi_event_t *encoder = nullptr;
  snd_midi_event_t *decoder = nullptr;
  size_t bufferReserve = RAWMIDI_BUF_SIZE;

  int numPollFDs = snd_rawmidi_poll_descriptors_count(handle);
  pollfd pollFileDescriptors[numPollFDs + 1];
  numPollFDs = snd_rawmidi_poll_descriptors(handle, pollFileDescriptors, numPollFDs);
  pollFileDescriptors[numPollFDs].fd = m_cancelPipe[0];
  pollFileDescriptors[numPollFDs].events = POLLIN;

  snd_rawmidi_nonblock(handle, 1);  // make reads non-blocking

  snd_rawmidi_params_t *pParams;
  snd_rawmidi_params_alloca(&pParams);
  snd_rawmidi_params_current(handle, pParams);
  snd_rawmidi_params_set_buffer_size(handle, pParams, RAWMIDI_BUF_SIZE);
  snd_rawmidi_params(handle, pParams);
  snd_rawmidi_params_current(handle, pParams);
  if(snd_rawmidi_params_get_buffer_size(pParams) != RAWMIDI_BUF_SIZE)
  {
    nltools::Log::error("Could not set up midi receive buffer");
    goto _Exit;
  }

  // Parsers to encode/decode byte stream <--> midi event
  // These parsers run asynchronous to the main buffer data chunks --> must be global
  snd_midi_event_new(ENCODER_SIZE, &encoder);
  snd_midi_event_new(DECODER_SIZE, &decoder);
  if(encoder == nullptr || decoder == nullptr)
  {
    nltools::Log::error("Could not set up midi parser buffers");
    goto _Exit;
  }

  snd_midi_event_no_status(decoder, 1);  // force full-qualified midi events (no "running status")

  snd_seq_event_t event;
  snd_rawmidi_status_t *pStatus;
  snd_rawmidi_status_alloca(&pStatus);

  while(!m_quit)
  {
    auto result = snd_rawmidi_status(handle, pStatus);
    if(result != 0)
    {  // unplugging event, typically
    _StreamError:
      nltools::Log::error("Could read data/status from midi input file descriptor =>", snd_strerror(result));
      goto _Exit;
    }

    if(snd_rawmidi_status_get_xruns(pStatus) > 0)
    {
      nltools::Log::error("raw_midi receive buffer overrun");
      snd_midi_event_reset_decode(decoder);  // purge any half-decoded events
    }

    auto currentBufferReserve = RAWMIDI_BUF_SIZE - snd_rawmidi_status_get_avail(pStatus);
    if(currentBufferReserve < bufferReserve)
    {
      bufferReserve = currentBufferReserve;
      nltools::Log::info("raw_midi receive buffer headroom : ", bufferReserve);
    }

    // try fetch a block of bytes, non-blocking
    result = snd_rawmidi_read(handle, buffer, LOCAL_BUF_SIZE);
    if(result == -EAGAIN)  // nothing remaining or no data
    {
      // now sleep until data is coming in ...
      if(poll(pollFileDescriptors, numPollFDs + 1, -1) > 0)
        continue;  // ... have new data, go read it

      nltools::Log::error("Polling the midi input file descriptor failed.");
      goto _Exit;
    }

    if(result < 0)  // catch errors
      goto _StreamError;

    auto remaining = result;      // # of bytes
    auto currentBuffer = buffer;  // position in buffer

    // process fetched chunk of bytes
    while(remaining > 0)
    {
      auto consumed = snd_midi_event_encode(encoder, currentBuffer, remaining, &event);
      if(consumed <= 0)
      {
        nltools::Log::error("Could not encode stream into midi event =>", snd_strerror(consumed));
        snd_midi_event_reset_encode(encoder);
        break;  // discard and continue reading
      }

      remaining -= consumed;
      currentBuffer += consumed;

      // TODO : Bundle available messages into one packet for less IP overhead ?
      if(event.type < SND_SEQ_EVENT_SONGPOS)
      {  // event is complete *and* is relevant for us --> reconvert event to 1..3 bytes raw data
        snd_midi_event_reset_decode(decoder);
        Midi::SimpleMessage msg;
        msg.numBytesUsed = std::min(3l, snd_midi_event_decode(decoder, msg.rawBytes.data(), 3, &event));
        send(EndPoint::ExternalMidiOverIPClient, msg);
      }
    }
  }  // while(!m_quit)

_Exit:
  if(encoder)
    snd_midi_event_free(encoder);
  if(decoder)
    snd_midi_event_free(decoder);
  delete[] buffer;
  m_quit = true;
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
