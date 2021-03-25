#include "InputEventStageTester.h"
#include <synth/input/InputEventStage.h>
#include <synth/input/MIDIDecoder.h>
#include <synth/input/TCDDecoder.h>

InputEventStageTester::InputEventStageTester(InputEventStage* stage)
    : m_inputStage { stage }
{
}

TCDDecoder* InputEventStageTester::getTCDDecoder()
{
  return &m_inputStage->m_tcdDecoder;
}

MIDIDecoder* InputEventStageTester::getMIDIDecoder()
{
  return &m_inputStage->m_midiDecoder;
}

DSPInterface* InputEventStageTester::getDSPHost()
{
  return m_inputStage->m_dspHost;
}

MidiRuntimeOptions* InputEventStageTester::getOptions()
{
  return m_inputStage->m_options;
}

KeyShift* InputEventStageTester::getShiftableKeys()
{
  return &m_inputStage->m_shifteable_keys;
}

bool InputEventStageTester::checkMIDIKeyEventEnabled(MIDIDecoder* pDecoder)
{
  return m_inputStage->checkMIDIKeyEventEnabled(pDecoder);
}

bool InputEventStageTester::checkMIDIHardwareChangeEnabled(MIDIDecoder* pDecoder)
{
  return m_inputStage->checkMIDIHardwareChangeEnabled(pDecoder);
}

void InputEventStageTester::onMIDIEvent(MIDIDecoder* decoder)
{
  m_inputStage->onMIDIEvent(decoder);
}

void InputEventStageTester::onTCDEvent(TCDDecoder* decoder)
{
  m_inputStage->onTCDEvent(decoder);
}

void InputEventStageTester::sendKeyDownAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart)
{
  m_inputStage->sendKeyDownAsMidi(pDecoder, determinedPart);
}

void InputEventStageTester::convertToAndSendMIDI(TCDDecoder* pDecoder, const VoiceGroup& determinedPart)
{
  m_inputStage->convertToAndSendMIDI(pDecoder, determinedPart);
}

void InputEventStageTester::sendKeyUpAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart)
{
  m_inputStage->sendKeyDownAsMidi(pDecoder, determinedPart);
}

void InputEventStageTester::sendHardwareChangeAsMidi(TCDDecoder* pDecoder)
{
  m_inputStage->sendHardwareChangeAsMidi(pDecoder);
}

void InputEventStageTester::sendCCOut(int hwID, float value, int msbCC, int lsbCC)
{
  m_inputStage->sendCCOut(hwID, value, msbCC, lsbCC);
}

void InputEventStageTester::doSendCCOut(uint16_t value, int msbCC, int lsbCC)
{
  m_inputStage->doSendCCOut(value, msbCC, lsbCC);
}

VoiceGroup InputEventStageTester::calculateSplitPartForEvent(DSPInterface::InputEvent inputEvent, int keyNumber)
{
  return m_inputStage->calculateSplitPartForEvent(inputEvent, keyNumber);
}

DSPInterface::InputEvent InputEventStageTester::getInterfaceFromDecoder(MidiReceiveChannel channel)
{
  return m_inputStage->getInterfaceFromParsedChannel(channel);
}
