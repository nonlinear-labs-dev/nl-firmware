#include <MidiRuntimeOptions.h>

#include <utility>
#include "synth/c15-audio-engine/dsp_host_dual.h"
#include "InputEventStage.h"

InputEventStage::InputEventStage(DSPInterface *dspHost, MidiRuntimeOptions *options, InputEventStage::MIDIOut outCB)
    : m_dspHost { dspHost }
    , m_options { options }
    , m_midiOut { std::move(outCB) }
{
}

void InputEventStage::onTCDMessage(const MidiEvent &tcdEvent)
{
  if(!m_tcdDecoder)
    m_tcdDecoder = std::make_unique<TCDDecoder>();

  if(m_tcdDecoder->decode(tcdEvent))
  {
    onTCDEvent(m_tcdDecoder.get());
    m_tcdDecoder.reset(nullptr);
  }
}

void InputEventStage::onMIDIMessage(const MidiEvent &midiEvent)
{
  if(!m_midiDecoder)
    m_midiDecoder = std::make_unique<MIDIDecoder>(m_dspHost, m_options);

  if(m_midiDecoder->decode(midiEvent))
  {
    onMIDIEvent(m_midiDecoder.get());
    m_midiDecoder.reset(nullptr);
  }
}

void InputEventStage::onTCDEvent(TCDDecoder *decoder)
{
  if(decoder)
  {
    switch(decoder->getEventType())
    {
      case DecoderEventType::KeyDown:
        if(m_options->shouldReceiveLocalNotes())
          m_dspHost->onKeyDown(decoder->getKeyOrController(), decoder->getValue(), VoiceGroup::I);
        if(m_options->shouldSendNotes())
          convertToAndSendMIDI(decoder);

        break;
      case DecoderEventType::KeyUp:
        if(m_options->shouldReceiveLocalNotes())
          m_dspHost->onKeyUp(decoder->getKeyOrController(), decoder->getValue(), VoiceGroup::I);
        if(m_options->shouldSendNotes())
          convertToAndSendMIDI(decoder);

        break;
      case DecoderEventType::HardwareChange:
        if(m_options->shouldReceiveLocalControllers())
          m_dspHost->onHWChanged(decoder->getKeyOrController(), decoder->getValue());
        if(m_options->shouldSendControllers())
          convertToAndSendMIDI(decoder);

        break;
      case DecoderEventType::UNKNOWN:
        nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
    }
  }
}

void InputEventStage::onMIDIEvent(MIDIDecoder *decoder)
{
  if(decoder)
  {
    switch(decoder->getEventType())
    {
      case DecoderEventType::KeyDown:
        if(checkKeyDownEnabled(decoder))
          m_dspHost->onKeyDown(decoder->getKeyOrControl(), decoder->getValue(), VoiceGroup::I);
        break;

      case DecoderEventType::KeyUp:
        if(checkKeyUpEnabled(decoder))
          m_dspHost->onKeyUp(decoder->getKeyOrControl(), decoder->getValue(), VoiceGroup::I);
        break;

      case DecoderEventType::HardwareChange:
        if(checkHardwareChangeEnabled(decoder))
          m_dspHost->onHWChanged(decoder->getKeyOrControl(), decoder->getValue());
        break;

      case DecoderEventType::UNKNOWN:
        nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
    }
  }
}

void InputEventStage::convertToAndSendMIDI(TCDDecoder *pDecoder)
{
  MIDIOutType msg {};
  //TODO put stuff from pDecoder into msg
  m_midiOut(msg);
}

bool InputEventStage::checkKeyDownEnabled(MIDIDecoder *pDecoder)
{
  const auto recNotes = m_options->shouldReceiveNotes();
  const auto channelMatches = pDecoder->getChannel() == m_options->getReceiveChannel()
      || m_options->getReceiveChannel() == MidiReceiveChannel::Omni;
  return recNotes && channelMatches;
}

bool InputEventStage::checkKeyUpEnabled(MIDIDecoder *pDecoder)
{
  return checkKeyDownEnabled(pDecoder);
}

bool InputEventStage::checkHardwareChangeEnabled(MIDIDecoder *pDecoder)
{
  const auto recControls = m_options->shouldReceiveControllers();
  const auto channelMatches = pDecoder->getChannel() == m_options->getReceiveChannel()
      || m_options->getReceiveChannel() == MidiReceiveChannel::Omni;
  return recControls && channelMatches;
}
