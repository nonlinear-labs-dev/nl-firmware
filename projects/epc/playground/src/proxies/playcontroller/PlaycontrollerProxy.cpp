#include "PlaycontrollerProxy.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameters/PhysicalControlParameter.h"
#include <parameters/RibbonParameter.h>
#include "MessageParser.h"
#include "EditBufferMessageComposer.h"
#include "libundo/undo/Scope.h"
#include "http/UndoScope.h"
#include "proxies/hwui/Layout.h"
#include "device-settings/SendPresetAsPlaycontrollerWriteFallback.h"
#include "device-settings/Settings.h"
#include "proxies/hwui/HWUI.h"
#include "groups/HardwareSourcesGroup.h"
#include "device-settings/DebugLevel.h"
#include "device-settings/VelocityCurve.h"
#include "use-cases/ParameterUseCases.h"
#include "parameters/ValueRange.h"
#include "device-settings/SelectedRibbonsSetting.h"
#include <device-settings/ParameterEditModeRibbonBehaviour.h>
#include <memory.h>
#include <nltools/messaging/Message.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <device-settings/Settings.h>
#include <filesystem>
#include <use-cases/IncrementalChangerUseCases.h>

PlaycontrollerProxy::PlaycontrollerProxy()
    : m_lastTouchedRibbon(HardwareSourcesGroup::getLowerRibbon2ParameterID().getNumber())
    , m_throttledRelativeParameterChange(Application::get().getMainContext(), std::chrono::milliseconds(1))
    , m_throttledAbsoluteParameterChange(Application::get().getMainContext(), std::chrono::milliseconds(1))
{
  m_msgParser.reset(new MessageParser());

  if(Application::exists())
  {
    nltools::msg::onConnectionEstablished(nltools::msg::EndPoint::Playcontroller,
                                          sigc::mem_fun(this, &PlaycontrollerProxy::onPlaycontrollerConnected));

    nltools::msg::receive<nltools::msg::PlaycontrollerMessage>(
        nltools::msg::EndPoint::Playground, sigc::mem_fun(this, &PlaycontrollerProxy::onPlaycontrollerMessage));

    nltools::msg::receive<nltools::msg::Keyboard::NoteEventHappened>(
        nltools::msg::EndPoint::Playground,
        sigc::hide(sigc::mem_fun(this, &PlaycontrollerProxy::notifyKeyBedActionHappened)));

    Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(
        sigc::mem_fun(this, &PlaycontrollerProxy::onSelectedRibbonsChanged));
  }
}

PlaycontrollerProxy::~PlaycontrollerProxy() = default;

void PlaycontrollerProxy::onPlaycontrollerMessage(const nltools::msg::PlaycontrollerMessage &msg)
{
  gsize numBytes = 0;
  const auto *buffer = (const uint8_t *) (msg.message->get_data(numBytes));

  if(numBytes > 0)
  {
    if(m_msgParser->parse(buffer, numBytes) == 0)
    {
      onMessageReceived(m_msgParser->getMessage());
      m_msgParser.reset(new MessageParser());
    }
  }
}

int PlaycontrollerProxy::getLastTouchedRibbonParameterID() const
{
  return m_lastTouchedRibbon;
}

gint16 PlaycontrollerProxy::separateSignedBitToComplementary(uint16_t v)
{
  gint16 sign = (v & 0x8000) ? -1 : 1;
  auto value = static_cast<gint16>(v & 0x7FFF);
  return static_cast<gint16>(sign * value);
}

void PlaycontrollerProxy::onMessageReceived(const MessageParser::NLMessage &msg)
{
  if(msg.type == MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_EDIT_CONTROL)
  {
    onEditControlMessageReceived(msg);
  }
  else if(msg.type == MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_ASSERTION)
  {
    onAssertionMessageReceived(msg);
  }
  else if(msg.type == MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_UHID64)
  {
    onUHIDReceived(msg);
  }
  else if(msg.type == MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_NOTIFICATION)
  {
    onNotificationMessageReceived(msg);
  }
  else if(msg.type == MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_HEARTBEAT)
  {
    onHeartbeatReceived(msg);
  }
}

void PlaycontrollerProxy::onHeartbeatReceived(const MessageParser::NLMessage &msg)
{
  auto heartbeat = *(reinterpret_cast<const uint64_t *>(msg.params.data()));

  if(heartbeat < m_lastReceivedHeartbeat)
  {
    DebugLevel::warning("PlaycontrollerProxy had to re-send the edit buffer, as the heartbeat stumbled from",
                        m_lastReceivedHeartbeat, "to", heartbeat);

    onHeartbeatStumbled();
  }

  m_lastReceivedHeartbeat = heartbeat;
}

void PlaycontrollerProxy::onAssertionMessageReceived(const MessageParser::NLMessage &msg)
{
  auto numWords = msg.length;
  auto numBytes = numWords * 2;
  const char *bytes = (const char *) msg.params.data();
  char txt[numBytes + 2];
  strncpy(txt, bytes, numBytes);
  txt[numBytes] = '\0';
  nltools::Log::error("!!! Assertion from playcontroller in", (const char *) txt);
}

void PlaycontrollerProxy::onNotificationMessageReceived(const MessageParser::NLMessage &msg)
{
  uint16_t id = msg.params[0];
  uint16_t value = msg.params[1];

  if(id == MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_SW_VERSION)
  {
    if(m_playcontrollerSoftwareVersion != value)
    {
      m_playcontrollerSoftwareVersion = value;
      m_signalPlaycontrollerSoftwareVersionChanged.send(m_playcontrollerSoftwareVersion);
    }
  }

  else if(id == MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_AT_STATUS)
  {
    AT_status_T atStatus = AT_uint16ToStatus(value);

    if(m_hasAftertouchCalibrationData != (atStatus.calibrated != 0))
    {
      m_hasAftertouchCalibrationData = (atStatus.calibrated != 0);
      m_signalCalibrationStatus.send(m_hasAftertouchCalibrationData);
    }
  }
}

void PlaycontrollerProxy::onUHIDReceived(const MessageParser::NLMessage &msg)
{
  if(msg.params.size() == 4)
  {
    uint64_t uhid = 0;
    for(auto i = 0; i < 4; i++)
    {
      uint64_t val = msg.params[i];
      auto shifted = val << (i * 16);
      uhid += shifted;
    }

    setUHID(uhid);
  }
}

void PlaycontrollerProxy::onPlaycontrollerConnected()
{
  sendCalibrationData();
  requestPlaycontrollerSoftwareVersion();
  requestPlaycontrollerUHID();
  requestHWPositions();
  requestCalibrationStatus();
}

void PlaycontrollerProxy::sendCalibrationData()
{
  static const char *calibrationPath = "/persistent/calibration/calibration.bin";
  if(std::filesystem::exists(calibrationPath))
  {
    auto message = nltools::readBinaryFile(calibrationPath);
    nltools::msg::PlaycontrollerMessage msg;
    msg.message = Glib::Bytes::create(&message[0], message.size());
    nltools::msg::send(nltools::msg::EndPoint::Playcontroller, msg);
  }
  else
  {
    nltools::Log::error("Could not send calibration data as the file", calibrationPath, "is nonexistent!");
  }
}

void PlaycontrollerProxy::onEditControlMessageReceived(const MessageParser::NLMessage &msg)
{
  uint16_t id = msg.params[0];
  if(auto ribbonParam
     = Application::get().getAudioEngineProxy()->findPhysicalControlParameterFromAudioEngineHWSourceID(id))
  {
    notifyRibbonTouch(ribbonParam->getID().getNumber());
  }

  gint16 value = separateSignedBitToComplementary(msg.params[1]);

  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  if(auto p = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg))
  {
    auto ribbonModeBehaviour = Application::get().getSettings()->getSetting<ParameterEditModeRibbonBehaviour>()->get();

    if(ribbonModeBehaviour == ParameterEditModeRibbonBehaviours::PARAMETER_EDIT_MODE_RIBBON_BEHAVIOUR_RELATIVE)
    {
      onRelativeEditControlMessageReceived(p, value);
    }
    else
    {
      onAbsoluteEditControlMessageReceived(p, value);
    }
  }
}

void PlaycontrollerProxy::onRelativeEditControlMessageReceived(Parameter *p, gint16 value)
{
  m_throttledRelativeParameterAccumulator += value;

  m_throttledRelativeParameterChange.doTask(
      [this, p]()
      {
        if(!m_relativeEditControlMessageChanger || !m_relativeEditControlMessageChanger->isManaging(p->getValue()))
          m_relativeEditControlMessageChanger = p->getValue().startUserEdit(Initiator::EXPLICIT_PLAYCONTROLLER);

        auto amount = m_throttledRelativeParameterAccumulator / (p->isBiPolar() ? 8000.0 : 16000.0);
        IncrementalChangerUseCases useCase(m_relativeEditControlMessageChanger.get());
        useCase.changeBy(amount, false);
        m_throttledRelativeParameterAccumulator = 0;
      });
}

void PlaycontrollerProxy::onAbsoluteEditControlMessageReceived(Parameter *p, gint16 value)
{
  m_throttledAbsoluteParameterValue = value;

  m_throttledAbsoluteParameterChange.doTask(
      [this, p]()
      {
        ParameterUseCases useCase(p);

        if(p->isBiPolar())
        {
          useCase.setControlPosition((m_throttledAbsoluteParameterValue - 8000.0) / 8000.0);
        }
        else
        {
          useCase.setControlPosition(m_throttledAbsoluteParameterValue / 16000.0);
        }
      });
}

void PlaycontrollerProxy::notifyRibbonTouch(int ribbonsParameterID)
{
  if(ribbonsParameterID == HardwareSourcesGroup::getLowerRibbon2ParameterID().getNumber()
     || ribbonsParameterID == HardwareSourcesGroup::getUpperRibbon1ParameterID().getNumber()
     || ribbonsParameterID == HardwareSourcesGroup::getLowerRibbon4ParameterID().getNumber()
     || ribbonsParameterID == HardwareSourcesGroup::getUpperRibbon3ParameterID().getNumber())
  {
    m_lastTouchedRibbon = ribbonsParameterID;

    m_signalRibbonTouched.send(ribbonsParameterID);
  }
}

void PlaycontrollerProxy::queueToPlaycontroller(const tMessageComposerPtr &cmp)
{
  auto flushed = cmp->flush();
  traceBytes(flushed);

  nltools::msg::PlaycontrollerMessage msg;
  msg.message = flushed;

  nltools::msg::send(nltools::msg::EndPoint::Playcontroller, msg);
}

void PlaycontrollerProxy::traceBytes(const Glib::RefPtr<Glib::Bytes> &bytes) const
{
  if(Application::get().getSettings()->getSetting<DebugLevel>()->get() == DebugLevels::DEBUG_LEVEL_GASSY)
  {
    gsize numBytes = 0;
    auto data = static_cast<const uint8_t *>(bytes->get_data(numBytes));

    char txt[numBytes * 4];
    char *ptr = txt;

    for(size_t i = 0; i < numBytes; i++)
      ptr += sprintf(ptr, "%02x ", data[i]);

    *ptr = '\0';
    DebugLevel::gassy((const char *) txt);
  }
}

void PlaycontrollerProxy::sendSetting(uint16_t key, uint16_t value)
{
  tMessageComposerPtr cmp(new MessageComposer(MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_SETTING));
  *cmp << key;
  *cmp << value;
  queueToPlaycontroller(cmp);
}

void PlaycontrollerProxy::sendPedalSetting(uint16_t pedal, PedalTypes pedalType, bool reset)
{
  auto len = EHC_GetPlaycontrollerMessageLength();
  uint8_t buffer[len];

  if(auto written = EHC_ComposePlaycontrollerSetupMessageById(static_cast<EHC_PRESET_ID>(pedalType), pedal,
                                                              reset ? EHC_RESET : EHC_NORESET, buffer))

  {
    nltools::msg::PlaycontrollerMessage msg;
    msg.message = Glib::Bytes::create(buffer, written);
    nltools::msg::send(nltools::msg::EndPoint::Playcontroller, msg);
  }
  else
  {
    DebugLevel::error("Could not compose pedal preset", pedal, "=", pedalType);
  }
}

void PlaycontrollerProxy::sendSetting(uint16_t key, gint16 value)
{
  tMessageComposerPtr cmp(new MessageComposer(MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_SETTING));
  *cmp << key;
  *cmp << value;
  queueToPlaycontroller(cmp);
}

void PlaycontrollerProxy::onHeartbeatStumbled()
{
  auto settings = Application::get().getSettings();
  Application::get().getAudioEngineProxy()->sendEditBuffer();
  settings->sendSettingsToPlaycontroller(SendReason::HeartBeatDropped);
  settings->sendPresetSettingsToPlaycontroller();
  sendCalibrationData();
  requestPlaycontrollerSoftwareVersion();
  requestPlaycontrollerUHID();
  requestHWPositions();
  requestCalibrationStatus();
}

sigc::connection PlaycontrollerProxy::onPlaycontrollerSoftwareVersionChanged(const sigc::slot<void, int> &s)
{
  return m_signalPlaycontrollerSoftwareVersionChanged.connectAndInit(s, m_playcontrollerSoftwareVersion);
}

sigc::connection PlaycontrollerProxy::onLastKeyChanged(sigc::slot<void> s)
{
  return m_lastKeyChanged.connect(s);
}

void PlaycontrollerProxy::requestPlaycontrollerSoftwareVersion()
{
  sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_SW_VERSION);
}

void PlaycontrollerProxy::requestPlaycontrollerUHID()
{
  sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_UHID64);
}

void PlaycontrollerProxy::requestHWPositions()
{
  sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_POLLHWS);
}

std::string PlaycontrollerProxy::getPlaycontrollerSoftwareVersion() const
{
  return (m_playcontrollerSoftwareVersion == -1) ? "-" : std::to_string(m_playcontrollerSoftwareVersion);
}

void PlaycontrollerProxy::notifyKeyBedActionHappened()
{
  m_lastKeyChanged.send();
}

void PlaycontrollerProxy::sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes type)
{
  tMessageComposerPtr cmp(new MessageComposer(MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_REQUEST));
  uint16_t v = type;
  *cmp << v;
  queueToPlaycontroller(cmp);
}

sigc::connection PlaycontrollerProxy::onUHIDChanged(const sigc::slot<void, uint64_t> &s)
{
  return m_signalUHIDChanged.connectAndInit(s, m_uhid);
}

uint64_t PlaycontrollerProxy::getUHID() const
{
  return m_uhid;
}

void PlaycontrollerProxy::setUHID(uint64_t uhid)
{
  if(m_uhid != uhid)
  {
    m_uhid = uhid;
    m_signalUHIDChanged.send(m_uhid);
  }
}

sigc::connection PlaycontrollerProxy::onCalibrationStatusChanged(const sigc::slot<void, bool> &slot)
{
  return m_signalCalibrationStatus.connect(slot);
}

void PlaycontrollerProxy::requestCalibrationStatus()
{
  sendRequestToPlaycontroller(MessageParser::PlaycontrollerRequestTypes::PLAYCONTROLLER_REQUEST_ID_AT_STATUS);
}

template <typename tRet, typename tInValue>
tRet scaleValueToRange(const ValueRange<tTcdValue> &tcdRange, const tInValue &in, const ValueRange<tInValue> &inRange,
                       bool clip)
{
  tInValue inRangeWidth = inRange.getRangeWidth();
  tRet outRangeWidth = tcdRange.getRangeWidth();
  tInValue clippedIn = clip ? inRange.clip(in) : in;
  auto res = (clippedIn - inRange.getMin()) * outRangeWidth / inRangeWidth + tcdRange.getMin();

  if(std::numeric_limits<tRet>::is_integer)
    return lround(res);

  return res;
}

int16_t PlaycontrollerProxy::ribbonRelativeFactorToTCDValue(tControlPositionValue d)
{
  static ValueRange<tTcdValue> s_ribbonRelativeFactorTcdRange { 256, 2560 };
  return scaleValueToRange<int16_t>(s_ribbonRelativeFactorTcdRange, d, ValueRange<tControlPositionValue>(0, 1), false);
}

int16_t PlaycontrollerProxy::ribbonCPValueToTCDValue(tControlPositionValue d, bool bipolar)
{
  static ValueRange<tTcdValue> s_ribbonValueTcdRangeUnipolar { 0, 16000 };

  if(bipolar)
    return scaleValueToRange<int16_t>(s_ribbonValueTcdRangeUnipolar, d, ValueRange<tControlPositionValue>(-1, 1), false);
  else
    return scaleValueToRange<int16_t>(s_ribbonValueTcdRangeUnipolar, d, ValueRange<tControlPositionValue>(0, 1), false);
}

void PlaycontrollerProxy::onSelectedRibbonsChanged(const Setting *s)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto r1 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_1, VoiceGroup::Global }));
  auto r2 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_2, VoiceGroup::Global }));
  auto r3 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_3, VoiceGroup::Global }));
  auto r4 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_4, VoiceGroup::Global }));
  r1->sendModeToPlaycontroller();
  r2->sendModeToPlaycontroller();
  r3->sendModeToPlaycontroller();
  r4->sendModeToPlaycontroller();
}

sigc::connection PlaycontrollerProxy::onRibbonTouched(const sigc::slot<void, int> &s)
{
  return m_signalRibbonTouched.connectAndInit(s, m_lastTouchedRibbon);
}
