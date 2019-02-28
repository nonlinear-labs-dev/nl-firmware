#include "C15Synth.h"
#include "c15-audio-engine/dsp_host.h"
#include "main.h"
#include "Options.h"

C15Synth::C15Synth()
    : m_dsp(std::make_unique<dsp_host>())
{
  m_dsp->init(getOptions()->getSampleRate(), getOptions()->getPolyphony());
}

C15Synth::~C15Synth()
{
}

void C15Synth::doMidi(const MidiEvent &event)
{
#if test_inputModeFlag
  m_dsp->testMidi(event.data.raw8.d[0], event.data.raw8.d[1], event.data.raw8.d[2]);
#else
  m_dsp->evalMidi(event.data.raw8.d[0], event.data.raw8.d[1], event.data.raw8.d[2]);
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
