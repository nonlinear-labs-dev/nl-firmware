#pragma once

#include "nltools/Assert.h"
#include <memory>
#include <chrono>
#include <sigc++/connection.h>
#include <nltools/logging/Log.h>
#include <nltools/enums/EnumTools.h>
#include <nltools/threading/Threading.h>
#include <glibmm/bytes.h>
#include <glibmm/main.h>
#include <string.h>
#include <set>

namespace nltools
{
  namespace msg
  {
    // Types:
    using SerializedMessage = Glib::RefPtr<Glib::Bytes>;

    ENUM(EndPoint, uint16_t, None, Playcontroller, Oled, PanelLed, RibbonLed, AudioEngine, Playground, BeagleBone,
         ExternalMidiOverIPBridge, ExternalMidiOverIPClient, TestEndPoint);

    uint getPortFor(EndPoint p);

    ENUM(MessageType, uint16_t, Preset, Morph_A, Morph_B, Parameter, EditControl, MorphPosition, Setting, Notification,
         Assertion, Request,

         SetRibbonLED, SetPanelLED, SetOLED, OLEDState, SetOLEDTimestamped, RotaryChanged, TimestampedRotaryChanged,
         ButtonChanged, Playcontroller, Ping,

         SinglePreset, LayerPreset, SplitPreset,

         UpdateLocalDisabledRibbon,

         NoteShiftSetting, PresetGlitchSetting, TransitionTimeSetting, EditSmoothingTimeSetting, TuneReference,

         NoteDown, NoteUp, NoteAction,

         USBStatusMessage,

         WiFiPasswordChanged, WiFiSSIDChanged, WiFiSetSSID, WiFiSetPassword, WiFiEnable,

         NotifyHardwareSourceChanged,

         MidiSimpleMessage, MidiAck, MidiProgramChange, MidiBridgeSettings, MidiSettings, MidiHardwareChange,

         SyncFS, UpdateUploaded, AutoStartRecorderMessage, AEPanic, GlobalLocalSetting, WifiDevBBBEnable,
         BufferUnderrunsChanged, SetFramesPerPeriod, FlacRecorderStateChanged, HardwarePollEnded,
         StopRecorderPlaybackMessage, NotifyNoRecorderClients,

         // new ParameterChanged protocol
         HardwareSourceParameterChanged, HardwareSourceSendParameterChanged, HardwareAmountParameterChanged,
         MacroControlParameterChanged, MacroTimeParameterChanged, GlobalModulateableParameterChanged,
         GlobalUnmodulateableParameterChanged, PolyphonicModulateableParameterChanged,
         PolyphonicUnmodulateableParameterChanged, MonophonicModulateableParameterChanged,
         MonophonicUnmodulateableParameterChanged,

         //Test Tone Protocol
         TestToneTypeMessage, TestTonePanMessage);

    namespace detail
    {
      // default (de)serialization for messages, may be specialized for more compilcated types:
      template <typename Msg> Msg deserialize(const SerializedMessage &s)
      {
        Msg ret;
        nltools_assertAlways(s->get_size() == sizeof(Msg) + 2);
        gsize numBytes = 0;
        auto ptr = reinterpret_cast<const uint8_t *>(s->get_data(numBytes));
        memcpy(&ret, ptr + 2, sizeof(Msg));
        return ret;
      }

      template <typename Msg> SerializedMessage serialize(const Msg &msg)
      {
        uint8_t scratch[sizeof(Msg) + 2];
        auto type = Msg::getType();
        memcpy(scratch, &type, 2);
        memcpy(scratch + 2, &msg, sizeof(Msg));
        return Glib::Bytes::create(scratch, sizeof(scratch));
      }

      // send raw bytes to receiver
      bool send(EndPoint receiver, const SerializedMessage &msg);

      template <typename Msg>
      sigc::connection receive(MessageType type, EndPoint receivingEndPoint, std::function<void(const Msg &)> cb)
      {
        return receiveSerialized(type, receivingEndPoint, [=](const SerializedMessage &s) {
          auto msg = detail::deserialize<Msg>(s);
          cb(msg);
        });
      }

      sigc::connection receiveSerialized(MessageType type, EndPoint receivingEndPoint,
                                         std::function<void(const SerializedMessage &)> cb);
    }

    // Client has to call this on startup

    struct ChannelConfiguration
    {
      ChannelConfiguration(EndPoint p = EndPoint::None, const std::string &hostName = "localhost",
                           threading::Priority prio = threading::Priority::Normal);

      ChannelConfiguration(EndPoint p, threading::Priority prio);

      bool operator<(const ChannelConfiguration &other) const
      {
        return peer < other.peer;
      }

      EndPoint peer;
      std::string uri;
      threading::Priority prio = threading::Priority::Normal;
    };

    struct Configuration
    {
      std::set<ChannelConfiguration> offerEndpoints;
      std::set<ChannelConfiguration> useEndpoints;
      Glib::RefPtr<Glib::MainContext> mainContext;
    };

    void init(const Configuration &conf);
    void deInit();
    void addTestEndpoint(EndPoint e);
    const Configuration &getConfig();

    // wait at most timeOut for the sigc::connection to be established
    // return true if there is a sigc::connection to receiver
    bool waitForConnection(EndPoint receiver, std::chrono::milliseconds timeOut = std::chrono::seconds(10));

    // Send msg to receiver. If there is no receiver, does nothing.
    template <typename Msg> bool send(EndPoint receiver, const Msg &msg)
    {
      return detail::send(receiver, detail::serialize<Msg>(msg));
    }

    void flush(EndPoint receiver, const std::chrono::milliseconds timeout);

    template <typename Msg> sigc::connection receive(EndPoint receivingEndPoint, std::function<void(const Msg &)> cb)
    {
      return detail::receive<Msg>(Msg::getType(), receivingEndPoint, [=](const auto &s) { cb(s); });
    }

    sigc::connection onConnectionEstablished(EndPoint endPoint, std::function<void()> cb);
  }
}
