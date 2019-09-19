#include "C15Synth.h"
#include "c15-audio-engine/dsp_host_dual.h"
#include "main.h"
#include "Options.h"

#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>

C15Synth::C15Synth()
    : m_dsp(std::make_unique<dsp_host_dual>())
{
  m_dsp->init(getOptions()->getSampleRate(), getOptions()->getPolyphony());

  using namespace nltools::msg;

  receive<SetPresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onPresetMessage));

  receive<UnmodulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                                 sigc::mem_fun(this, &C15Synth::onUnmodulateableParameterMessage));
  receive<ModulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                               sigc::mem_fun(this, &C15Synth::onModulateableParameterMessage));
  receive<MacroControlChangedMessage>(EndPoint::AudioEngine,
                                      sigc::mem_fun(this, &C15Synth::onMacroControlParameterMessage));
  receive<HWSourceChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWSourceMessage));
  receive<HWAmountChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWAmountMessage));
}

C15Synth::~C15Synth() = default;

void C15Synth::doMidi(const MidiEvent &event)
{
#if test_inputModeFlag
  //m_dsp->onMidiMessage(event.raw[0], event.raw[1], event.raw[2]);
#else
  m_dsp->onMidiMessage(event.raw[0], event.raw[1], event.raw[2]);
#endif
}

void C15Synth::doAudio(SampleFrame *target, size_t numFrames)
{
  for(size_t i = 0; i < numFrames; i++)
  {
    m_dsp->render();
    target[i].left = m_dsp->m_mainOut_L;
    target[i].right = m_dsp->m_mainOut_R;
  }
}

void C15Synth::printAndResetTcdInputLog()
{
    nltools::Log::info("currently disabled");
}

void C15Synth::resetDSP()
{
  m_dsp->reset();
  nltools::Log::info("DSP has been reset.");
}

void C15Synth::toggleTestTone()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::selectTestToneFrequency()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::selectTestToneAmplitude()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::increase()
{
  changeSelectedValueBy(1);
}

void C15Synth::decrease()
{
  changeSelectedValueBy(-1);
}

double C15Synth::measurePerformance(std::chrono::seconds time)
{
  for(int i = 0; i < getOptions()->getPolyphony(); i++)
  {
    m_dsp->onMidiMessage(1, 0, 53);
    m_dsp->onMidiMessage(5, 62, 64);
    m_dsp->onMidiMessage(1, 0, 83);
    m_dsp->onMidiMessage(5, 62, 64);
  }
  return Synth::measurePerformance(time);
}

void C15Synth::changeSelectedValueBy(int i)
{
  nltools::Log::info("currently disabled");
}

void C15Synth::onPresetMessage(const nltools::msg::SetPresetMessage &msg)
{
  nltools::Log::notify("got preset message!");
}

void C15Synth::onModulateableParameterMessage(const nltools::msg::ModulateableParameterChangedMessage &msg)
{
  nltools::Log::info("Received modulateable parameter message!");
}

void C15Synth::onUnmodulateableParameterMessage(const nltools::msg::UnmodulateableParameterChangedMessage &msg)
{
  nltools::Log::info("Received unmodulateable parameter message!");
}

void C15Synth::onMacroControlParameterMessage(const nltools::msg::MacroControlChangedMessage &msg)
{
  nltools::Log::info("Received macro control parameter message!");
}

void C15Synth::onHWAmountMessage(const nltools::msg::HWAmountChangedMessage &msg)
{
  nltools::Log::info("Received hwAmount parameter message!");
}

void C15Synth::onHWSourceMessage(const nltools::msg::HWSourceChangedMessage &msg)
{
  nltools::Log::info("Received hwSource parameter message!");
}
