#pragma once

#include "Messaging.h"
#include <nltools/Types.h>
#include <nltools/Testing.h>
#include <cstring>

namespace nltools
{
  namespace msg
  {

    template <MessageType msgType> struct Message
    {
      static const MessageType theType = msgType;
      MessageType type = msgType;
    };

    using tID = int;
    using tControlPosition = double;

    struct HWSourceChangedMessage : Message<MessageType::HWSourceParameter>
    {
      HWSourceChangedMessage(tID id = 0, tControlPosition controlPosition = 0, ReturnMode mode = ReturnMode::None,
                             bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : parameterId{ id }
          , controlPosition{ controlPosition }
          , returnMode{ mode }
          , lock(locked)
          , voiceGroup{ vg }
      {
      }

      tID parameterId;
      tControlPosition controlPosition;
      ReturnMode returnMode;
      bool lock;

      VoiceGroup voiceGroup;
    };

    struct HWAmountChangedMessage : Message<MessageType::HWAmountParameter>
    {
      HWAmountChangedMessage(tID id = 0, tControlPosition pos = 0.0, bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : parameterId{ id }
          , controlPosition{ pos }
          , lock{ locked }
          , voiceGroup{ vg }
      {
      }

      tID parameterId;
      tControlPosition controlPosition;
      bool lock;

      VoiceGroup voiceGroup;
    };

    struct MacroControlChangedMessage : Message<MessageType::MacroControlParameter>
    {
      MacroControlChangedMessage(tID id = 0, tControlPosition pos = 0.0, tControlPosition time = 0.0,
                                 bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId{ id }
          , controlPosition{ pos }
          , smoothingTime{ time }
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;
      tControlPosition smoothingTime;

      VoiceGroup voiceGroup;
    };

    struct UnmodulateableParameterChangedMessage : Message<MessageType::UnmodulateableParameter>
    {
      UnmodulateableParameterChangedMessage(tID id = 0, tControlPosition controlPosition = 0, bool locked = false,
                                            VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId(id)
          , controlPosition(controlPosition)
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;

      VoiceGroup voiceGroup;
    };

    struct ModulateableParameterChangedMessage : Message<MessageType::ModulateableParameter>
    {
      ModulateableParameterChangedMessage(tID id = 0, tControlPosition pos = 0.0,
                                          MacroControls source = MacroControls::NONE, tControlPosition amount = 0.0,
                                          tControlPosition upper = 0.0, tControlPosition lower = 0.0,
                                          bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId{ id }
          , controlPosition{ pos }
          , sourceMacro{ source }
          , mcAmount{ amount }
          , mcUpper{ upper }
          , mcLower{ lower }
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;
      MacroControls sourceMacro;
      tControlPosition mcAmount;
      tControlPosition mcUpper;
      tControlPosition mcLower;

      VoiceGroup voiceGroup;
    };

    struct RotaryChangedMessage : Message<MessageType::RotaryChanged>
    {
      int8_t increment;
    };

    struct ButtonChangedMessage : Message<MessageType::ButtonChanged>
    {
      int8_t buttonId;
      bool pressed;
    };

    struct SetRibbonLEDMessage : Message<MessageType::SetRibbonLED>
    {
      uint8_t id;
      uint8_t brightness;
    };

    struct SetPanelLEDMessage : Message<MessageType::SetPanelLED>
    {
      uint8_t id;
      bool on;
    };

    struct SetOLEDMessage : Message<MessageType::SetOLED>
    {
      uint8_t pixels[256][96];
    };

    struct LPCMessage : Message<MessageType::LPC>
    {
      Glib::RefPtr<Glib::Bytes> message;
    };

    struct PingMessage : Message<MessageType::Ping>
    {
    };

    namespace detail
    {
      // default (de)serialization for messages, may be specialized for more compilcated types:
      template <> inline LPCMessage deserialize<LPCMessage>(const SerializedMessage& s)
      {
        LPCMessage ret;
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(s->get_data(numBytes));
        ret.message = Glib::Bytes::create(data + 2, numBytes - 2);
        return ret;
      }

      template <> inline SerializedMessage serialize<LPCMessage>(const LPCMessage& msg)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(msg.message->get_data(numBytes));
        auto scratch = reinterpret_cast<uint16_t*>(g_malloc(numBytes + 2));
        scratch[0] = static_cast<uint16_t>(MessageType::LPC);
        std::memcpy(&scratch[1], data, numBytes);
        auto bytes = g_bytes_new_take(scratch, numBytes + 2);
        return Glib::wrap(bytes);
      }
    }

    namespace ParameterGroups
    {
      struct Parameter
      {
        uint16_t id;
        double controlPosition = 0;
      };

      struct RibbonParameter : Parameter
      {
        RibbonTouchBehaviour ribbonTouchBehaviour;
        RibbonReturnMode ribbonReturnMode;
      };

      struct PedalParameter : Parameter
      {
        PedalModes pedalMode;
        ReturnMode returnMode;
      };

      struct AftertouchParameter : Parameter
      {
        ReturnMode returnMode;
      };

      struct BenderParameter : Parameter
      {
        ReturnMode returnMode;
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls mc = MacroControls::NONE;
        double modulationAmount = 0;
      };

      struct UnmodulatebaleParameter : Parameter
      {
      };

      struct MonoParameter : Parameter
      {
      };

      struct SplitPoint : Parameter
      {
      };

#warning "Auge Auge Auge"
      //autsch.. TODO cleanup

      inline std::ostream& operator<<(std::ostream& o, const Parameter& p)
      {
        return o << "ID: " << p.id << " value: " << p.controlPosition;
      }

      inline bool operator==(const Parameter& lhs, const Parameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition;
      }

      inline bool operator==(const ModulateableParameter& lhs, const ModulateableParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.mc == rhs.mc
            && lhs.modulationAmount == rhs.modulationAmount;
      }

      inline bool operator==(const AftertouchParameter& lhs, const AftertouchParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const BenderParameter& lhs, const BenderParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const PedalParameter& lhs, const PedalParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.pedalMode == rhs.pedalMode
            && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const RibbonParameter& lhs, const RibbonParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition
            && lhs.ribbonTouchBehaviour == rhs.ribbonTouchBehaviour && lhs.ribbonReturnMode == rhs.ribbonReturnMode;
      }
    }

    struct SinglePresetMessage : Message<MessageType::SinglePreset>
    {
      std::array<ParameterGroups::PedalParameter, 4> pedals;
      std::array<ParameterGroups::AftertouchParameter, 1> aftertouch;
      std::array<ParameterGroups::RibbonParameter, 2> ribbons;
      std::array<ParameterGroups::BenderParameter, 1> bender;
      std::array<ParameterGroups::MacroParameter, 4> macros;
      std::array<ParameterGroups::ModulateableParameter, 89> modulateables;
      std::array<ParameterGroups::UnmodulatebaleParameter, 124> unmodulateables;
      std::array<ParameterGroups::MonoParameter, 4> monos;

      std::array<ParameterGroups::Parameter, 2> master;
      std::array<ParameterGroups::Parameter, 12> scale;
    };

    struct SplitPresetMessage : Message<MessageType::SplitPreset>
    {
      std::array<std::array<ParameterGroups::PedalParameter, 4>, 2> pedals;
      std::array<std::array<ParameterGroups::AftertouchParameter, 1>, 2> aftertouch;
      std::array<std::array<ParameterGroups::RibbonParameter, 2>, 2> ribbons;
      std::array<std::array<ParameterGroups::BenderParameter, 1>, 2> bender;
      std::array<std::array<ParameterGroups::MacroParameter, 4>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 89>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 124>, 2> unmodulateables;
      std::array<std::array<ParameterGroups::MonoParameter, 4>, 2> monos;
      std::array<std::array<ParameterGroups::Parameter, 2>, 2> vgMaster;

      std::array<ParameterGroups::Parameter, 2> master;
      std::array<ParameterGroups::Parameter, 12> scale;
      ParameterGroups::SplitPoint splitpoint;
    };

    struct LayerPresetMessage : Message<MessageType::LayerPreset>
    {
      std::array<std::array<ParameterGroups::PedalParameter, 4>, 2> pedals;
      std::array<std::array<ParameterGroups::AftertouchParameter, 1>, 2> aftertouch;
      std::array<std::array<ParameterGroups::RibbonParameter, 2>, 2> ribbons;
      std::array<std::array<ParameterGroups::BenderParameter, 1>, 2> bender;
      std::array<std::array<ParameterGroups::MacroParameter, 4>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 89>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 124>, 2> unmodulateables;
      std::array<ParameterGroups::MonoParameter, 4> monos;
      std::array<std::array<ParameterGroups::Parameter, 2>, 2> vgMaster;

      std::array<ParameterGroups::Parameter, 2> master;
      std::array<ParameterGroups::Parameter, 12> scale;
    };

    //Helpers
    template <typename tArray> bool compareArray(const tArray& a1, const tArray& a2)
    {
      auto ret = a1.size() == a2.size();

      if(ret)
      {
        for(unsigned long i = 0; i < a1.size(); i++)
        {
          ret = a1[i] == a2[i];
          if(!ret)
            return false;
        }
      }
      return ret;
    }

    inline bool operator==(const SinglePresetMessage& p1, const SinglePresetMessage& p2)
    {
      auto mod = compareArray(p1.modulateables, p2.modulateables);
      auto unmod = compareArray(p1.unmodulateables, p2.unmodulateables);
      auto after = compareArray(p1.aftertouch, p2.aftertouch);
      auto scale = compareArray(p1.scale, p2.scale);
      auto mono = compareArray(p1.monos, p2.monos);
      auto pedal = compareArray(p1.pedals, p2.pedals);
      auto bender = compareArray(p1.bender, p2.bender);
      auto ribbon = compareArray(p1.ribbons, p2.ribbons);
      auto master = compareArray(p1.master, p2.master);
      auto mc = compareArray(p1.macros, p2.macros);
      return mod && unmod && after && scale && mono && bender && pedal && ribbon && master && mc;
    }

    inline bool operator==(const LayerPresetMessage& p1, const LayerPresetMessage& p2)
    {
      auto same = true;
      for(auto i = 0; i < 2; i++)
      {
        auto mod = compareArray(p1.modulateables[i], p2.modulateables[i]);
        auto unmod = compareArray(p1.unmodulateables[i], p2.unmodulateables[i]);
        auto after = compareArray(p1.aftertouch[i], p2.aftertouch[i]);
        auto pedal = compareArray(p1.pedals[i], p2.pedals[i]);
        auto bender = compareArray(p1.bender[i], p2.bender[i]);
        auto ribbon = compareArray(p1.ribbons[i], p2.ribbons[i]);
        auto mc = compareArray(p1.macros[i], p2.macros[i]);
        auto vgMaster = compareArray(p1.vgMaster[i], p2.vgMaster[i]);
        same = same && mod && unmod && after && bender && pedal && ribbon && mc && vgMaster;
      }
      auto master = compareArray(p1.master, p2.master);
      auto mono = compareArray(p1.monos, p2.monos);
      auto scale = compareArray(p1.scale, p2.scale);
      return same && mono && scale && master;
    }

    inline bool operator==(const SplitPresetMessage& p1, const SplitPresetMessage& p2)
    {
      auto same = true;
      for(auto i = 0; i < 2; i++)
      {
        auto mod = compareArray(p1.modulateables[i], p2.modulateables[i]);
        auto unmod = compareArray(p1.unmodulateables[i], p2.unmodulateables[i]);
        auto after = compareArray(p1.aftertouch[i], p2.aftertouch[i]);
        auto pedal = compareArray(p1.pedals[i], p2.pedals[i]);
        auto bender = compareArray(p1.bender[i], p2.bender[i]);
        auto ribbon = compareArray(p1.ribbons[i], p2.ribbons[i]);
        auto mc = compareArray(p1.macros[i], p2.macros[i]);
        auto vgMaster = compareArray(p1.vgMaster[i], p2.vgMaster[i]);
        same = same && mod && unmod && after && bender && pedal && ribbon && mc && vgMaster;
      }
      auto mono = compareArray(p1.monos, p2.monos);
      auto scale = compareArray(p1.scale, p2.scale);
      auto split = p1.splitpoint == p2.splitpoint;
      auto master = compareArray(p1.master, p2.master);

      return same && mono && scale && split && master;
    }
  }
}