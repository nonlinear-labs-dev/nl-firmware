#pragma once

#include "Synth.h"
#include <nltools/messaging/Message.h>
#include <sigc++/sigc++.h>

namespace nltools
{
  namespace msg
  {
    struct ParameterChangedMessage;
    struct SetPresetMessage;
  }
}

class dsp_host_dual;

class C15Synth : public Synth, public sigc::trackable
{
 public:
  C15Synth();
  ~C15Synth() override;

  void doMidi(const MidiEvent &event) override;
  void doAudio(SampleFrame *target, size_t numFrames) override;

  void resetDSP() override;
  void printAndResetTcdInputLog();
  void toggleTestTone();
  void selectTestToneFrequency();
  void selectTestToneAmplitude();
  void increase();
  void decrease();

  double measurePerformance(std::chrono::seconds time) override;

 private:
  void changeSelectedValueBy(int i);
  void onPresetMessage(const nltools::msg::SetPresetMessage &msg);

  void onModulateableParameterMessage(const nltools::msg::ModulateableParameterChangedMessage& msg);
  void onUnmodulateableParameterMessage(const nltools::msg::UnmodulateableParameterChangedMessage& msg);
  void onMacroControlParameterMessage(const nltools::msg::MacroControlChangedMessage& msg);
  void onHWAmountMessage(const nltools::msg::HWAmountChangedMessage& msg);
  void onHWSourceMessage(const nltools::msg::HWSourceChangedMessage& msg);

  std::unique_ptr<dsp_host_dual> m_dsp;
};
