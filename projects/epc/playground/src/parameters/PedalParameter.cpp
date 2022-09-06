#include <Application.h>
#include <device-settings/PedalType.h>
#include <device-settings/Settings.h>
#include <groups/HardwareSourcesGroup.h>
#include <groups/MacroControlMappingGroup.h>
#include <libundo/undo/Scope.h>
#include <libundo/undo/SwapData.h>
#include <libundo/undo/Transaction.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/value/QuantizedValue.h>
#include <playground.h>
#include <presets/ParameterGroupSet.h>
#include <presets/PresetParameter.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/PlayControlParameterLayouts.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <xml/Writer.h>
#include <presets/EditBuffer.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>

void PedalParameter::writeDocProperties(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("pedal-mode", to_string(m_mode));
}

void PedalParameter::undoableSetPedalMode(UNDO::Transaction *transaction, PedalModes mode, Initiator initiator)
{
  if(mode != PedalModes::STAY && mode != PedalModes::RETURN_TO_ZERO && mode != PedalModes::RETURN_TO_CENTER)
    mode = PedalModes::STAY;

  if(m_mode != mode)
  {
    auto swapData = UNDO::createSwapData(mode);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      swapData->swapWith(m_mode);
      getValue().setScaleConverter(createScaleConverter());
      getValue().setDefaultValue(getDefValueAccordingToMode());
      if(m_mode != PedalModes::STAY && initiator == Initiator::EXPLICIT_USECASE)
        getValue().setToDefault(Initiator::INDIRECT);

      setRoutersModeAccordingToReturnMode();

      invalidate();
      sendModeToPlaycontroller();
      m_updateIdWhenModeChanged = getUpdateIDOfLastChange();
    });
  }
  else
  {
    setRoutersModeAccordingToReturnMode();
    sendModeToPlaycontroller();
  }
}

void PedalParameter::setRoutersModeAccordingToReturnMode()
{
  bool routersAreBoolean = getReturnMode() == ReturnMode::None;
  if(auto eb = dynamic_cast<ParameterGroupSet *>(getParentGroup()->getParent()))
  {
    auto mappings = dynamic_cast<MacroControlMappingGroup *>(eb->getParameterGroupByID({ "MCM", VoiceGroup::Global }));

    for(auto router : mappings->getModulationRoutingParametersFor(this))
    {
      if(router->getValue().setIsBoolean(routersAreBoolean))
        router->onChange();
    }
  }
}

tControlPositionValue PedalParameter::getDefValueAccordingToMode() const
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

bool PedalParameter::shouldWriteDocProperties(UpdateDocumentContributor::tUpdateID knownRevision) const
{
  return Parameter::shouldWriteDocProperties(knownRevision) || knownRevision <= m_updateIdWhenModeChanged;
}

const ScaleConverter *PedalParameter::createScaleConverter() const
{
  if(m_mode == PedalModes::RETURN_TO_CENTER)
    return ScaleConverter::get<LinearBipolar100PercentScaleConverter>();

  return ScaleConverter::get<Linear100PercentScaleConverter>();
}

void PedalParameter::undoableSetPedalMode(UNDO::Transaction *transaction, const Glib::ustring &mode, Initiator initiator)
{
  if(mode == "stay")
    undoableSetPedalMode(transaction, PedalModes::STAY, initiator);
  else if(mode == "return-to-zero")
    undoableSetPedalMode(transaction, PedalModes::RETURN_TO_ZERO, initiator);
  else if(mode == "return-to-center")
    undoableSetPedalMode(transaction, PedalModes::RETURN_TO_CENTER, initiator);
}

void PedalParameter::sendModeToPlaycontroller() const
{
  if(Application::exists())
  {
    sendToPlaycontroller();
  }
}

ReturnMode PedalParameter::getReturnMode() const
{
  switch(m_mode)
  {
    case PedalModes::STAY:
      return ReturnMode::None;

    case PedalModes::RETURN_TO_CENTER:
      return ReturnMode::Center;

    case PedalModes::RETURN_TO_ZERO:
      return ReturnMode::Zero;

    case PedalModes::NUM_PEDAL_MODES:
    default:
      return ReturnMode::None;
  }
}

PedalModes PedalParameter::getPedalMode() const
{
  return m_mode;
}

void PedalParameter::copyFrom(UNDO::Transaction *transaction, const PresetParameter *other)
{
  if(!isLocked())
  {
    resetWasDefaulted(transaction);
    loadFromPreset(transaction, other->getValue());
    undoableSetPedalMode(transaction, other->getPedalMode(), Initiator::EXPLICIT_LOAD);
  }
}

void PedalParameter::copyTo(UNDO::Transaction *transaction, PresetParameter *other) const
{
  super::copyTo(transaction, other);
  other->setField(transaction, PresetParameter::Fields::PedalMode, to_string(getPedalMode()));
}

bool PedalParameter::hasBehavior() const
{
  return true;
}

Glib::ustring PedalParameter::getCurrentBehavior() const
{
  switch(m_mode)
  {
    case PedalModes::STAY:
      return "Non-Return";

    case PedalModes::RETURN_TO_ZERO:
      return "Return Zero";

    case PedalModes::RETURN_TO_CENTER:
      return "Return Center";
  }

  return PhysicalControlParameter::getCurrentBehavior();
}

void PedalParameter::undoableStepBehavior(UNDO::Transaction *transaction, int direction)
{
  int e = static_cast<int>(m_mode);
  e += direction;

  if(e >= static_cast<int>(PedalModes::NUM_PEDAL_MODES))
    e = 0;
  else if(e < 0)
    e = static_cast<int>(PedalModes::RETURN_TO_CENTER);

  undoableSetPedalMode(transaction, static_cast<PedalModes>(e), Initiator::EXPLICIT_USECASE);
}

PedalType *PedalParameter::getAssociatedPedalTypeSetting() const
{
  int key = 1;

  if(getID() == HardwareSourcesGroup::getPedal2ParameterID())
    key = 2;
  else if(getID() == HardwareSourcesGroup::getPedal3ParameterID())
    key = 3;
  else if(getID() == HardwareSourcesGroup::getPedal4ParameterID())
    key = 4;

  auto str = Glib::ustring::format("Pedal", key, "Type");
  return static_cast<PedalType *>(Application::get().getSettings()->getSetting(str));
}

void PedalParameter::loadDefault(UNDO::Transaction *transaction, Defaults mode)
{
  super::loadDefault(transaction, mode);
  undoableSetPedalMode(transaction, PedalModes::STAY, Initiator::EXPLICIT_USECASE);
}

size_t PedalParameter::getHash() const
{
  size_t hash = super::getHash();
  hash_combine(hash, (int) m_mode);
  return hash;
}

bool PedalParameter::isLocalEnabled() const
{
  if(auto eb = getParentEditBuffer())
  {
    using tIndex = RoutingSettings::tRoutingIndex;
    using tAspect = RoutingSettings::tAspectIndex;
    auto &s = eb->getSettings();
    const auto setting = s.getSetting<RoutingSettings>();
    const auto globalState = s.getSetting<GlobalLocalEnableSetting>()->get();

    tIndex index = tIndex::LENGTH;

    switch(getID().getNumber())
    {
      case C15::PID::Pedal_1:
        index = tIndex::Pedal1;
        break;
      case C15::PID::Pedal_2:
        index = tIndex::Pedal2;
        break;
      case C15::PID::Pedal_3:
        index = tIndex::Pedal3;
        break;
      case C15::PID::Pedal_4:
        index = tIndex::Pedal4;
        break;
    }

    if(index != tIndex::LENGTH)
    {
      auto state = setting->getState(index, tAspect::LOCAL);
      return state && globalState;
    }
  }
  return false;
}

void PedalParameter::onLocalEnableChanged(bool localEnableState)
{
  auto scope = UNDO::Scope::startTrashTransaction();
  const auto isReturning = getReturnMode() != ReturnMode::None;

  if(localEnableState)
  {
    auto oldSendPos = getSendParameter()->getControlPositionValue();
    PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), oldSendPos);
  }
  else
  {
    getSendParameter()->setCPFromSetting(scope->getTransaction(), getControlPositionValue());
    if(isReturning)
      PhysicalControlParameter::setCPFromSetting(scope->getTransaction(), getDefValueAccordingToMode());
  }
}