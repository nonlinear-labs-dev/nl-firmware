#include "synth/Synth.h"
#include "io/audio/AudioOutput.h"
#include "io/midi/AlsaMidiInput.h"
#include "io/midi/TestMidiInput.h"
#include "Options.h"
#include "main.h"

#include <iostream>
#include <chrono>

Synth::Synth()
{
  auto options = getOptions();
  auto outDeviceName = options->getAudioOutputDeviceName();
  m_out = std::make_unique<AudioOutput>(outDeviceName, [=](auto buf, auto length) { process(buf, length); });

  if(auto distance = options->testNotesDistance())
  {
    m_in = std::make_unique<TestMidiInput>(distance, [=](auto event) { pushMidiEvent(event); });
  }
  else
  {
    auto inDeviceName = options->getMidiInputDeviceName();
    m_in = std::make_unique<AlsaMidiInput>(inDeviceName, [=](auto event) { pushMidiEvent(event); });
  }
}

Synth::~Synth() = default;

void Synth::start()
{
  m_in->start();
  m_out->start();
}

void Synth::stop()
{
  m_in->stop();
  m_out->stop();
}

const AudioOutput *Synth::getAudioOut() const
{
  return m_out.get();
}

void Synth::pushMidiEvent(const MidiEvent &event)
{
  auto &c = m_midiRingBuffer.push(event);
  c.timestamp = std::chrono::high_resolution_clock::now() + m_out->getLatency();
}

void Synth::process(SampleFrame *target, size_t numFrames)
{
  constexpr auto nanoRec = 1.0 / std::nano::den;
  const auto sr = getOptions()->getSampleRate();
  const auto nanosToSamples = sr * nanoRec;

  auto now = std::chrono::high_resolution_clock::now();

  uint32_t done = 0;

  while(done < numFrames)
  {
    if(auto e = m_midiRingBuffer.peek())
    {
      auto eventPos = e->timestamp;
      auto diffInNanos = eventPos - now;
      auto diffInSamples = static_cast<int32_t>(diffInNanos.count() * nanosToSamples);

      if(diffInSamples <= static_cast<int32_t>(done))
      {
        doMidi(*e);
        m_midiRingBuffer.pop();
      }
      else
      {
        auto todoNow = std::min<size_t>(numFrames, static_cast<size_t>(diffInSamples) - done);
        doAudio(target + done, todoNow);
        done += todoNow;
      }
    }
    else
    {
      doAudio(target + done, numFrames);
      done += numFrames;
    }
  }
}
