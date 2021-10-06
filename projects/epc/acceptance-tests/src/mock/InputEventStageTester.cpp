#include "InputEventStageTester.h"
#include <synth/input/InputEventStage.h>
#include <synth/input/MIDIDecoder.h>
#include <synth/input/TCDDecoder.h>

InputEventStageTester::InputEventStageTester(InputEventStage* stage)
    : m_inputStage { stage }
{
}

MidiRuntimeOptions* InputEventStageTester::getOptions()
{
  return m_inputStage->m_options;
}

void InputEventStageTester::onTCDMessage(const MidiEvent& tcdEvent)
{
  m_inputStage->onTCDMessage(tcdEvent);
}

void InputEventStageTester::onMIDIMessage(const MidiEvent& midiEvent)
{
  m_inputStage->onMIDIMessage(midiEvent);
}

DSPInterface::InputEventSource InputEventStageTester::getInputSourceFromParsedChannel(MidiReceiveChannel channel)
{
  return m_inputStage->getInputSourceFromParsedChannel(channel);
}
