#include "AudioEngineOptions.h"
#include "synth/C15Synth.h"
#include "synth/CPUBurningSynth.h"
#include "ui/CommandlinePerformanceWatch.h"
#include "io/MidiHeartBeat.h"
#include "io/audio/AudioOutputMock.h"
#include "io/audio/AlsaAudioOutput.h"
#include "io/midi/MidiInputMock.h"
#include "io/midi/AlsaMidiInput.h"

#include "recorder/Recorder.h"

#include <nltools/logging/Log.h>
#include <nltools/StringTools.h>

#include <glibmm.h>
#include <iostream>

#include "ui/C15_CLI.h"

static Glib::RefPtr<Glib::MainLoop> theMainLoop;
static std::unique_ptr<AudioEngineOptions> theOptions;
static nltools::msg::BufferUnderrunsChangedMessage lastSent = { 0, 0 };

static void quit(int)
{
  if(theMainLoop)
    theMainLoop->quit();
}

static void connectSignals()
{
  signal(SIGINT, quit);
  signal(SIGQUIT, quit);
  signal(SIGTERM, quit);
}

static const AudioEngineOptions *getOptions()
{
  return theOptions.get();
}

static void setupMessaging(const AudioEngineOptions *o)
{
  using namespace nltools::msg;

  auto highPrio = nltools::threading::Priority::AlmostRealtime;

  Configuration conf;
  conf.offerEndpoints = { { EndPoint::AudioEngine }, { EndPoint::ExternalMidiOverIPClient, highPrio } };
  conf.useEndpoints = { { EndPoint::Playground, o->getPlaygroundHost() },
                        { EndPoint::ExternalMidiOverIPBridge, o->getMidiBridgeHost(), highPrio } };

  nltools::msg::init(conf);
}

static std::unique_ptr<Synth> createSynth(AudioEngineOptions *options)
{
  if(options->getNumCpuBurningSines())
    return std::make_unique<CPUBurningSynth>(options);

  return std::make_unique<C15Synth>(options);
}

static std::unique_ptr<C15_CLI> createCLI(Synth *synth, AudioOutput *audioOut)
{
  if(auto c15 = dynamic_cast<C15Synth *>(synth))
    return std::make_unique<C15_CLI>(c15, audioOut);

  return nullptr;
}

static int measurePerformance()
{
  auto synth = std::make_unique<C15Synth>(theOptions.get());
  synth->measurePerformance(std::chrono::seconds(5));  // warm up
  auto result = std::get<1>(synth->measurePerformance(std::chrono::seconds(5)));
  nltools::Log::info("Audio engine performs at", result, "x realtime.");
  return EXIT_SUCCESS;
}

static std::unique_ptr<Recorder> createRecorder(int sampleRate)
{
  return std::make_unique<Recorder>(sampleRate);
}

static std::unique_ptr<AudioOutput> createAudioOut(const std::string &name, Synth *synth, Recorder *recorder)
{
  return name.empty() ? nullptr
                      : std::make_unique<AlsaAudioOutput>(theOptions.get(), name,
                                                          [synth, recorder](auto buf, auto length)
                                                          {
                                                            synth->process(buf, length);
                                                            recorder->process(buf, length);

                                                            for(auto i = 0; i < length; i++)
                                                            {
                                                              buf[i].left = std::clamp(buf[i].left, -1.0f, 1.0f);
                                                              buf[i].right = std::clamp(buf[i].right, -1.0f, 1.0f);
                                                            }
                                                          });
}

static std::unique_ptr<MidiInput> createMidiIn(const std::string &name, Synth *synth)
{
  return name.empty() ? nullptr
                      : std::make_unique<AlsaMidiInput>(name, [synth](auto event) { synth->pushMidiEvent(event); });
}

static std::unique_ptr<MidiInput> createTCDIn(const std::string &name, Synth *synth)
{
  return name.empty() ? nullptr
                      : std::make_unique<AlsaMidiInput>(name, [synth](auto event) { synth->pushTcdEvent(event); });
}

static bool checkBufferUnderruns(AudioOutput *out)
{
  using namespace nltools::msg;
  BufferUnderrunsChangedMessage msg { out->getNumUnderruns(), theOptions->getFramesPerPeriod() };

  if(lastSent.numUnderruns != msg.numUnderruns || lastSent.framesPerPeriod != msg.framesPerPeriod)
  {
    send<BufferUnderrunsChangedMessage>(EndPoint::Playground, msg);
    lastSent = msg;
  }
  return true;
}

sigc::connection connectFramesPerPeriodMessage(AlsaAudioOutput *out)
{
  return nltools::msg::receive<nltools::msg::SetFramesPerPeriod>(
      nltools::msg::EndPoint::AudioEngine,
      [out = out, defaultFramesPerPeriod = theOptions->getFramesPerPeriod()](const auto &msg)
      {
        auto desiredFPP = msg.framesPerPeriod ? msg.framesPerPeriod : defaultFramesPerPeriod;

        if(theOptions->getFramesPerPeriod() != desiredFPP)
        {
          theOptions->setFramesPerPeriod(desiredFPP);
          out->stop();
          out->close();
          out->open(theOptions->getAudioOutputDeviceName());
          out->start();
        }
      });
}

template <typename... A> static void start(A &...a)
{
  auto start = [](auto &p)
  {
    if(p)
      p->start();
  };
  (start(a), ...);
}

template <typename... A> static void stop(A &...a)
{
  auto stop = [](auto &p)
  {
    if(p)
      p->stop();
  };
  (..., stop(a));
}

int main(int args, char *argv[])
{
  Glib::init();
  connectSignals();

  testParallelData();

  theOptions = std::make_unique<AudioEngineOptions>(args, argv);
  setupMessaging(theOptions.get());

  if(theOptions->doMeasurePerformance())
    return measurePerformance();

  auto synth = createSynth(theOptions.get());
  auto recorder = createRecorder(theOptions->getSampleRate());
  auto audioOut = createAudioOut(theOptions->getAudioOutputDeviceName(), synth.get(), recorder.get());
  auto midiIn = createMidiIn(theOptions->getMidiInputDeviceName(), synth.get());
  auto tcdIn = createTCDIn(theOptions->getTcdInputDeviceName(), synth.get());
  auto cli = createCLI(synth.get(), audioOut.get());

  MidiHeartBeat heartbeat(theOptions->getHeartBeatDeviceName());
  CommandlinePerformanceWatch watch(audioOut.get());

  start(audioOut, midiIn, tcdIn);
  {
    theMainLoop = Glib::MainLoop::create();

    auto fpp = connectFramesPerPeriodMessage(dynamic_cast<AlsaAudioOutput *>(audioOut.get()));

    auto checkForXRunTimer = theMainLoop->get_context()->signal_timeout().connect_seconds(
        sigc::bind(sigc::ptr_fun(&checkBufferUnderruns), audioOut.get()), 1);

    auto pgConnected = nltools::msg::onConnectionEstablished(nltools::msg::EndPoint::Playground,
                                                             [&]() {
                                                               lastSent = { 0, 0 };
                                                             });
    
    theMainLoop->run();
  }
  stop(audioOut, midiIn, tcdIn);

  return EXIT_SUCCESS;
}
