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
  // note: unnecessary code duplication (better use C15::Parameters::Hardware_Sources in <parameter_declarations.h>
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

  void freezePresetMessages();
  void thawPresetMessages(bool send);

  void freezeParameterMessages();
  void thawParameterMessages(bool send);

  Parameter* findPhysicalControlParameterFromAudioEngineHWSourceID(int index);

  static nltools::msg::LayerPresetMessage createLayerEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SplitPresetMessage createSplitEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SinglePresetMessage createSingleEditBufferMessage(const EditBuffer& eb);

 private:
  void onMidiBankSelectionChanged(const Uuid& newMidiBankUuid);
  void setLastKnownMIDIProgramChangeNumber(int pc);
  void sendSelectedMidiPresetAsProgramChange();
  void onPresetManagerLoaded();

  uint8_t m_lastMIDIKnownProgramNumber = std::numeric_limits<uint8_t>::max();
  uint m_suppressParamChanges = 0;
  uint m_suppressPresetChanges = 0;

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
