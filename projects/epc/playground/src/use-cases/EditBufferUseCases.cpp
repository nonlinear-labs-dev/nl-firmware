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

EditBufferUseCases::EditBufferUseCases(EditBuffer* eb)
    : m_editBuffer { eb }
{
}

IntrusiveList<Parameter*> getScaleParameters(EditBuffer* editBuffer)
{
  auto paramGroup = editBuffer->getParameterGroupByID({ "Scale", VoiceGroup::Global });
  return paramGroup->getParameters();
}

void EditBufferUseCases::resetScaleGroup()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Reset Scale Group");

  for(auto scaleParam : getScaleParameters(m_editBuffer))
  {
    auto transaction = scope->getTransaction();
    scaleParam->reset(transaction, Initiator::EXPLICIT_USECASE);
  }
}

void EditBufferUseCases::resetCustomScale()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Reset Custom Scale");
  auto scaleGroup = m_editBuffer->getParameterGroupByID({ "Scale", VoiceGroup::Global });
  scaleGroup->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_USECASE);
}

void EditBufferUseCases::undoableLoadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to)
{
  auto transactionName = preset->buildUndoTransactionTitle("Load Part " + toString(from) + " To Part " + toString(to));
  auto scope = m_editBuffer->getUndoScope().startTransaction(transactionName);
  m_editBuffer->undoableLoadToPart(scope->getTransaction(), preset, from, to);
}

void EditBufferUseCases::undoableLoad(const Preset* preset)
{
  if(m_editBuffer->getUUIDOfLastLoadedPreset() != preset->getUuid())
  {
    auto scope = m_editBuffer->getUndoScope().startTransaction(preset->buildUndoTransactionTitle("Load"));
    m_editBuffer->undoableLoad(scope->getTransaction(), preset, true);
  }
}

std::unique_ptr<ParameterUseCases> EditBufferUseCases::getUseCase(ParameterId id)
{
  if(auto parameter = m_editBuffer->findParameterByID(id))
    return std::move(std::make_unique<ParameterUseCases>(parameter));

  return nullptr;
}

std::unique_ptr<ModParameterUseCases> EditBufferUseCases::getModParamUseCase(ParameterId id)
{
  if(auto parameter = m_editBuffer->findAndCastParameterByID<ModulateableParameter>(id))
    return std::move(std::make_unique<ModParameterUseCases>(parameter));
  return nullptr;
}

std::unique_ptr<MacroControlParameterUseCases> EditBufferUseCases::getMCUseCase(ParameterId id)
{
  if(auto parameter = m_editBuffer->findAndCastParameterByID<MacroControlParameter>(id))
    return std::move(std::make_unique<MacroControlParameterUseCases>(parameter));

  return nullptr;
}

void EditBufferUseCases::selectParameter(const ParameterId& id)
{
  if(auto p = m_editBuffer->findParameterByID(id))
  {
    auto& undoScope = m_editBuffer->getUndoScope();
    auto groupAndParamName = p->getGroupAndParameterName();
    auto scope = undoScope.startContinuousTransaction(&p, std::chrono::hours(1), "Select '%0'", groupAndParamName);
    m_editBuffer->undoableSelectParameter(scope->getTransaction(), ParameterId(id));
  }

  //TODO: if parameter was already selected this code was called from editbuffer:
  //    auto hwui = Application::get().getHWUI();
  //    if(hwui->getFocusAndMode().mode == UIMode::Info)
  //      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Info));
  //    else
  //      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Select));
}

void EditBufferUseCases::selectParameter(const Parameter* p)
{
  if(p)
  {
    selectParameter(p->getID());
  }
}

void EditBufferUseCases::setParameter(const ParameterId& id, tControlPositionValue cp)
{
  if(auto p = m_editBuffer->findParameterByID(id))
  {
    Glib::ustring name {};
    if(m_editBuffer->getType() == SoundType::Single || ParameterId::isGlobal(id.getNumber()))
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterName());
    else
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterNameWithVoiceGroup());

    if(cp == p->getDefaultValue())
      name += " to Default";

    auto scope = m_editBuffer->getUndoScope().startContinuousTransaction(p, name);
    p->setCPFromWebUI(scope->getTransaction(), cp);
    m_editBuffer->onChange();
  }
}

void EditBufferUseCases::setModulationSource(MacroControls src, const ParameterId& id)
{
  if(auto p = m_editBuffer->findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto dual = m_editBuffer->isDual() && id.isDual();
    auto name = dual ? p->getGroupAndParameterNameWithVoiceGroup() : p->getGroupAndParameterName();
    auto scope = m_editBuffer->getUndoScope().startTransaction("Set MC Select for '%0'", name);
    p->undoableSelectModSource(scope->getTransaction(), src);
  }
}

void EditBufferUseCases::setModulationAmount(tControlPositionValue amount, const ParameterId& id)
{
  if(auto p = m_editBuffer->findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto dual = m_editBuffer->isDual() && id.isDual();
    auto name = dual ? p->getGroupAndParameterNameWithVoiceGroup() : p->getGroupAndParameterName();
    auto& undoScope = m_editBuffer->getUndoScope();
    auto scope = undoScope.startContinuousTransaction(p->getAmountCookie(), "Set MC Amount for '%0'", name);
    p->undoableSetModAmount(scope->getTransaction(), amount);
  }
}

void EditBufferUseCases::setModAmountAndValue(const ParameterId& id, tControlPositionValue modAmt,
                                              tControlPositionValue val)
{
  if(auto param = m_editBuffer->findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto isDual = m_editBuffer->isDual();
    auto withSuffix = isDual && id.isDual();
    auto name = withSuffix ? param->getGroupAndParameterNameWithVoiceGroup() : param->getGroupAndParameterName();
    auto scope = m_editBuffer->getUndoScope().startContinuousTransaction(param->getAmountCookie(),
                                                                         "Set Modulation Amount for '%0'", name);
    auto transaction = scope->getTransaction();
    param->undoableSetModAmount(transaction, modAmt);
    param->setCPFromHwui(transaction, val);
  }
}

void EditBufferUseCases::setSplits(const ParameterId& id, tControlPositionValue cp, tControlPositionValue otherCp)
{
  if(auto s = m_editBuffer->findAndCastParameterByID<SplitPointParameter>(ParameterId(id)))
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
  auto scope = m_editBuffer->getUndoScope().startTransaction("Mute " + toString(part));
  m_editBuffer->findParameterByID({ C15::PID::Voice_Grp_Mute, part })->setCPFromWebUI(scope->getTransaction(), 1);
}

void EditBufferUseCases::unmutePart(VoiceGroup part)
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Unmute " + toString(part));
  m_editBuffer->findParameterByID({ C15::PID::Voice_Grp_Mute, part })->setCPFromWebUI(scope->getTransaction(), 0);
}

void EditBufferUseCases::mutePartUnmuteOtherPart(VoiceGroup part)
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Mute " + toString(part));
  auto p = m_editBuffer->findParameterByID({ C15::PID::Voice_Grp_Mute, part });
  auto other = m_editBuffer->findParameterByID({ C15::PID::Voice_Grp_Mute, invert(part) });

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
  if(auto param = m_editBuffer->findAndCastParameterByID<ModulateableParameter>(id))
  {
    auto name = param->getGroupAndParameterName();
    auto scope = param->getUndoScope().startContinuousTransaction(param, "Set Modulation Limit for '%0'", name);
    param->undoableSetModAmount(scope->getTransaction(), modAmt);
    param->setCPFromHwui(scope->getTransaction(), paramVal);
  }
}

void EditBufferUseCases::unlockAllGroups()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Unlock all Groups");
  m_editBuffer->undoableUnlockAllGroups(scope->getTransaction());
}

void EditBufferUseCases::lockAllGroups()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Unlock all Groups");
  m_editBuffer->undoableLockAllGroups(scope->getTransaction());
}

void EditBufferUseCases::toggleLock(const std::string& groupName)
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Toggle Group Lock");
  m_editBuffer->undoableToggleGroupLock(scope->getTransaction(), groupName);
}

void EditBufferUseCases::loadSelectedPresetPartIntoPart(VoiceGroup from, VoiceGroup to)
{
  if(auto selectedPreset = m_editBuffer->getParent()->getSelectedPreset())
  {
    auto name = UNDO::StringTools::buildString("Load Preset Part", toString(from), "into", toString(to));
    auto scope = getPresetManager()->getUndoScope().startTransaction(name);
    m_editBuffer->undoableLoadToPart(scope->getTransaction(), selectedPreset, from, to);
  }
}

PresetManager* EditBufferUseCases::getPresetManager() const
{
  if(auto parentPM = dynamic_cast<PresetManager*>(m_editBuffer->getParent()))
  {
    return parentPM;
  }

  nltools_assertNotReached();
}