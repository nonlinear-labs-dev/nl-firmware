#pragma once

#include "Synth.h"
#include "c15-audio-engine/dsp_host_dual.h"
#include <nltools/messaging/Message.h>
#include <sigc++/sigc++.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <nltools/threading/BackgroundThreadWaiter.h>
#include <MidiRuntimeOptions.h>
#include <synth/input/InputEventStage.h>
#include <synth/input/MidiChannelModeMessages.h>

namespace nltools
{
  namespace msg
  {
    struct ParameterChangedMessage;
    struct SetPresetMessage;
  }
}

class dsp_host_dual;
class AudioEngineOptions;

class C15Synth : public Synth, public sigc::trackable
{
 public:
  explicit C15Synth(AudioEngineOptions* options);
  ~C15Synth() override;

  dsp_host_dual* getDsp() const;

  void doMidi(const MidiEvent& event) override;
  void doTcd(const MidiEvent& event) override;
  void doAudio(SampleFrame* target, size_t numFrames) override;

  void resetDSP() override;
  void logStatus();
  void toggleTestTone();
  void onSettingInitialSinglePreset();
  void onLayerPresetMessage(const nltools::msg::LayerPresetMessage& msg);
  void onSplitPresetMessage(const nltools::msg::SplitPresetMessage& msg);
  void onSinglePresetMessage(const nltools::msg::SinglePresetMessage& msg);

  void onModulateableParameterMessage(const nltools::msg::ModulateableParameterChangedMessage& msg);
  void onUnmodulateableParameterMessage(const nltools::msg::UnmodulateableParameterChangedMessage& msg);
  void onMacroControlParameterMessage(const nltools::msg::MacroControlChangedMessage& msg);
  void onHWAmountMessage(const nltools::msg::HWAmountChangedMessage& msg);
  void onHWSourceMessage(const nltools::msg::HWSourceChangedMessage& msg);
  void onHWSourceSendMessageReceived(const nltools::msg::HWSourceSendChangedMessage& msg);

  void onNoteShiftMessage(const nltools::msg::Setting::NoteShiftMessage& msg);
  void onPresetGlitchMessage(const nltools::msg::Setting::PresetGlitchMessage& msg);
  void onTransitionTimeMessage(const nltools::msg::Setting::TransitionTimeMessage& msg);
  void onEditSmoothingTimeMessage(const nltools::msg::Setting::EditSmoothingTimeMessage& msg);
  void onTuneReferenceMessage(const nltools::msg::Setting::TuneReference& msg);

  void onMidiSettingsMessage(const nltools::msg::Setting::MidiSettingsMessage& msg);
  void onPanicNotificationReceived(const nltools::msg::PanicAudioEngine& msg);

  unsigned int getRenderedSamples();

 private:
  void queueExternalMidiOut(const dsp_host_dual::SimpleRawMidiMessage& m);
  void queueChannelModeMessage(MidiChannelModeMessages function);

  void syncExternalsLoop();

  void doSyncExternalMidiBridge();
  void doSyncPlayground();
  void doChannelModeMessageFunctions();

  std::unique_ptr<dsp_host_dual> m_dsp;
  AudioEngineOptions* m_options;
  MidiRuntimeOptions m_midiOptions;

  //Latch-Filters, Queues
  std::array<std::array<float, 3>, 8> m_playgroundHwSourceKnownValues {};
  RingBuffer<nltools::msg::Midi::SimpleMessage> m_externalMidiOutBuffer;
  RingBuffer<MidiChannelModeMessages> m_queuedChannelModeMessages;

  InputEventStage m_inputEventStage;

  //Concurrency
  std::condition_variable m_syncExternalsWaiter;
  std::mutex m_syncExternalsMutex;
  std::atomic<bool> m_quit { false };
  std::future<void> m_syncExternalsTask;
};
