#include "Options.h"
#include <nltools/messaging/Message.h>
#include <nltools/messaging/Messaging.h>
#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <vector>
#include <glibmm.h>

static bool quitApp = false;
static Glib::RefPtr<Glib::MainLoop> loop;
static int cancelPipe[2];
static std::vector<std::chrono::high_resolution_clock::time_point> sendTime;

void quit(int)
{
  quitApp = true;
  uint8_t v = 0;
  write(cancelPipe[1], &v, 1);
  loop->quit();
}

void readMidi(int cancelHandle, snd_rawmidi_t *inputHandle)
{
  using namespace nltools::msg;

  while(!quitApp)
  {
    uint8_t byte = 0;
    snd_midi_event_t *encoder = nullptr;
    snd_midi_event_new(128, &encoder);

    snd_midi_event_t *decoder = nullptr;
    snd_midi_event_new(128, &decoder);
    snd_midi_event_no_status(decoder, 1);

    int numPollFDs = snd_rawmidi_poll_descriptors_count(inputHandle);

    pollfd pollFileDescriptors[numPollFDs + 1];
    numPollFDs = snd_rawmidi_poll_descriptors(inputHandle, pollFileDescriptors, numPollFDs);
    pollFileDescriptors[numPollFDs].fd = cancelHandle;
    pollFileDescriptors[numPollFDs].events = POLLIN;

    while(true)
    {
      switch(poll(pollFileDescriptors, numPollFDs + 1, -1))
      {
        case POLLPRI:
        case POLLRDHUP:
        case POLLERR:
        case POLLHUP:
        case POLLNVAL:
          nltools::Log::error("Polling the midi input file descriptor failed. Terminating.");
          quitApp = true;
          break;

        default:
          break;
      }

      if(quitApp)
        break;

      snd_seq_event_t event;

      while(!quitApp)
      {
        auto readResult = snd_rawmidi_read(inputHandle, &byte, 1);

        if(readResult == 1)
        {
          if(snd_midi_event_encode_byte(encoder, byte, &event) == 1)
          {
            if(event.type != SND_SEQ_EVENT_NONE)
            {
              Midi::SimpleMessage msg;
              snd_midi_event_decode(decoder, msg.rawBytes, sizeof(msg.rawBytes), &event);
              sendTime.push_back(std::chrono::high_resolution_clock::now());
              msg.id = sendTime.size() - 1;
              send(EndPoint::AudioEngine, msg);
              break;
            }
          }
        }
        else if(readResult == -19)
        {
          nltools::Log::error("Could not read from midi input file descriptor =>", snd_strerror(readResult));
          quit(0);
        }
      }
    }
  }
}

int main(int args, char *argv[])
{
  using namespace nltools::msg;

  signal(SIGINT, quit);
  signal(SIGQUIT, quit);
  signal(SIGTERM, quit);

  Options options(args, argv);

  Configuration conf;
  conf.offerEndpoints = { EndPoint::ExternalMidiOverIP };
  conf.useEndpoints = { { EndPoint::AudioEngine, options.getAudioEngineHost() } };
  nltools::msg::init(conf);

  snd_rawmidi_t *inputHandle = nullptr;
  snd_rawmidi_t *outputHandle = nullptr;

  if(snd_rawmidi_open(&inputHandle, &outputHandle, options.getMidiDevice().c_str(), SND_RAWMIDI_NONBLOCK))
  {
    nltools::Log::error("Could not open midi device");
    return EXIT_FAILURE;
  }

  auto msg = receive<nltools::msg::Midi::SimpleMessage>(EndPoint::ExternalMidiOverIP, [&](const auto &msg) {
    if(auto res = snd_rawmidi_write(outputHandle, msg.rawBytes, sizeof(msg.rawBytes)))
      if(size_t(res) != sizeof(msg.rawBytes))
        nltools::Log::error("Could not write message into midi output device");

    snd_rawmidi_drain(outputHandle);
  });

  auto ack = receive<nltools::msg::Midi::MessageAcknowledge>(EndPoint::ExternalMidiOverIP, [&](const auto &msg) {
    auto flightTime = std::chrono::high_resolution_clock::now() - sendTime[msg.id];
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(flightTime);
    nltools::Log::warning("Received ack to event", msg.id, "after", micros.count(), "us");
  });

  if(pipe(cancelPipe) < 0)
  {
    nltools::Log::error("Could not create pipe for cancellation of the Midi thread");
    return EXIT_FAILURE;
  }

  auto sender = std::thread([&] { readMidi(cancelPipe[0], inputHandle); });

  loop = Glib::MainLoop::create();
  loop->run();

  if(sender.joinable())
    sender.join();

  msg.disconnect();
  ack.disconnect();

  snd_rawmidi_close(outputHandle);
  snd_rawmidi_close(inputHandle);
  return EXIT_SUCCESS;
}
