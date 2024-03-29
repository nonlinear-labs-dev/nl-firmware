#include "SplitPointParameter.h"
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableDualVoiceGroupMasterAndSplitPointLayout.h>
#include "groups/ParameterGroup.h"
#include "proxies/hwui/HWUI.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <parameter_declarations.h>
#include <libundo/undo/Scope.h>
#include <SplitPointRounding.h>

SplitPointParameter::SplitPointParameter(ParameterGroup* group, const ParameterId& id)
    : ModulateableParameter(group, id)
{
}

Layout* SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new SplitPointParameterEditLayout();
    case UIMode::Select:
    default:
      return new SplitPointParameterLayout();
  }
}

void SplitPointParameter::setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                                     bool notifyAudioEngine)
{
  const auto syncActive = isSynced();

  if(syncActive && isAtExtremes(value) && initiator != Initiator::INDIRECT_SPLIT_SYNC)
  {
    clampToExtremes(transaction, notifyAudioEngine);
    return;
  }

  Parameter::setCpValue(transaction, initiator, value, notifyAudioEngine);

  if(syncActive)
  {
    if(initiator != Initiator::INDIRECT_SPLIT_SYNC)
    {
      auto other = getSibling();
      auto siblingValue
          = getValue().getNextStepValue(value, other->getVoiceGroup() == VoiceGroup::I ? -1 : 1, false, false);
      other->setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, siblingValue, notifyAudioEngine);
    }
  }
}

void SplitPointParameter::clampToExtremes(UNDO::Transaction* transaction, bool notifyAudioEngine)
{
  auto other = getSibling();
  if(getVoiceGroup() == VoiceGroup::I)
  {
    auto myVal = getValue().getNextStepValue(1, -1, false, false);
    auto otherVal = 1.0;
    other->setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, otherVal, notifyAudioEngine);
    setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, myVal, notifyAudioEngine);
  }
  else if(getVoiceGroup() == VoiceGroup::II)
  {
    auto myVal = getValue().getNextStepValue(0, 1, false, false);
    auto otherVal = 0.0;
    other->setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, otherVal, notifyAudioEngine);
    setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, myVal, notifyAudioEngine);
  }
}

void SplitPointParameter::setModulationAmount(UNDO::Transaction* transaction, const tDisplayValue& amount)
{
  ModulateableParameter::setModulationAmount(transaction, amount);
  if(isSynced())
  {
    getSibling()->setModulationAmountFromSibling(transaction, amount);
  }
}

void SplitPointParameter::setModulationSource(UNDO::Transaction* transaction, MacroControls src)
{
  ModulateableParameter::setModulationSource(transaction, src);
  if(isSynced())
  {
    getSibling()->setModulationSourceFromSibling(transaction, src);
  }
}

Glib::ustring SplitPointParameter::stringizeModulationAmount(tControlPositionValue amount) const
{
  return std::to_string(static_cast<int>(60 * amount)) + " st";
}

SplitPointParameter* SplitPointParameter::getSibling() const
{
  auto parent = static_cast<ParameterGroup*>(getParent());
  auto groupSet = static_cast<ParameterGroupSet*>(parent->getParent());

  return static_cast<SplitPointParameter*>(groupSet->findParameterByID(
      { C15::PID::Split_Split_Point, getVoiceGroup() == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I }));
}

void SplitPointParameter::setModulationAmountFromSibling(UNDO::Transaction* transaction, const tDisplayValue& amount)
{
  ModulateableParameter::setModulationAmount(transaction, amount);
}

void SplitPointParameter::setModulationSourceFromSibling(UNDO::Transaction* transaction, MacroControls src)
{
  ModulateableParameter::setModulationSource(transaction, src);
}

bool SplitPointParameter::isAtExtremes(tControlPositionValue value)
{
  if(getVoiceGroup() == VoiceGroup::I)
  {
    return value > getValue().getNextStepValue(1, -1, false, false);
  }
  else if(getVoiceGroup() == VoiceGroup::II)
  {
    return value < getValue().getNextStepValue(0, 1, false, false);
  }

  return false;
}

bool SplitPointParameter::hasOverlap()
{
  if(getVoiceGroup() == VoiceGroup::I)
  {
    return getControlPositionValue() >= getSibling()->getControlPositionValue();
  }
  else
  {
    return getSibling()->hasOverlap();
  }
}

bool SplitPointParameter::inDefaultSplitBehaviour() const
{
  auto sib = getSibling();
  if(getVoiceGroup() == VoiceGroup::I)
  {
    auto sibPrev = sib->getNextStepValue(-1, {});
    return getValue().getQuantizedClipped() == sibPrev;
  }
  else
  {
    return sib->inDefaultSplitBehaviour();
  }
}

void SplitPointParameter::updateCPFromSyncChange(UNDO::Transaction* transaction, double cp)
{
  setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, cp, true);
}

void SplitPointParameter::loadDefault(UNDO::Transaction* transaction, Defaults mode)
{
  if(!isLocked())
  {
    undoableSelectModSource(transaction, MacroControls::NONE);
    undoableSetModAmount(transaction, 0.0);
    if(mode == Defaults::FactoryDefault)
      loadFromPreset(transaction, getFactoryDefaultValue());
    else
      loadFromPreset(transaction, getDefaultValue());
  }
}

bool SplitPointParameter::isSynced() const
{
  if(auto eb = getParentEditBuffer())
  {
    auto& settings = eb->getSettings();
    return settings.getSetting<SplitPointSyncParameters>()->get();
  }
  return false;
}

void SplitPointParameter::applyMacroControl(tDisplayValue mcValue, Initiator initiator)
{
  auto newValue
      = getModulationBase() + (double) SplitPointRounding::getModulation(mcValue, (float) getModulationAmount());
  getValue().setRawValue(initiator, newValue);
}

void SplitPointParameter::updateModulationBase()
{
  auto calcModulationBase = [this] {
    if(auto macroParam = getMacroControl())
    {
      auto _mod = (float) macroParam->getControlPositionValue();
      auto curValue = getValue().getQuantizedClipped();
      return curValue - (double) SplitPointRounding::getModulation(_mod, (float) getModulationAmount());
    }
    return 0.0;
  };

  setModulationBase(calcModulationBase());
}
