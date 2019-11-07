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

  receive<SinglePresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onSinglePresetMessage));
  receive<LayerPresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onLayerPresetMessage));
  receive<SplitPresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onSplitPresetMessage));

  receive<UnmodulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                                 sigc::mem_fun(this, &C15Synth::onUnmodulateableParameterMessage));
  receive<ModulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                               sigc::mem_fun(this, &C15Synth::onModulateableParameterMessage));
  receive<MacroControlChangedMessage>(EndPoint::AudioEngine,
                                      sigc::mem_fun(this, &C15Synth::onMacroControlParameterMessage));
  receive<HWSourceChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWSourceMessage));
  receive<HWAmountChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWAmountMessage));

  //Settings
  receive<Setting::NoteShiftMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onNoteShiftMessage));
  receive<Setting::PresetGlitchMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onPresetGlitchMessage));
  receive<Setting::TransitionTimeMessage>(EndPoint::AudioEngine,
                                          sigc::mem_fun(this, &C15Synth::onTransitionTimeMessage));
  receive<Setting::EditSmoothingTimeMessage>(EndPoint::AudioEngine,
                                             sigc::mem_fun(this, &C15Synth::onEditSmoothingTimeMessage));
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

void C15Synth::onSplitPresetMessage(const nltools::msg::SplitPresetMessage &msg)
{
  nltools::Log::info("Received Split Preset Message!");
}

void C15Synth::onSinglePresetMessage(const nltools::msg::SinglePresetMessage &msg)
{
  nltools::Log::info("Received Single Preset Message!");
}

void C15Synth::onLayerPresetMessage(const nltools::msg::LayerPresetMessage &msg)
{
  nltools::Log::info("Received Layer Preset Message!");
}

void C15Synth::onNoteShiftMessage(const nltools::msg::Setting::NoteShiftMessage &msg)
{
  nltools::Log::info("Received NoteShift Message!");
}

void C15Synth::onPresetGlitchMessage(const nltools::msg::Setting::PresetGlitchMessage &msg)
{
  nltools::Log::info("Received Preset Glitch Suppression Message!");
}

void C15Synth::onTransitionTimeMessage(const nltools::msg::Setting::TransitionTimeMessage &msg)
{
  nltools::Log::info("Received Transition Time Message!");
}

void C15Synth::onEditSmoothingTimeMessage(const nltools::msg::Setting::EditSmoothingTimeMessage &msg)
{
  nltools::Log::info("Received EditSmoothing Time Message!");
}
