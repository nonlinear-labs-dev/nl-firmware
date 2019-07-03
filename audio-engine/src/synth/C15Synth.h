#pragma once

#include "Synth.h"
#include <sigc++/sigc++.h>

namespace nltools
{
  namespace msg
  {
    class ParameterChangedMessage;
  }
}

class dsp_host;

class C15Synth : public Synth, public sigc::trackable
{
 public:
  C15Synth();
  ~C15Synth() override;

  void doMidi(const MidiEvent &event) override;
  void doAudio(SampleFrame *target, size_t numFrames) override;

  void printAndResetTcdInputLog();
  void resetDSP();
  void toggleTestTone();
  void selectTestToneFrequency();
  void selectTestToneAmplitude();
  void increase();
  void decrease();

  double measurePerformance(std::chrono::seconds time) override;

 private:
  void changeSelectedValueBy(int i);
  void onParameterMessage(const nltools::msg::ParameterChangedMessage &msg);

  std::unique_ptr<dsp_host> m_dsp;
};
