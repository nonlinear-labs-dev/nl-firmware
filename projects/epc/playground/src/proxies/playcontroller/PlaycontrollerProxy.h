#pragma once

#include "playground.h"
#include "MessageParser.h"
#include "MessageComposer.h"
#include "parameters/ValueRange.h"
#include "parameter_declarations.h"
#include <parameters/value/QuantizedValue.h>
#include <memory>
#include <nltools/threading/Throttler.h>
#include <tools/Signal.h>
#include <device-settings/PedalType.h>

class Application;
class Parameter;
class ModulateableParameter;
class PhysicalControlParameter;
class Preset;
class MessageParser;

namespace nltools
{
  namespace msg
  {
    struct PlaycontrollerMessage;
  }
}

class PlaycontrollerProxy
{
 public:
  PlaycontrollerProxy();
  virtual ~PlaycontrollerProxy();

  void sendSetting(uint16_t key, gint16 value);
  void sendSetting(uint16_t key, uint16_t value);

  void sendPedalSetting(uint16_t pedal, PedalTypes pedalType, bool reset);

  sigc::connection onPlaycontrollerSoftwareVersionChanged(const sigc::slot<void, int> &s);
  sigc::connection onLastKeyChanged(sigc::slot<void> s);
  sigc::connection onUHIDChanged(const sigc::slot<void, uint64_t> &s);
  sigc::connection onRibbonTouched(const sigc::slot<void, int>& s);

  int getLastTouchedRibbonParameterID() const;
  std::string getPlaycontrollerSoftwareVersion() const;
  uint64_t getUHID() const;
  void notifyRibbonTouch(int ribbonsParameterID);
  void setUHID(uint64_t uhid);
  int16_t ribbonRelativeFactorToTCDValue(tControlPositionValue d);
  int16_t ribbonCPValueToTCDValue(tControlPositionValue d, bool bipolar);

  sigc::connection onCalibrationStatusChanged(const sigc::slot<void, bool> &slot);

 private:
  void onPlaycontrollerMessage(const nltools::msg::PlaycontrollerMessage &msg);
  void onMessageReceived(const MessageParser::NLMessage &msg);

  typedef std::shared_ptr<MessageComposer> tMessageComposerPtr;
  void queueToPlaycontroller(const tMessageComposerPtr &cmp);

  static gint16 separateSignedBitToComplementary(uint16_t v);
  void traceBytes(const Glib::RefPtr<Glib::Bytes> &bytes) const;

  void onEditControlMessageReceived(const MessageParser::NLMessage &msg);
  void onRelativeEditControlMessageReceived(Parameter *p, gint16 value);
  void onAbsoluteEditControlMessageReceived(Parameter *p, gint16 value);
  void onAssertionMessageReceived(const MessageParser::NLMessage &msg);
  void onNotificationMessageReceived(const MessageParser::NLMessage &msg);
  void onPlaycontrollerConnected();
  void onHeartbeatReceived(const MessageParser::NLMessage &msg);
  void sendCalibrationData();

  std::shared_ptr<MessageParser> m_msgParser;

  int m_lastTouchedRibbon = C15::PID::Ribbon_2;
  Signal<void, int> m_signalRibbonTouched;
  Signal<void, int> m_signalPlaycontrollerSoftwareVersionChanged;
  Signal<void, uint64_t> m_signalUHIDChanged;
  Signal<void, bool> m_signalCalibrationStatus;
  Signal<void> m_lastKeyChanged;

  std::unique_ptr<QuantizedValue::IncrementalChanger> m_relativeEditControlMessageChanger;

  int m_playcontrollerSoftwareVersion = -1;
  uint64_t m_uhid = 0;

  bool m_hasAftertouchCalibrationData = false;

  Throttler m_throttledRelativeParameterChange;
  gint32 m_throttledRelativeParameterAccumulator = 0;

  Throttler m_throttledAbsoluteParameterChange;
  gint32 m_throttledAbsoluteParameterValue = 0;

  uint64_t m_lastReceivedHeartbeat = -1;

  void onHeartbeatStumbled();
  void requestPlaycontrollerSoftwareVersion();
  void requestPlaycontrollerUHID();
  void sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes type);
  void notifyKeyBedActionHappened();
  void onUHIDReceived(const MessageParser::NLMessage &message);
  void requestHWPositions();
  void requestCalibrationStatus();
  void onSelectedRibbonsChanged(const Setting *s);
};
