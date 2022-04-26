#include "EditBufferUseCases.h"
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <parameters/Parameter.h>
#include <parameters/SplitPointParameter.h>
#include <parameters/MacroControlParameter.h>
#include <libundo/undo/Scope.h>
#include <parameters/ModulateableParameter.h>
#include <parameter_declarations.h>
#include <presets/Bank.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/hwui/HWUI.h>
#include <presets/PresetPartSelection.h>

EditBufferUseCases::EditBufferUseCases(EditBuffer& eb)
    : m_editBuffer { eb }
{
}

IntrusiveList<Parameter*> getScaleParameters(EditBuffer& editBuffer)
{
  auto paramGroup = editBuffer.getParameterGroupByID({ "Scale", VoiceGroup::Global });
  return paramGroup->getParameters();
}

void EditBufferUseCases::resetScaleGroup()
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Reset Scale Group");

  for(auto scaleParam : getScaleParameters(m_editBuffer))
  {
    auto transaction = scope->getTransaction();
    scaleParam->reset(transaction, Initiator::EXPLICIT_USECASE);
  }
}

void EditBufferUseCases::resetCustomScale()
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Reset Custom Scale");
  auto scaleGroup = m_editBuffer.getParameterGroupByID({ "Scale", VoiceGroup::Global });
  scaleGroup->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_USECASE);
}

void EditBufferUseCases::loadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to)
{
  auto transactionName = preset->buildUndoTransactionTitle("Load Part " + toString(from) + " To Part " + toString(to));
  auto scope = m_editBuffer.getUndoScope().startTransaction(transactionName);
  m_editBuffer.undoableLoadToPart(scope->getTransaction(), preset, from, to);
}

void EditBufferUseCases::load(const Uuid& uuid)
{
  if(auto pm = m_editBuffer.getParent())
  {
    if(auto preset = pm->findPreset(uuid))
    {
      load(preset);
    }
  }
}

void EditBufferUseCases::load(const Preset* preset)
{
  load(preset, preset->buildUndoTransactionTitle("Load"));
}

void EditBufferUseCases::load(const Preset* preset, const std::string& transactionName)
{
  auto& undoScope = m_editBuffer.getUndoScope();
  auto scope = undoScope.startContinuousTransaction(preset->getParent(), std::chrono::seconds(5), transactionName);
  m_editBuffer.undoableLoad(scope->getTransaction(), preset, true);
}

std::unique_ptr<ParameterUseCases> EditBufferUseCases::getUseCase(const ParameterId& id)
{
  if(auto parameter = m_editBuffer.findParameterByID(id))
    return std::move(std::make_unique<ParameterUseCases>(parameter));

  return nullptr;
}

std::unique_ptr<ModParameterUseCases> EditBufferUseCases::getModParamUseCase(const ParameterId& id)
{
  if(auto parameter = m_editBuffer.findAndCastParameterByID<ModulateableParameter>(id))
    return std::move(std::make_unique<ModParameterUseCases>(parameter));
  return nullptr;
}

std::unique_ptr<MacroControlParameterUseCases> EditBufferUseCases::getMCUseCase(const ParameterId& id)
{
  if(auto parameter = m_editBuffer.findAndCastParameterByID<MacroControlParameter>(id))
    return std::move(std::make_unique<MacroControlParameterUseCases>(parameter));

  return nullptr;
}

void EditBufferUseCases::selectParameter(const ParameterId& id, bool sendReselectionSignal)
{
  if(auto p = m_editBuffer.findParameterByID(id))
  {
    auto& undoScope = m_editBuffer.getUndoScope();
    auto groupAndParamName = p->getGroupAndParameterName();
    auto scope = undoScope.startContinuousTransaction(&p, std::chrono::hours(1), "Select '%0'", groupAndParamName);
    m_editBuffer.undoableSelectParameter(scope->getTransaction(), ParameterId(id), sendReselectionSignal);
  }
}

void EditBufferUseCases::selectParameter(const Parameter* p, bool sendReselectionSignal)
{
  if(p)
  {
    selectParameter(p->getID(), sendReselectionSignal);
  }
}

void EditBufferUseCases::setParameter(const ParameterId& id, tControlPositionValue cp)
{
  if(auto p = m_editBuffer.findParameterByID(id))
  {
    Glib::ustring name {};
    if(m_editBuffer.getType() == SoundType::Single || ParameterId::isGlobal(id.getNumber()))
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterName());
    else
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterNameWithVoiceGroup());

    if(cp == p->getDefaultValue())
      name += " to Default";

    auto scope = m_editBuffer.getUndoScope().startContinuousTransaction(p, name);
    p->setCPFromWebUI(scope->getTransaction(), cp);
    m_editBuffer.onChange();
  }
}

void EditBufferUseCases::setModulationSource(MacroControls src, const ParameterId& id)
{
  if(auto p = m_editBuffer.findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto dual = m_editBuffer.isDual() && id.isDual();
    auto name = dual ? p->getGroupAndParameterNameWithVoiceGroup() : p->getGroupAndParameterName();
    auto scope = m_editBuffer.getUndoScope().startTransaction("Set MC Select for '%0'", name);
    p->undoableSelectModSource(scope->getTransaction(), src);
  }
}

void EditBufferUseCases::setModulationAmount(tControlPositionValue amount, const ParameterId& id)
{
  if(auto p = m_editBuffer.findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto dual = m_editBuffer.isDual() && id.isDual();
    auto name = dual ? p->getGroupAndParameterNameWithVoiceGroup() : p->getGroupAndParameterName();
    auto& undoScope = m_editBuffer.getUndoScope();
    auto scope = undoScope.startContinuousTransaction(p->getAmountCookie(), "Set MC Amount for '%0'", name);
    p->undoableSetModAmount(scope->getTransaction(), amount);
  }
}

void EditBufferUseCases::setModAmountAndValue(const ParameterId& id, tControlPositionValue modAmt,
                                              tControlPositionValue val)
{
  if(auto param = m_editBuffer.findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto isDual = m_editBuffer.isDual();
    auto withSuffix = isDual && id.isDual();
    auto name = withSuffix ? param->getGroupAndParameterNameWithVoiceGroup() : param->getGroupAndParameterName();
    auto scope = m_editBuffer.getUndoScope().startContinuousTransaction(param->getAmountCookie(),
                                                                         "Set Modulation Amount for '%0'", name);
    auto transaction = scope->getTransaction();
    param->undoableSetModAmount(transaction, modAmt);
    param->setCPFromHwui(transaction, val);
  }
}

void EditBufferUseCases::setSplits(const ParameterId& id, tControlPositionValue cp, tControlPositionValue otherCp)
{
  if(auto s = m_editBuffer.findAndCastParameterByID<SplitPointParameter>(ParameterId(id)))
  {
    auto other = s->getSibling();
    auto name = s->getGroupAndParameterNameWithVoiceGroup();
    auto scope = s->getUndoScope().startContinuousTransaction(s, "Set '%0'", name);
    s->setCPFromWebUI(scope->getTransaction(), cp);
    other->setCPFromWebUI(scope->getTransaction(), otherCp);
  }
}

void EditBufferUseCases::mutePart(VoiceGroup part)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Mute " + toString(part));
  m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, part })->setCPFromWebUI(scope->getTransaction(), 1);
}

void EditBufferUseCases::unmutePart(VoiceGroup part)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Unmute " + toString(part));
  m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, part })->setCPFromWebUI(scope->getTransaction(), 0);
}

void EditBufferUseCases::mutePartUnmuteOtherPart(VoiceGroup part)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Mute " + toString(part));
  auto p = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, part });
  auto other = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, invert(part) });

  p->setCPFromWebUI(scope->getTransaction(), 1);
  other->setCPFromWebUI(scope->getTransaction(), 0);
}

VoiceGroup EditBufferUseCases::invert(VoiceGroup vg)
{
  return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
}

void EditBufferUseCases::setModulationLimit(const ParameterId& id, tControlPositionValue modAmt,
                                            tControlPositionValue paramVal)
{
  if(auto param = m_editBuffer.findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto name = param->getGroupAndParameterName();
    auto scope = param->getUndoScope().startContinuousTransaction(param, "Set Modulation Limit for '%0'", name);
    param->undoableSetModAmount(scope->getTransaction(), modAmt);
    param->setCPFromHwui(scope->getTransaction(), paramVal);
  }
}

void EditBufferUseCases::unlockAllGroups()
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Unlock all Groups");
  m_editBuffer.undoableUnlockAllGroups(scope->getTransaction());
}

void EditBufferUseCases::lockAllGroups()
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Unlock all Groups");
  m_editBuffer.undoableLockAllGroups(scope->getTransaction());
}

void EditBufferUseCases::toggleLock(const std::string& groupName)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Toggle Group Lock");
  m_editBuffer.undoableToggleGroupLock(scope->getTransaction(), groupName);
}

PresetManager* EditBufferUseCases::getPresetManager() const
{
  if(auto parentPM = dynamic_cast<PresetManager*>(m_editBuffer.getParent()))
  {
    return parentPM;
  }

  nltools_assertNotReached();
}

void EditBufferUseCases::toggleMute(VoiceGroup part)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Toggle Mute " + toString(part));
  auto transaction = scope->getTransaction();
  if(auto mute = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, part }))
  {
    if(mute->getControlPositionValue() >= 0.5)
    {
      mute->setCPFromHwui(transaction, 0);
    }
    else
    {
      mute->setCPFromHwui(transaction, 1);
    }
  }
}

void EditBufferUseCases::unmuteBothPartsWithTransactionNameForPart(VoiceGroup part)
{
  auto scope = m_editBuffer.getParent()->getUndoScope().startTransaction("Unmute Part " + toString(part));
  auto mute = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, part });
  auto muteOther = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Mute, invert(part) });
  mute->setCPFromHwui(scope->getTransaction(), 0);
  muteOther->setCPFromHwui(scope->getTransaction(), 0);
}

void EditBufferUseCases::unlockGroup(ParameterGroup* group)
{
  if(group)
  {
    auto scope = m_editBuffer.getUndoScope().startTransaction("Unlock Group");
    group->undoableUnlock(scope->getTransaction());
  }
}

void EditBufferUseCases::lockGroup(ParameterGroup* group)
{
  if(group)
  {
    auto scope = m_editBuffer.getUndoScope().startTransaction("Lock Group");
    group->undoableLock(scope->getTransaction());
  }
}

void EditBufferUseCases::renamePart(VoiceGroup part, const Glib::ustring& name)
{
  auto scope = m_editBuffer.getParent()->getUndoScope().startTransaction("Rename Part to %s", name);
  m_editBuffer.setVoiceGroupName(scope->getTransaction(), name, part);
}

void EditBufferUseCases::undoableLoadAccordingToType(Preset* pPreset, HWUI* hwui)
{
  auto currentVoiceGroup = hwui->getCurrentVoiceGroup();

  if(pPreset)
  {
    auto loadToPartActive = hwui->isInLoadToPart();

    if(loadToPartActive)
    {
      auto load = hwui->getPresetPartSelection(currentVoiceGroup);
      loadToPart(load->m_preset, load->m_voiceGroup, currentVoiceGroup);
    }
    else
    {
      load(pPreset);
    }
  }
}

void EditBufferUseCases::initSound(Defaults defaults)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Init Sound");
  m_editBuffer.undoableInitSound(scope->getTransaction(), defaults);
}

void EditBufferUseCases::initPart(VoiceGroup part, Defaults defaults)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Init Part");
  m_editBuffer.undoableInitPart(scope->getTransaction(), part, defaults);
}

void EditBufferUseCases::convertToLayer(VoiceGroup currentSelectedVoiceGroup)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Convert Sound to Layer");
  auto transaction = scope->getTransaction();
  m_editBuffer.undoableConvertToDual(transaction, SoundType::Layer, currentSelectedVoiceGroup);
}

void EditBufferUseCases::convertToSingle(VoiceGroup partToUse)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Convert Sound to Single");
  auto transaction = scope->getTransaction();
  m_editBuffer.undoableConvertToSingle(transaction, partToUse);
}

void EditBufferUseCases::convertToSplit(VoiceGroup currentSelectedVoiceGroup)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Convert Sound to Split");
  auto transaction = scope->getTransaction();
  m_editBuffer.undoableConvertToDual(transaction, SoundType::Split, currentSelectedVoiceGroup);
}

void EditBufferUseCases::convertToDual(SoundType type, VoiceGroup group)
{
  switch(type)
  {
    case SoundType::Single:
      convertToSingle(group);
      break;
    case SoundType::Split:
      convertToSplit(group);
      break;
    case SoundType::Layer:
      convertToLayer(group);
      break;
    case SoundType::Invalid:
      break;
  }
}

void EditBufferUseCases::randomize(double amount)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Randomize Sound");
  m_editBuffer.undoableRandomize(scope->getTransaction(), Initiator::EXPLICIT_HWUI, amount);
}

void EditBufferUseCases::randomizePart(VoiceGroup part, double amount)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Randomize Part");
  m_editBuffer.undoableRandomizePart(scope->getTransaction(), part, Initiator::EXPLICIT_WEBUI, amount);
}

void EditBufferUseCases::setModulationSourceOfAll(MacroControls controls)
{
  auto scope = m_editBuffer.getUndoScope().startTransaction("Set all Modulation Sources to %s", toString(controls));
  auto trans = scope->getTransaction();

  for(auto& groups: m_editBuffer.getParameters())
  {
    for(auto& g: groups)
    {
      for(auto& p: g->getParameters())
      {
        if(auto mod = dynamic_cast<ModulateableParameter*>(p))
        {
          mod->setModulationSource(trans, controls);
          mod->setModulationAmount(trans, 1);
        }
      }
    }
  }
}
