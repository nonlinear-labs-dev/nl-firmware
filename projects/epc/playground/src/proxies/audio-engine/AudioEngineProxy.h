#pragma once

#include <nltools/messaging/Message.h>
#include <parameters/messaging/ParameterMessageFactory.h>
#include <groups/MonoGroup.h>
#include <tools/RecursionGuard.h>
#include <nltools-2/include/PresetMessages.h>

namespace UNDO
{
  class Transaction;
}

class EditBuffer;
class Uuid;
class Settings;
class PresetManager;
class PlaycontrollerProxy;

class AudioEngineProxy : public sigc::trackable
{
 public:
  enum class HardwareSource_AE : int8_t
  {
    PEDAL1 = 0,
    PEDAL2 = 1,
    PEDAL3 = 2,
    PEDAL4 = 3,
    BENDER = 4,
    AFTERTOUCH = 5,
    RIBBON1 = 6,
    RIBBON2 = 7,
    RIBBON3 = 8,
    RIBBON4 = 9,
    NONE = -1
  };

  AudioEngineProxy(PresetManager& pm, Settings& settings, PlaycontrollerProxy& playProxy);

  template <typename tParameter> auto createAndSendParameterMessage(const tParameter* parameter)
  {
    if(!m_suppressParamChanges)
      sendParameterMessage(createMessage<tParameter>(parameter));
  }

  template <typename tParameter> auto createMessage(const tParameter* parameter)
  {
    return ParameterMessageFactory::createMessage<tParameter>(parameter);
  }

  template <class tMessage> void sendParameterMessage(const tMessage& msg)
  {
    if(!m_suppressParamChanges)
      send(nltools::msg::EndPoint::AudioEngine, msg);
  }

  template <typename tSettingMessage> void sendSettingMessage(const tSettingMessage& msg)
  {
    send(nltools::msg::EndPoint::AudioEngine, msg);
  }

  void sendEditBuffer();

  void freezeParameterMessages();
  void thawParameterMessages(bool send);

  Parameter* findPhysicalControlParameterFromAudioEngineHWSourceID(int index);

  static nltools2::msg::LayerPresetMessage createLayerEditBufferMessage(const EditBuffer& eb);
  static nltools2::msg::SplitPresetMessage createSplitEditBufferMessage(const EditBuffer& eb);
  static nltools2::msg::SinglePresetMessage createSingleEditBufferMessage(const EditBuffer& eb);

 private:
  static void fillMonoPart(nltools2::msg::ParameterGroups::MonoGroup& monoGroup, ParameterGroup* const& g);
  static void fillUnisonPart(nltools2::msg::ParameterGroups::UnisonGroup& unisonGroup, ParameterGroup* const& g);

  void onMidiBankSelectionChanged(const Uuid& newMidiBankUuid);
  void setLastKnownMIDIProgramChangeNumber(int pc);
  void sendSelectedMidiPresetAsProgramChange();
  void onPresetManagerLoaded();

  uint8_t m_lastMIDIKnownProgramNumber = std::numeric_limits<uint8_t>::max();
  uint m_suppressParamChanges = 0;

  sigc::connection m_midiBankChangedConnection;
  sigc::connection m_midiBankConnection;
  RecursionGuard m_programChangeRecursion;
  template <typename T> void subscribeToMidiSetting(Settings* s);
  template <typename... TT> void subscribeToMidiSettings(Settings* s);
  void connectSettingsToAudioEngineMessage();
  void scheduleMidiSettingsMessage();

  Throttler m_sendMidiSettingThrottler;
  std::vector<sigc::connection> m_settingConnections;
  PresetManager& m_presetManager;
  Settings& m_settings;
  PlaycontrollerProxy& m_playcontrollerProxy;
};
