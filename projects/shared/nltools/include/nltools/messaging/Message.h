#pragma once

#include "Messaging.h"
#include <cstring>
#include <nltools/Testing.h>
#include <nltools/Types.h>
#include <array>

namespace nltools
{
  namespace msg
  {
    namespace Midi
    {
      struct SimpleMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::MidiSimpleMessage;
        }

        SimpleMessage()
        {
        }

        SimpleMessage(uint8_t status, uint8_t data)
        {
          rawBytes[0] = status;
          rawBytes[1] = data;
          numBytesUsed = 2;
        }

        SimpleMessage(uint8_t status, uint8_t data1, uint8_t data2)
        {
          rawBytes[0] = status;
          rawBytes[1] = data1;
          rawBytes[2] = data2;
          numBytesUsed = 3;
        }

        std::array<uint8_t, 3> rawBytes;
        uint8_t numBytesUsed = 0;
      };

      struct ProgramChangeMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::MidiProgramChange;
        }

        uint8_t program;
        SoundType programType = SoundType::Invalid;
      };
    }

    using tID = int;
    using tControlPosition = double;

    namespace Keyboard
    {
      struct NoteDown
      {
        constexpr static MessageType getType()
        {
          return MessageType::NoteDown;
        }

        int m_keyPos;
      };

      struct NoteUp
      {
        constexpr static MessageType getType()
        {
          return MessageType::NoteUp;
        }

        int m_keyPos;
      };

      struct NoteEventHappened
      {
        constexpr static MessageType getType()
        {
          return MessageType::NoteAction;
        }
      };
    }

    namespace USB
    {
      struct USBStatusMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::USBStatusMessage;
        }

        bool m_usbAvailable;
        bool m_updateAvailable;
      };
    }

    namespace FileSystem
    {
      struct Sync
      {
        constexpr static MessageType getType()
        {
          return MessageType::SyncFS;
        }
      };
    }

    namespace Update
    {
      struct UpdateUploadedNotification
      {
        constexpr static MessageType getType()
        {
          return MessageType::UpdateUploaded;
        }
      };
    }

    namespace WiFi
    {

      namespace Helper
      {
        template <size_t tSize> struct StringWrapper
        {
          StringWrapper()
          {
            std::memset(data, '\0', tSize + 1);
          }

          void set(const std::string& s)
          {
            assert(s.size() <= tSize);
            std::copy(s.begin(), s.end() + 1, data);
          }

          [[nodiscard]] std::string get() const
          {
            return data;
          }

          char data[tSize + 1] {};
        };
      }

      struct SetWiFiSSIDMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::WiFiSetSSID;
        }

        SetWiFiSSIDMessage()
        {
        }

        template <typename T> SetWiFiSSIDMessage(const T& ssid)
        {
          m_ssid.set(ssid);
        }

        Helper::StringWrapper<128> m_ssid;
      };

      struct SetWiFiPasswordMessage
      {
        constexpr static auto maxSize = 16;

        constexpr static MessageType getType()
        {
          return MessageType::WiFiSetPassword;
        }

        SetWiFiPasswordMessage() = default;

        template <typename T> explicit SetWiFiPasswordMessage(const T& password)
        {
          m_password.set(password);
        }

        Helper::StringWrapper<maxSize> m_password;
      };

      struct EnableWiFiMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::WiFiEnable;
        }

        EnableWiFiMessage()
            : m_state { false }
        {
        }
        explicit EnableWiFiMessage(bool state)
            : m_state { state }
        {
        }

        bool m_state {};
      };

    }

    namespace Recorder
    {
      struct State
      {
        constexpr static MessageType getType()
        {
          return MessageType::FlacRecorderStateChanged;
        }

        bool recording = false;
      };
    }

    struct PanicAudioEngine
    {
      constexpr static MessageType getType()
      {
        return MessageType::AEPanic;
      }
    };

    struct BufferUnderrunsChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::BufferUnderrunsChanged;
      }

      uint64_t numUnderruns = 0;
      int32_t framesPerPeriod = 0;
    };

    struct SetFramesPerPeriod
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetFramesPerPeriod;
      }

      int32_t framesPerPeriod = 0;
    };

    namespace Setting
    {
      struct NoteShiftMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::NoteShiftSetting;
        }

        int m_shift = 0;
      };

      struct FlacRecorderAutoStart
      {
        constexpr static MessageType getType()
        {
          return MessageType::AutoStartRecorderMessage;
        }

        bool enabled;
      };

      struct FlacRecorderStopPlayback
      {
        constexpr static MessageType getType()
        {
          return MessageType::StopRecorderPlaybackMessage;
        }
      };

      struct NotifyNoRecorderClients
      {
        constexpr static MessageType getType()
        {
          return MessageType::NotifyNoRecorderClients;
        }
      };

      struct PresetGlitchMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::PresetGlitchSetting;
        }

        bool m_enabled;
      };

      struct TuneReference
      {
        constexpr static MessageType getType()
        {
          return MessageType::TuneReference;
        }

        double m_tuneReference;
      };

      struct MidiSettingsMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::MidiSettings;
        }

        enum class RoutingIndex : std::size_t
        {
          Pedal1 = 0,
          Pedal2 = 1,
          Pedal3 = 2,
          Pedal4 = 3,
          Bender = 4,
          Aftertouch = 5,
          Ribbon1 = 6,
          Ribbon2 = 7,
          ProgramChange = 8,
          Notes = 9,
          LENGTH = 10
        };

        enum class RoutingAspect : std::size_t
        {
          SEND_PRIMARY = 0,
          RECEIVE_PRIMARY = 1,
          SEND_SPLIT = 2,
          RECEIVE_SPLIT = 3,
          LOCAL = 4,
          LENGTH = 5
        };

        typedef std::array<bool, static_cast<size_t>(RoutingAspect::LENGTH)> tEntry;
        typedef std::array<tEntry, static_cast<size_t>(RoutingIndex::LENGTH)> tRoutingMappings;

        MidiReceiveChannel receiveChannel;
        MidiReceiveChannelSplit receiveSplitChannel;

        MidiSendChannel sendChannel;
        MidiSendChannelSplit sendSplitChannel;

        bool highVeloCCEnabled = true;
        bool highResCCEnabled = true;

        PedalCC pedal1cc;
        PedalCC pedal2cc;
        PedalCC pedal3cc;
        PedalCC pedal4cc;
        RibbonCC ribbon1cc;
        RibbonCC ribbon2cc;
        AftertouchCC aftertouchcc;
        BenderCC bendercc;

        bool localEnable = true;
        tRoutingMappings routings {};
      };

      struct EnableBBBWifiFromDevSettings
      {
        constexpr static MessageType getType()
        {
          return MessageType::WifiDevBBBEnable;
        }
      };

      struct SetGlobalLocalSetting
      {
        constexpr static MessageType getType()
        {
          return MessageType::GlobalLocalSetting;
        }

        bool m_state;
      };

      struct TransitionTimeMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::TransitionTimeSetting;
        }

        float m_value;
      };

      struct EditSmoothingTimeMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::EditSmoothingTimeSetting;
        }

        float m_time;
      };
    }

    struct HWSourceChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::HWSourceParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      ReturnMode returnMode;
      bool isLocalEnabled;
    };

    struct HWSourceSendChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::HWSourceSendParameter;
      }

      tID parameterId;
      tID siblingId;
      tControlPosition controlPosition;
      ReturnMode returnMode;
      bool localEnabled;
    };

    struct HWAmountChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::HWAmountParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
    };

    struct MacroControlChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::MacroControlParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
    };

    struct UnmodulateableParameterChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::UnmodulateableParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      VoiceGroup voiceGroup;
    };

    struct ModulateableParameterChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::ModulateableParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      MacroControls sourceMacro;
      tControlPosition mcAmount;
      tControlPosition mcUpper;
      tControlPosition mcLower;
      VoiceGroup voiceGroup;
    };

    struct RotaryChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::RotaryChanged;
      }

      int8_t increment;
    };

    struct TimestampedRotaryChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::TimestampedRotaryChanged;
      }

      int8_t increment;
      uint64_t timestamp;
    };

    struct ButtonChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::ButtonChanged;
      }

      int8_t buttonId;
      bool pressed;
    };

    struct SetRibbonLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetRibbonLED;
      }

      uint8_t id;
      uint8_t brightness;
    };

    struct SetPanelLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetPanelLED;
      }

      uint8_t id;
      bool on;
    };

    struct SetOLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetOLED;
      }

      uint64_t messageId = 0;
      uint8_t pixels[256][96] {};
    };

    struct OLEDState
    {
      constexpr static MessageType getType()
      {
        return MessageType::OLEDState;
      }

      uint64_t displaysMessageId = 0;
    };

    struct SetTimestampedOledMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetOLEDTimestamped;
      }

      SetOLEDMessage m_oledMessage {};
      int64_t m_timestamp {};
    };

    struct PlaycontrollerMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::Playcontroller;
      }

      Glib::RefPtr<Glib::Bytes> message;
    };

    struct PingMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::Ping;
      }
    };

    struct HardwareSourceChangedNotification
    {
      constexpr static MessageType getType()
      {
        return MessageType::NotifyHardwareSourceChanged;
      }

      size_t hwSource = 0;  // 0...7
      double position = 0;  // -1...1
      HWChangeSource source;
    };

    struct HardwareSourcePollEnd
    {
      constexpr static MessageType getType()
      {
        return MessageType::HardwarePollEnded;
      }

      std::array<float, 8> m_data;
    };

    namespace detail
    {
      template <> inline PlaycontrollerMessage deserialize<PlaycontrollerMessage>(const SerializedMessage& s)
      {
        PlaycontrollerMessage ret;
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(s->get_data(numBytes));
        ret.message = Glib::Bytes::create(data + 2, numBytes - 2);
        return ret;
      }

      template <> inline SerializedMessage serialize<PlaycontrollerMessage>(const PlaycontrollerMessage& msg)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(msg.message->get_data(numBytes));
        auto scratch = reinterpret_cast<uint16_t*>(g_malloc(numBytes + 2));
        scratch[0] = static_cast<uint16_t>(MessageType::Playcontroller);
        std::memcpy(&scratch[1], data, numBytes);
        auto bytes = g_bytes_new_take(scratch, numBytes + 2);
        return Glib::wrap(bytes);
      }

    }

    namespace ParameterGroups
    {
      struct Parameter
      {
        uint16_t id {};
        double controlPosition = 0;
      };

      struct RibbonParameter : Parameter
      {
        RibbonTouchBehaviour ribbonTouchBehaviour {};
        RibbonReturnMode ribbonReturnMode {};
      };

      struct PedalParameter : Parameter
      {
        PedalModes pedalMode {};
        ReturnMode returnMode {};
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls mc = MacroControls::NONE;
        double modulationAmount = 0;
      };

      struct UnmodulateableParameter : Parameter
      {
      };

      struct GlobalParameter : Parameter
      {
      };

      struct HardwareSourceParameter : Parameter
      {
        ReturnMode returnMode = ReturnMode::None;
      };

      struct HardwareAmountParameter : Parameter
      {
      };

      struct SplitPoint : ModulateableParameter
      {
      };

      struct UnisonGroup
      {
        ParameterGroups::UnmodulateableParameter unisonVoices;
        ParameterGroups::ModulateableParameter detune;
        ParameterGroups::UnmodulateableParameter phase;
        ParameterGroups::UnmodulateableParameter pan;
      };

      struct MonoGroup
      {
        ParameterGroups::UnmodulateableParameter monoEnable;
        ParameterGroups::UnmodulateableParameter legato;
        ParameterGroups::UnmodulateableParameter priority;
        ParameterGroups::ModulateableParameter glide;
      };

      struct MasterGroup
      {
        ParameterGroups::ModulateableParameter volume;
        ParameterGroups::ModulateableParameter tune;
        ParameterGroups::ModulateableParameter pan;
        ParameterGroups::ModulateableParameter serialFX;
      };

      inline bool operator==(const Parameter& lhs, const Parameter& rhs)
      {
        auto ret = lhs.id == rhs.id;
        ret &= lhs.controlPosition == rhs.controlPosition;
        return ret;
      }

      inline bool operator==(const ModulateableParameter& lhs, const ModulateableParameter& rhs)
      {
        auto ret = lhs.id == rhs.id;
        ret &= lhs.controlPosition == rhs.controlPosition;
        ret &= lhs.modulationAmount == rhs.modulationAmount;
        ret &= lhs.mc == rhs.mc;
        return ret;
      }

      inline bool operator==(const MonoGroup& lhs, const MonoGroup& rhs)
      {
        auto ret = lhs.glide == rhs.glide;
        ret &= lhs.monoEnable == rhs.monoEnable;
        ret &= lhs.priority == rhs.priority;
        ret &= lhs.legato == rhs.legato;
        return ret;
      }

      inline bool operator==(const UnisonGroup& lhs, const UnisonGroup& rhs)
      {
        auto ret = lhs.unisonVoices == rhs.unisonVoices;
        ret &= lhs.detune == rhs.detune;
        ret &= lhs.pan == rhs.pan;
        ret &= lhs.phase == rhs.phase;
        return ret;
      }

      inline bool operator==(const MasterGroup& lhs, const MasterGroup& rhs)
      {
        auto ret = lhs.volume == rhs.volume;
        ret &= lhs.tune == rhs.tune;
        ret &= lhs.pan == rhs.pan;
        ret &= lhs.serialFX == rhs.serialFX;
        return ret;
      }
    }

    struct SinglePresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SinglePreset;
      }

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      std::array<ParameterGroups::ModulateableParameter, 169> modulateables;
      std::array<ParameterGroups::UnmodulateableParameter, 29> unmodulateables;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;

      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
    };

    inline bool operator==(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      return ret;
    }

    inline bool operator!=(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct SplitPresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SplitPreset;
      }

      std::array<std::array<ParameterGroups::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 29>, 2> unmodulateables;

      std::array<ParameterGroups::UnisonGroup, 2> unison;
      std::array<ParameterGroups::MonoGroup, 2> mono;

      ParameterGroups::MasterGroup master;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
      std::array<ParameterGroups::SplitPoint, 2> splitpoint;
    };

    inline bool operator==(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      ret &= lhs.splitpoint == rhs.splitpoint;
      return ret;
    }

    inline bool operator!=(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct LayerPresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::LayerPreset;
      }

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      std::array<std::array<ParameterGroups::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 29>, 2> unmodulateables;

      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
    };

    inline bool operator==(const LayerPresetMessage& lhs, const LayerPresetMessage& rhs)
    {
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      return ret;
    }

    inline bool operator!=(const LayerPresetMessage& lhs, const LayerPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }
  }
}