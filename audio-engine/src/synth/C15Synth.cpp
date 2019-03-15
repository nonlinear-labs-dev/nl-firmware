#include "C15Synth.h"
#include "c15-audio-engine/dsp_host.h"
#include "main.h"
#include "Options.h"

C15Synth::C15Synth()
    : m_dsp(std::make_unique<dsp_host>())
{
  m_dsp->init(getOptions()->getSampleRate(), getOptions()->getPolyphony());
}

C15Synth::~C15Synth() = default;

void C15Synth::doMidi(const MidiEvent &event)
{
#if test_inputModeFlag
  m_dsp->testMidi(event.raw[0], event.raw[1], event.raw[2]);
#else
  m_dsp->evalMidi(event.raw[0], event.raw[1], event.raw[2]);
#endif
}

void C15Synth::doAudio(SampleFrame *target, size_t numFrames)
{
  for(size_t i = 0; i < numFrames; i++)
  {
    m_dsp->tickMain();
    target[i].left = m_dsp->m_mainOut_L;
    target[i].right = m_dsp->m_mainOut_R;
  }
}

void C15Synth::printAndResetTcdInputLog()
{
  auto cp = m_dsp->m_tcd_input_log;
  m_dsp->m_tcd_input_log.reset();

  std::cout << std::endl << "TCD MIDI Input Log: (Length: " << cp.m_length << ")" << std::endl;

  if(cp.m_length > 0)
  {
    std::cout << "(elapsed Samples\t: Command Argument, ...) - Argument is always unsigned 14 bit" << std::endl;
    uint32_t idx = cp.m_startPos;
    for(uint32_t i = 0; i < cp.m_length; i++)
    {
      if(cp.m_entry[idx].m_time > 0)
      {
        std::cout << std::endl;
        std::cout << cp.m_entry[idx].m_time << "\t: ";
      }
      std::cout << tcd_command_names[cp.m_entry[idx].m_cmdId] << " ";
      std::cout << cp.m_entry[idx].m_arg << ", ";
      idx = (idx + 1) % tcd_log_buffersize;
    }
    std::cout << std::endl << "\nEnd of TCD MIDI Input Log" << std::endl;
  }
}

void C15Synth::resetDSP()
{
  m_dsp->resetDSP();
  std::cout << std::endl << "DSP has been reset." << std::endl;
}

void C15Synth::toggleTestTone()
{
  auto tmp = static_cast<float>(1 - m_dsp->m_test_tone.m_state);
  m_dsp->m_decoder.m_utilityId = 4;
  m_dsp->utilityUpdate(tmp);
  std::cout << std::endl << "Test Tone toggled (" << m_dsp->m_test_tone.m_state << ")" << std::endl;
}

void C15Synth::selectTestToneFrequency()
{
  m_dsp->m_test_tone.m_focus = 0;
  std::cout << std::endl << "Test Tone: Frequency:\t" << m_dsp->m_test_tone.a_frequency << " Hz" << std::endl;
}
