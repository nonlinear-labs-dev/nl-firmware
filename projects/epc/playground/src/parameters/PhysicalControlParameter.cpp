#include "PhysicalControlParameter.h"
#include "ModulationRoutingParameter.h"
#include "MacroControlParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/InfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/DebugLevel.h>
#include <groups/MacroControlMappingGroup.h>
#include <groups/HardwareSourcesGroup.h>
#include <presets/ParameterGroupSet.h>
#include <cmath>
#include "ParameterAlgorithm.h"
#include <http/UpdateDocumentMaster.h>
#include <Application.h>
#include <proxies/hwui/panel-unit/PanelUnitParameterEditMode.h>
#include <presets/EditBuffer.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <libundo/undo/Scope.h>

PhysicalControlParameter::PhysicalControlParameter(ParameterGroup *group, ParameterId id)
    : super(group, id)
{
}

bool PhysicalControlParameter::isChangedFromLoaded() const
{
  return false;
}

bool PhysicalControlParameter::isValueChangedFromLoaded() const
{
  return false;
}

double PhysicalControlParameter::getLastControlPositionValueBeforePresetLoad() const
{
  return m_valueBeforeLastLoad;
}

ReturnMode PhysicalControlParameter::getLastReturnModeBeforePresetLoad() const
{
  return m_returnModeBeforeLastLoad;
}

void PhysicalControlParameter::onChangeFromExternalSource(tControlPositionValue newValue, HWChangeSource source)
{
  if(source == HWChangeSource::MIDI)
    getValue().setRawValue(Initiator::EXPLICIT_MIDI, getValue().getFineQuantizedClippedValue(newValue));
  else
    getValue().setRawValue(Initiator::EXPLICIT_PLAYCONTROLLER, getValue().getFineQuantizedClippedValue(newValue));
}

void PhysicalControlParameter::setCPFromHwui(UNDO::Transaction *transaction, const tControlPositionValue &cpValue)
{
  m_changingFromHWUI = true;
  Parameter::setCPFromHwui(transaction, cpValue);
  m_changingFromHWUI = false;
  m_lastChangedFromHWUI = true;
}

void PhysicalControlParameter::setCPFromWebUI(UNDO::Transaction *transaction, const tControlPositionValue &cpValue)
{
  Parameter::setCPFromWebUI(transaction, cpValue);
}

void PhysicalControlParameter::onValueChanged(Initiator initiator, tControlPositionValue oldValue,
                                              tControlPositionValue newValue)
{
  if(!m_changingFromHWUI)
    m_lastChangedFromHWUI = false;

  if(initiator != Initiator::INDIRECT)
  {
    if(getReturnMode() != ReturnMode::None)
    {
      for(ModulationRoutingParameter *target : m_targets)
        target->applyPlaycontrollerPhysicalControl(newValue - oldValue);
    }
    else if(initiator != Initiator::EXPLICIT_LOAD)
    {
      for(ModulationRoutingParameter *target : m_targets)
        target->applyAbsolutePlaycontrollerPhysicalControl(newValue);
    }
  }

  super::onValueChanged(initiator, oldValue, newValue);
}

Glib::ustring PhysicalControlParameter::getDisplayString() const
{
  if(m_lastChangedFromHWUI && getReturnMode() != ReturnMode::None && getControlPositionValue() != 0.0)
    return Glib::ustring("! ") + Parameter::getDisplayString();

  return Parameter::getDisplayString();
}

void PhysicalControlParameter::loadFromPreset(UNDO::Transaction *transaction, const tControlPositionValue &value)
{
  m_changingFromHWUI = m_lastChangedFromHWUI;
  m_returnModeBeforeLastLoad = getReturnMode();
  m_valueBeforeLastLoad = getControlPositionValue();
  setIndirect(transaction, value);
}

void PhysicalControlParameter::registerTarget(ModulationRoutingParameter *target)
{
  m_targets.append(target);
}

size_t PhysicalControlParameter::getHash() const
{
  size_t hash = 0;
  hash_combine(hash, getLongName());
  return hash;
}

Glib::ustring PhysicalControlParameter::generateName() const
{
  auto it = std::max_element(m_targets.begin(), m_targets.end(),
                             [](const ModulationRoutingParameter *a, const ModulationRoutingParameter *b)
                             {
                               auto fa = fabs(a->getControlPositionValue());
                               auto fb = fabs(b->getControlPositionValue());

                               if(fa == fb)
                                 return a->getID() < b->getID();

                               return fa < fb;
                             });

  if(it == m_targets.end())
    return "Not assigned";

  ModulationRoutingParameter *target = *it;

  if(target->getControlPositionValue() == 0)
    return "Not assigned";

  return target->getTargetParameter()->getLongName();
}

Layout *PhysicalControlParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();

    default:
      return super::createLayout(focusAndMode);
  }

  g_return_val_if_reached(nullptr);
}

void PhysicalControlParameter::onSelected()
{
  super::onSelected();

  if(auto grandPa = dynamic_cast<const EditBuffer *>(getParent()->getParent()))
  {
    auto lastSelectedMacroControl = MacroControlParameter::getLastSelectedMacroControl();
    dynamic_cast<MacroControlParameter *>(grandPa->findParameterByID(lastSelectedMacroControl))
        ->setUiSelectedHardwareSource(getID());
  }
}

void PhysicalControlParameter::onUnselected()
{
  Parameter::onUnselected();

  if(m_lastChangedFromHWUI && getReturnMode() != ReturnMode::None)
  {
    m_lastChangedFromHWUI = false;
    getValue().setRawValue(Initiator::EXPLICIT_OTHER, 0);
    sendToAudioEngine();
    onChange(Generic | DontTrustOracle);
    invalidate();
  }
}

void PhysicalControlParameter::setUiSelectedModulationRouter(const ParameterId &paramNumber)
{
  auto current = getUiSelectedModulationRouter();

  if(current != paramNumber)
  {
    if(auto grandPa = dynamic_cast<ParameterGroupSet *>(getParent()->getParent()))
    {
      if(auto oldRouter = grandPa->findParameterByID(current))
        oldRouter->onUnselected();

      if(auto newRouter = dynamic_cast<ModulationRoutingParameter *>(grandPa->findParameterByID(paramNumber)))
        if(auto tgt = newRouter->getTargetParameter())
          tgt->onSelected();

      invalidate();

      if(auto u = std::dynamic_pointer_cast<PanelUnitParameterEditMode>(
             Application::get().getHWUI()->getPanelUnit().getUsageMode()))
      {
        u->bruteForceUpdateLeds();
      }
    }
  }
}

void PhysicalControlParameter::toggleUiSelectedModulationRouter(int inc)
{
  auto id = getUiSelectedModulationRouter();

  if(auto grandPa = dynamic_cast<ParameterGroupSet *>(getParent()->getParent()))
  {
    auto mappings
        = dynamic_cast<MacroControlMappingGroup *>(grandPa->getParameterGroupByID({ "MCM", VoiceGroup::Global }));
    auto routers = mappings->getModulationRoutingParametersFor(this);
    setUiSelectedModulationRouter(getIdOfAdvancedParameter(routers, id, inc));
  }
}

ParameterId PhysicalControlParameter::getUiSelectedModulationRouter() const
{
  auto grandPa = dynamic_cast<const ParameterGroupSet *>(getParent()->getParent());
  auto lastSelectedMacroControl = MacroControlParameter::getLastSelectedMacroControl();
  auto mc = dynamic_cast<MacroControlParameter *>(grandPa->findParameterByID(lastSelectedMacroControl));
  auto mcm = dynamic_cast<MacroControlMappingGroup *>(grandPa->getParameterGroupByID({ "MCM", VoiceGroup::Global }));

  if(auto router = mcm->getModulationRoutingParameterFor(this, mc))
    return router->getID();

  return mcm->getModulationRoutingParametersFor(this).front()->getID();
}

bool PhysicalControlParameter::isLocked() const
{
  return false;
}

bool PhysicalControlParameter::hasBehavior() const
{
  return false;
}

Glib::ustring PhysicalControlParameter::getCurrentBehavior() const
{
  throw std::bad_function_call();
}

void PhysicalControlParameter::undoableStepBehavior(UNDO::Transaction *transaction, int direction)
{
  throw std::bad_function_call();
}

void PhysicalControlParameter::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
}

void PhysicalControlParameter::sendParameterMessage() const
{
  if(auto eb = getParentEditBuffer())
    eb->getAudioEngineProxy().createAndSendParameterMessage<PhysicalControlParameter>(this);
}

bool PhysicalControlParameter::lockingEnabled() const
{
  return false;
}

HardwareSourceSendParameter *PhysicalControlParameter::getSendParameter() const
{
  auto idToSendID = [](auto id)
  {
    switch(id.getNumber())
    {
      case C15::PID::Pedal_1:
        return HardwareSourcesGroup::getPedal1SendID();
      case C15::PID::Pedal_2:
        return HardwareSourcesGroup::getPedal2SendID();
      case C15::PID::Pedal_3:
        return HardwareSourcesGroup::getPedal3SendID();
      case C15::PID::Pedal_4:
        return HardwareSourcesGroup::getPedal4SendID();
      case C15::PID::Ribbon_1:
        return HardwareSourcesGroup::getRibbon1SendID();
      case C15::PID::Ribbon_2:
        return HardwareSourcesGroup::getRibbon2SendID();
      case C15::PID::Aftertouch:
        return HardwareSourcesGroup::getAftertouchSendID();
      case C15::PID::Bender:
        return HardwareSourcesGroup::getBenderSendID();
    }
    return ParameterId::invalid();
  };

  return getParentEditBuffer()->findAndCastParameterByID<HardwareSourceSendParameter>({ idToSendID(getID()) });
}
