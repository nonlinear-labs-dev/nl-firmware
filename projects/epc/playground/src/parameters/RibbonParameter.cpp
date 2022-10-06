#include "RibbonParameter.h"
#include "scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "scale-converters/Linear100PercentScaleConverter.h"
#include "ModulationRoutingParameter.h"
#include "device-settings/SelectedRibbonsSetting.h"
#include <http/UpdateDocumentMaster.h>
#include <Application.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/InfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/PlayControlParameterLayouts.h>
#include <groups/HardwareSourcesGroup.h>
#include <groups/MacroControlMappingGroup.h>
#include <xml/Writer.h>
#include <cmath>
#include <libundo/undo/Transaction.h>
#include <presets/ParameterGroupSet.h>
#include <presets/PresetParameter.h>
#include <nltools/messaging/Message.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <libundo/undo/Scope.h>

void RibbonParameter::writeDocProperties(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("ribbon-touch-behaviour", to_string(m_touchBehaviour));
  writer.writeTextElement("ribbon-return-mode", to_string(m_returnMode));
}

void RibbonParameter::undoableSetRibbonTouchBehaviour(UNDO::Transaction *transaction, RibbonTouchBehaviour mode)
{
  if(m_touchBehaviour != mode)
  {
    auto swapData = UNDO::createSwapData(mode);

    transaction->addSimpleCommand(
        [=](UNDO::Command::State) mutable
        {
          swapData->swapWith(m_touchBehaviour);
          setupScalingAndDefaultValue(false);
        });
  }
  else
  {
    setupScalingAndDefaultValue(false);
  }
}

void RibbonParameter::setupScalingAndDefaultValue(bool defaultValue)
{
  getValue().setScaleConverter(createScaleConverter());
  getValue().setDefaultValue(getDefValueAccordingToMode());
  if(defaultValue)
    getValue().setToDefault(Initiator::INDIRECT);

  bool routersAreBoolean = getReturnMode() == ReturnMode::None;

  for(auto router : getRoutingParameters())
  {
    if(router->getValue().setIsBoolean(routersAreBoolean))
      router->onChange();
  }

  ensureExclusiveRoutingIfNeeded();
  invalidate();
  sendModeToPlaycontroller();
  m_updateIdWhenModeChanged = getUpdateIDOfLastChange();
}

std::list<ModulationRoutingParameter *> RibbonParameter::getRoutingParameters() const
{
  if(auto groups = dynamic_cast<ParameterGroupSet *>(getParentGroup()->getParent()))
  {
    auto group = groups->getParameterGroupByID({ "MCM", VoiceGroup::Global });
    if(auto mappings = dynamic_cast<MacroControlMappingGroup *>(group))
    {
      return mappings->getModulationRoutingParametersFor(this);
    }
  }
  return {};
}

Glib::ustring RibbonParameter::getLongName() const
{
  if(getRibbonTouchBehaviour() == RibbonTouchBehaviour::RELATIVE)
    return Parameter::getLongName() + "\uE282";
  return Parameter::getLongName();
}

Glib::ustring RibbonParameter::getShortName() const
{
  if(getRibbonTouchBehaviour() == RibbonTouchBehaviour::RELATIVE)
    return Parameter::getShortName() + "\uE282";
  return Parameter::getShortName();
}

tControlPositionValue RibbonParameter::getDefValueAccordingToMode() const
{
  switch(getReturnMode())
  {
    case ReturnMode::None:
      return 0.5;

    case ReturnMode::Center:
    case ReturnMode::Zero:
      return 0.0;
  }

  return 0.0;
}

void RibbonParameter::undoableSetRibbonTouchBehaviour(UNDO::Transaction *transaction, const Glib::ustring &mode)
{
  if(mode == "absolute")
    undoableSetRibbonTouchBehaviour(transaction, RibbonTouchBehaviour::ABSOLUTE);
  else if(mode == "relative")
    undoableSetRibbonTouchBehaviour(transaction, RibbonTouchBehaviour::RELATIVE);
}

void RibbonParameter::undoableIncRibbonTouchBehaviour(UNDO::Transaction *transaction)
{
  auto e = static_cast<int>(m_touchBehaviour);
  e++;

  if(e >= static_cast<int>(RibbonTouchBehaviour::NUM_TOUCH_BEHAVIOURS))
    e = 0;

  undoableSetRibbonTouchBehaviour(transaction, static_cast<RibbonTouchBehaviour>(e));
}

RibbonTouchBehaviour RibbonParameter::getRibbonTouchBehaviour() const
{
  return m_touchBehaviour;
}

void RibbonParameter::undoableSetRibbonReturnMode(UNDO::Transaction *transaction, RibbonReturnMode mode,
                                                  Initiator initiator)
{
  if(mode != RibbonReturnMode::STAY && mode != RibbonReturnMode::RETURN)
    mode = RibbonReturnMode::STAY;

  if(m_returnMode != mode)
  {
    auto swapData = UNDO::createSwapData(mode);

    undoableSetHWAmountsForReturnToCenterMode(transaction, mode);

    transaction->addSimpleCommand(
        [=](UNDO::Command::State) mutable
        {
          auto oldMode = m_returnMode;
          swapData->swapWith(m_returnMode);
          auto oldPos = getControlPositionValue();
          setupScalingAndDefaultValue(initiator == Initiator::EXPLICIT_USECASE
                                      && getRibbonReturnMode() == RibbonReturnMode::RETURN);
          if(initiator == Initiator::EXPLICIT_USECASE && oldMode == RibbonReturnMode::RETURN
             && getRibbonReturnMode() == RibbonReturnMode::STAY)
          {
            auto newPos = (oldPos * 0.5) + 0.5;
            getValue().setRawValue(initiator, newPos);
          }
          else if(initiator == Initiator::EXPLICIT_LOAD && oldMode == RibbonReturnMode::STAY
                  && getRibbonReturnMode() == RibbonReturnMode::RETURN)
          {
            setupScalingAndDefaultValue(true);
          }
          onChange();
        });
  }
  else
  {
    setupScalingAndDefaultValue(false);
    onChange();
  }
}

void RibbonParameter::undoableSetHWAmountsForReturnToCenterMode(UNDO::Transaction *transaction,
                                                                const RibbonReturnMode &mode) const
{
  if(mode == RibbonReturnMode::RETURN)
  {
    for(auto p : getRoutingParameters())
    {
      if(p->getControlPositionValue() > 0)
      {
        p->setCPFromHwui(transaction, 1);
      }
    }
  }
}

void RibbonParameter::undoableSetRibbonReturnMode(UNDO::Transaction *transaction, const Glib::ustring &mode,
                                                  Initiator initiator)
{
  if(mode == "stay")
    undoableSetRibbonReturnMode(transaction, RibbonReturnMode::STAY, initiator);
  else if(mode == "return")
    undoableSetRibbonReturnMode(transaction, RibbonReturnMode::RETURN, initiator);
}

RibbonReturnMode RibbonParameter::getRibbonReturnMode() const
{
  return m_returnMode;
}

ReturnMode RibbonParameter::getReturnMode() const
{
  if(m_returnMode == RibbonReturnMode::RETURN)
    return ReturnMode::Center;

  return ReturnMode::None;
}

void RibbonParameter::ensureExclusiveRoutingIfNeeded()
{
  if(getRibbonReturnMode() == RibbonReturnMode::STAY)
  {
    auto routers = getRoutingParameters();
    auto highest = *routers.begin();

    for(auto router : routers)
      if(highest != router)
        if(std::abs(router->getValue().getRawValue()) > std::abs(highest->getValue().getRawValue()))
          highest = router;

    for(auto router : routers)
      if(router != highest)
        router->onExclusiveRoutingLost();
  }
}

bool RibbonParameter::shouldWriteDocProperties(UpdateDocumentContributor::tUpdateID knownRevision) const
{
  return Parameter::shouldWriteDocProperties(knownRevision) || knownRevision <= m_updateIdWhenModeChanged;
}

const ScaleConverter *RibbonParameter::createScaleConverter() const
{
  if(m_returnMode == RibbonReturnMode::RETURN)
    return ScaleConverter::get<LinearBipolar100PercentScaleConverter>();

  return ScaleConverter::get<Linear100PercentScaleConverter>();
}

void RibbonParameter::sendModeToPlaycontroller() const
{
  const auto id = getID().getNumber();
  const auto isRibbon1 = id == C15::PID::Ribbon_1;
  const auto isRibbon3 = id == C15::PID::Ribbon_3;

  if(isRibbonCurrentlySelectedInBaseUnit())
  {
    uint16_t settingId = (isRibbon1 || isRibbon3) ? PLAYCONTROLLER_SETTING_ID_PLAY_MODE_UPPER_RIBBON_BEHAVIOUR
                                                  : PLAYCONTROLLER_SETTING_ID_PLAY_MODE_LOWER_RIBBON_BEHAVIOUR;
    uint16_t v = 0;

    if(getRibbonReturnMode() == RibbonReturnMode::RETURN)
      v = 1;

    if(getRibbonTouchBehaviour() == RibbonTouchBehaviour::RELATIVE)
      v = 2;

    if(Application::exists())
    {
      Application::get().getPlaycontrollerProxy()->sendSetting(settingId, v);
      sendToAudioEngine();
    }
  }
}

void RibbonParameter::copyFrom(UNDO::Transaction *transaction, const PresetParameter *other)
{
  if(!isLocked())
  {
    super::copyFrom(transaction, other);
    undoableSetRibbonReturnMode(transaction, other->getRibbonReturnMode(), Initiator::EXPLICIT_LOAD);
    undoableSetRibbonTouchBehaviour(transaction, other->getRibbonTouchBehaviour());
  }
}

void RibbonParameter::copyTo(UNDO::Transaction *transaction, PresetParameter *other) const
{
  super::copyTo(transaction, other);
  other->setField(transaction, PresetParameter::Fields::RibbonReturnMode, to_string(getRibbonReturnMode()));
  other->setField(transaction, PresetParameter::Fields::RibbonTouchBehaviour, to_string(getRibbonTouchBehaviour()));
}

void RibbonParameter::boundToMacroControl(tControlPositionValue v)
{
  getValue().setRawValue(Initiator::INDIRECT, v);
  onChange();
  invalidate();
}

RoutingSettings::tRoutingIndex indexFromID(const ParameterId& id)
{
  using tIndex = RoutingSettings::tRoutingIndex;
  switch(id.getNumber())
  {
    case C15::PID::Ribbon_1:
      return tIndex::Ribbon1;
    case C15::PID::Ribbon_2:
      return tIndex::Ribbon2;
    case C15::PID::Ribbon_3:
      return tIndex::Ribbon3;
    case C15::PID::Ribbon_4:
      return tIndex::Ribbon4;
  }
  nltools_assertAlways(false);
}

bool RibbonParameter::isLocalEnabled() const
{
  if(auto eb = getParentEditBuffer())
  {
    using tAspect = RoutingSettings::tAspectIndex;
    auto &s = eb->getSettings();
    const auto setting = s.getSetting<RoutingSettings>();
    const auto globalState = s.getSetting<GlobalLocalEnableSetting>()->get();
    auto state = setting->getState(indexFromID(getID()), tAspect::LOCAL);
    return state && globalState;
  }
  return false;
}

bool RibbonParameter::hasBehavior() const
{
  return true;
}

Glib::ustring RibbonParameter::getCurrentBehavior() const
{
  switch(m_returnMode)
  {
    case RibbonReturnMode::STAY:
      return "Non-Return";

    case RibbonReturnMode::RETURN:
      return "Return Center";

    default:
      g_assert_not_reached();
      break;
  }

  throw std::logic_error("unknown ribbon return mode");
}

void RibbonParameter::undoableStepBehavior(UNDO::Transaction *transaction, int direction)
{
  auto v = static_cast<int>(m_returnMode) + direction;
  auto numModes = static_cast<int>(RibbonReturnMode::NUM_RETURN_MODES);

  while(v < 0)
    v += numModes;

  while(v >= numModes)
    v -= numModes;

  undoableSetRibbonReturnMode(transaction, static_cast<RibbonReturnMode>(v), Initiator::EXPLICIT_USECASE);
}

void RibbonParameter::loadDefault(UNDO::Transaction *transaction, Defaults mode)
{
  super::loadDefault(transaction, mode);
  undoableSetRibbonReturnMode(transaction, RibbonReturnMode::STAY, Initiator::EXPLICIT_USECASE);
  undoableSetRibbonTouchBehaviour(transaction, RibbonTouchBehaviour::ABSOLUTE);
}

size_t RibbonParameter::getHash() const
{
  size_t hash = super::getHash();
  hash_combine(hash, (int) m_touchBehaviour);
  hash_combine(hash, (int) m_returnMode);
  return hash;
}

void RibbonParameter::sendToAudioEngine() const
{
  PhysicalControlParameter::sendToAudioEngine();

  auto proxy = Application::get().getPlaycontrollerProxy();
  const auto id = getID().getNumber();
  const auto isRibbon1 = id == C15::PID::Ribbon_1;
  const auto isRibbon3 = id == C15::PID::Ribbon_3;

  if(isRibbonCurrentlySelectedInBaseUnit())
  {
    uint16_t settingId = (isRibbon1 || isRibbon3) ? PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE
                                                  : PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_VALUE;
    auto newValue = proxy->ribbonCPValueToTCDValue(getValue().getQuantizedClipped(), isBiPolar());
    proxy->sendSetting(settingId, newValue);
  }
}

void RibbonParameter::onLocalEnableChanged(bool localEnableState)
{
  auto scope = UNDO::Scope::startTrashTransaction();

  if(localEnableState)  //Off -> On
  {
    if(getReturnMode() != ReturnMode::None)
    {
      auto oldSendPos = getSendParameter()->getControlPositionValue();
      getSendParameter()->setCPFromSetting(scope->getTransaction(), getDefValueAccordingToMode());
      PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), oldSendPos);
    }
    else
    {
      PhysicalControlParameter::setCPFromSetting(scope->getTransaction(),
                                                 getSendParameter()->getControlPositionValue());
    }
  }
  else  // On -> Off
  {
    if(getReturnMode() != ReturnMode::None)
    {
      getSendParameter()->setCPFromSetting(scope->getTransaction(), getControlPositionValue());
      PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), getDefValueAccordingToMode());
    }
    else
    {
      getSendParameter()->setCPFromSetting(scope->getTransaction(), getControlPositionValue());
    }
  }
}

void RibbonParameter::setCPFromSetting(UNDO::Transaction *transaction, const tControlPositionValue &cpValue)
{
  Parameter::setCPFromSetting(transaction, cpValue);

  const auto id = getID().getNumber();
  const auto isRibbon1 = id == C15::PID::Ribbon_1;
  const auto isRibbon3 = id == C15::PID::Ribbon_3;

  if(isRibbonCurrentlySelectedInBaseUnit())
  {
    auto settingId = (isRibbon1 || isRibbon3) ? PLAYCONTROLLER_SETTING_ID_UPPER_RIBBON_VALUE
                                              : PLAYCONTROLLER_SETTING_ID_LOWER_RIBBON_VALUE;
    auto proxy = Application::get().getPlaycontrollerProxy();
    auto v = proxy->ribbonCPValueToTCDValue(getValue().getQuantizedClipped(), isBiPolar());
    proxy->sendSetting(settingId, v);
  }
}

bool RibbonParameter::isRibbonCurrentlySelectedInBaseUnit() const
{
  auto selRibbons = getParentEditBuffer()->getSettings().getSetting<SelectedRibbonsSetting>()->get();
  const auto id = getID().getNumber();
  const auto isRibbon1 = id == C15::PID::Ribbon_1;
  const auto isRibbon2 = id == C15::PID::Ribbon_2;
  const auto isRibbon3 = id == C15::PID::Ribbon_3;
  const auto isRibbon4 = id == C15::PID::Ribbon_4;
  const auto isRibbon1_2 = selRibbons == SelectedRibbons::Ribbon1_2;
  const auto isRibbon3_4 = selRibbons == SelectedRibbons::Ribbon3_4;
  return (isRibbon1_2 && (isRibbon1 || isRibbon2)) || (isRibbon3_4 && (isRibbon3 || isRibbon4));
}
