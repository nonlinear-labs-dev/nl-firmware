#include "EditBuffer.h"
#include "device-settings/DebugLevel.h"
#include "parameters/Parameter.h"
#include "xml/Writer.h"
#include <http/ContentManager.h>
#include "Application.h"
#include <presets/Preset.h>
#include <presets/Bank.h>
#include "presets/PresetManager.h"
#include "EditBufferSnapshotMaker.h"
#include "proxies/lpc/LPCProxy.h"
#include <proxies/audio-engine/AudioEngineProxy.h>
#include "parameters/ModulateableParameter.h"
#include <parameters/PhysicalControlParameter.h>
#include <device-settings/DeviceName.h>
#include "device-settings/RandomizeAmount.h"
#include "device-info/DeviceInformation.h"
#include "parameters/MacroControlParameter.h"
#include <libundo/undo/Transaction.h>
#include <parameters/AftertouchParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PitchbendParameter.h>
#include <nltools/Testing.h>
#include <testing/EditBufferTests.h>
#include <nltools/Types.h>

EditBuffer::EditBuffer(PresetManager *parent)
    : ParameterDualGroupSet(parent)
    , m_deferredJobs(100, std::bind(&EditBuffer::doDeferedJobs, this))
    , m_isModified(false)
    , m_recallSet(this)
    , m_type(SoundType::Single)
    , m_lastSelectedParameter{ 0, VoiceGroup::I }
{
  m_hashOnStore = getHash();
}

EditBuffer::~EditBuffer()
{
  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);
}

void EditBuffer::initRecallValues(UNDO::Transaction *transaction)
{
  m_recallSet.copyFromEditBuffer(transaction, this);
}

SoundType EditBuffer::getType() const
{
  return m_type;
}

Glib::ustring EditBuffer::getName(VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid || m_type == SoundType::Single)
    return getName();
  else
    return toString(vg) + " " + getName();
}

Glib::ustring EditBuffer::getName() const
{
  if(auto o = getOrigin())
    return o->getName();

  return m_name;
}

size_t EditBuffer::getHash() const
{
  size_t hash = AttributesOwner::getHash();

  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
    for(const auto g : getParameterGroups(vg))
      hash_combine(hash, g->getHash());

  hash_combine(hash, static_cast<int>(getType()));

  return hash;
}

const Preset *EditBuffer::getOrigin() const
{
  if(m_originCache && m_originCache->getUuid() == m_lastLoadedPreset)
    return m_originCache;

  m_originCache = getParent()->findPreset(m_lastLoadedPreset);
  return m_originCache;
}

RecallParameterGroups &EditBuffer::getRecallParameterSet()
{
  return m_recallSet;
}

void EditBuffer::resetModifiedIndicator(UNDO::Transaction *transaction)
{
  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::resetModifiedIndicator(UNDO::Transaction *transaction, size_t hash)
{
  auto swap = UNDO::createSwapData(false, hash);

  transaction->addSimpleCommand([=](UNDO::Command::State) {
    swap->swapWith<0>(m_isModified);
    swap->swapWith<1>(m_hashOnStore);
    m_signalModificationState.send(m_isModified);
  });
}

bool EditBuffer::isModified() const
{
  return m_isModified;
}

connection EditBuffer::onModificationStateChanged(const slot<void, bool> &s)
{
  return m_signalModificationState.connectAndInit(s, m_isModified);
}

connection EditBuffer::onChange(const slot<void> &s)
{
  return m_signalChange.connectAndInit(s);
}

connection EditBuffer::onPresetLoaded(const slot<void> &s)
{
  return m_signalPresetLoaded.connect(s);
}

connection EditBuffer::onLocksChanged(const slot<void> &s)
{
  return m_signalLocksChanged.connectAndInit(s);
}

connection EditBuffer::onRecallValuesChanged(const slot<void> &s)
{
  return m_recallSet.m_signalRecallValues.connect(s);
}

connection EditBuffer::onSoundTypeChanged(slot<void> s)
{
  return m_signalTypeChanged.connect(s);
}

UpdateDocumentContributor::tUpdateID EditBuffer::onChange(uint64_t flags)
{
  m_deferredJobs.trigger();
  m_signalChange.send();

  if(flags & ChangeFlags::LockState)
  {
    m_signalLocksChanged.deferedSend();
  }

  return ParameterDualGroupSet::onChange(flags);
}

void EditBuffer::doDeferedJobs()
{
  checkModified();
}

void EditBuffer::checkModified()
{
  auto currentHash = getHash();
  bool modified = currentHash != m_hashOnStore;

  if(m_isModified != modified)
  {
    m_isModified = modified;
    m_signalModificationState.send(m_isModified);

    auto cm = dynamic_cast<ContentManager *>(getRoot());

    if(!cm->isSendResponsesScheduled())
      onChange();
  }
}

PresetManager *EditBuffer::getParent()
{
  return static_cast<PresetManager *>(super::getParent());
}

const PresetManager *EditBuffer::getParent() const
{
  return static_cast<const PresetManager *>(super::getParent());
}

sigc::connection EditBuffer::onSelectionChanged(const slot<void, Parameter *, Parameter *> &s)
{
  return m_signalSelectedParameter.connectAndInit(s, nullptr, getSelected());
}

void EditBuffer::undoableSelectParameter(const Glib::ustring &id)
{
  undoableSelectParameter(std::stoi(id));
}

void EditBuffer::undoableSelectParameter(uint16_t id)
{
  if(auto p = findParameterByID(id))
    undoableSelectParameter(p);
  else if(auto g = findGlobalParameterByID(id))
    undoableSelectParameter(g);
}

void EditBuffer::undoableSelectParameter(uint16_t id, VoiceGroup vg)
{
  if(auto p = findParameterByID(id, vg))
    undoableSelectParameter(p);

  if(auto p = findGlobalParameterByID(id))
    undoableSelectParameter(p);
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, const Glib::ustring &id)
{
  if(auto p = findParameterByID(std::stoi(id), VoiceGroup::I))
    undoableSelectParameter(transaction, p);
}

void EditBuffer::setParameter(size_t id, double cpValue, VoiceGroup vg)
{
  if(auto p = findParameterByID(id, vg))
  {
    DebugLevel::gassy("EditBuffer::setParameter", id, cpValue, toString(vg));
    Glib::ustring name{};
    if(m_type == SoundType::Single)
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterName());
    else
      name = UNDO::StringTools::formatString("Set '%0' [%1]", p->getGroupAndParameterName(), toString(vg));

    if(cpValue == p->getDefaultValue())
      name += " to Default";

    auto scope = getUndoScope().startContinuousTransaction(p, name);
    p->setCPFromWebUI(scope->getTransaction(), cpValue);
    onChange();
  }
}

void EditBuffer::setModulationSource(MacroControls src)
{
  if(auto p = dynamic_cast<ModulateableParameter *>(getSelected()))
  {
    auto scope = getUndoScope().startTransaction("Set MC Select for '%0'", p->getLongName());
    p->undoableSelectModSource(scope->getTransaction(), src);
  }
}

void EditBuffer::setModulationAmount(double amount)
{
  if(auto p = dynamic_cast<ModulateableParameter *>(getSelected()))
  {
    auto scope = getUndoScope().startContinuousTransaction(p->getAmountCookie(), "Set MC Amount for '%0'",
                                                           p->getGroupAndParameterName());
    p->undoableSetModAmount(scope->getTransaction(), amount);
  }
}

bool EditBuffer::hasLocks(VoiceGroup vg) const
{
  return searchForAnyParameterWithLock(vg) != nullptr;
}

bool EditBuffer::anyParameterChanged(VoiceGroup vg) const
{
  for(auto &paramGroup : getParameterGroups(vg))
  {
    for(auto &param : paramGroup->getParameters())
    {
      if(param->isChangedFromLoaded())
      {
        return true;
      }
    }
  }
  return false;
}

bool EditBuffer::anyParameterChanged() const
{
  if(m_type == SoundType::Single)
  {
    return anyParameterChanged(VoiceGroup::I);
  }
  else
  {
    return anyParameterChanged(VoiceGroup::I) || anyParameterChanged(VoiceGroup::II);
  }
}

void EditBuffer::resetOriginIf(const Preset *p)
{
  if(m_originCache == p)
    m_originCache = nullptr;
}

void EditBuffer::undoableSelectParameter(Parameter *p)
{
  if(p->getID() != m_lastSelectedParameter.m_id || p->getVoiceGroup() != m_lastSelectedParameter.m_voiceGroup)
  {
    auto newSelection = p;
    auto scope = getUndoScope().startContinuousTransaction(&newSelection, std::chrono::hours(1), "Select '%0'",
                                                           p->getGroupAndParameterName());
    undoableSelectParameter(scope->getTransaction(), p);
  }
  else
  {
    auto hwui = Application::get().getHWUI();

    if(hwui->getFocusAndMode().mode == UIMode::Info)
      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Info));
    else
      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Select));
  }
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p)
{
  if(m_lastSelectedParameter.m_id != p->getID())
  {
    auto swapData = UNDO::createSwapData(LastSelection(p->getID(), p->getVoiceGroup()));

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      auto oldSelection = m_lastSelectedParameter;

      swapData->swapWith(m_lastSelectedParameter);

      auto oldP = findParameterByID(oldSelection.m_id, oldSelection.m_voiceGroup);
      auto newP = findParameterByID(m_lastSelectedParameter.m_id, m_lastSelectedParameter.m_voiceGroup);

      m_signalSelectedParameter.send(oldP, newP);

      if(oldP)
        oldP->onUnselected();

      if(newP)
        newP->onSelected();

      if(!getParent()->isLoading())
      {
        if(auto hwui = Application::get().getHWUI())
        {
          hwui->setFocusAndMode(UIFocus::Parameters);
        }
      }

      onChange();
    });

    if(auto hwui = Application::get().getHWUI())
    {
      hwui->unsetFineMode();
    }
  }
}

bool isIDMonoGroup(int id)
{
  return id >= 12345 && id <= 12348;
}

Parameter *EditBuffer::getSelected() const
{
  if(m_lastSelectedParameter.m_voiceGroup == VoiceGroup::Global)
    if(auto p = findGlobalParameterByID(m_lastSelectedParameter.m_id))
      return p;

  return findParameterByID(m_lastSelectedParameter.m_id, m_lastSelectedParameter.m_voiceGroup);
}

Parameter *EditBuffer::getSelected(VoiceGroup vg) const
{
  if(auto p = findParameterByID(m_lastSelectedParameter.m_id, vg))
    return p;

  return findGlobalParameterByID(m_lastSelectedParameter.m_id);
}

void EditBuffer::setName(UNDO::Transaction *transaction, const ustring &name)
{
  transaction->addUndoSwap(this, m_name, name);
}

bool EditBuffer::isZombie() const
{

  if(getUUIDOfLastLoadedPreset() == Uuid::init())
    return false;

  return !getParent()->findPreset(getUUIDOfLastLoadedPreset());
}

void EditBuffer::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  auto changed = knownRevision < ParameterDualGroupSet::getUpdateIDOfLastChange();
  auto pm = static_cast<const PresetManager *>(getParent());
  auto origin = pm->findPreset(getUUIDOfLastLoadedPreset());
  auto zombie = isZombie();
  auto bank = origin ? dynamic_cast<const Bank *>(origin->getParent()) : nullptr;
  auto bankName = bank ? bank->getName(true) : "";

  writer.writeTag(
      "edit-buffer",
      { Attribute("selected-parameter", m_lastSelectedParameter.m_id), Attribute("editbuffer-type", toString(m_type)),
        Attribute("loaded-preset", getUUIDOfLastLoadedPreset().raw()), Attribute("loaded-presets-name", getName()),
        Attribute("loaded-presets-bank-name", bankName), Attribute("preset-is-zombie", zombie),
        Attribute("is-modified", m_isModified), Attribute("hash", getHash()), Attribute("changed", changed) },
      [&]() {
        if(changed)
          super::writeDocument(writer, knownRevision);

        m_recallSet.writeDocument(writer, knownRevision);
      });
}

void EditBuffer::undoableLoadSelectedPreset()
{
  if(auto bank = getParent()->getSelectedBank())
  {
    if(auto preset = bank->getSelectedPreset())
    {
      undoableLoad(preset);
    }
  }
}

void EditBuffer::undoableLoad(Preset *preset, VoiceGroup target)
{
  UNDO::Scope::tTransactionScopePtr scope = getUndoScope().startTransaction(preset->buildUndoTransactionTitle("Load"));
  undoableLoad(scope->getTransaction(), preset);
}

void EditBuffer::undoableLoad(UNDO::Transaction *transaction, Preset *preset)
{
  auto lpc = Application::get().getLPCProxy();
  auto ae = Application::get().getAudioEngineProxy();
  lpc->toggleSuppressParameterChanges(transaction);
  ae->toggleSuppressParameterChanges(transaction);

  copyFrom(transaction, preset);
  undoableSetLoadedPresetInfo(transaction, preset);

  if(auto bank = static_cast<Bank *>(preset->getParent()))
  {
    auto pm = static_cast<PresetManager *>(getParent());
    bank->selectPreset(transaction, preset->getUuid());
    pm->selectBank(transaction, bank->getUuid());
  }

  lpc->toggleSuppressParameterChanges(transaction);
  ae->toggleSuppressParameterChanges(transaction);
  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::copyFrom(UNDO::Transaction *transaction, const Preset *preset)
{
  EditBufferSnapshotMaker::get().addSnapshotIfRequired(transaction);

  if(preset->getType() == SoundType::Single)
    undoableConvertToSingle(transaction, VoiceGroup::I);
  else if(preset->getType() == SoundType::Split || preset->getType() == SoundType::Layer)
    undoableConvertToDual(transaction, preset->getType());

  super::copyFrom(transaction, preset);

  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::undoableSetLoadedPresetInfo(UNDO::Transaction *transaction, Preset *preset)
{
  Uuid newId = Uuid::none();
  if(preset)
  {
    setName(transaction, preset->getName());
    newId = preset->getUuid();
  }

  auto swap = UNDO::createSwapData(std::move(newId));

  transaction->addSimpleCommand([=](auto) {
    swap->swapWith(m_lastLoadedPreset);
    m_signalPresetLoaded.send();
    onChange();
  });

  initRecallValues(transaction);
}

void EditBuffer::undoableUpdateLoadedPresetInfo(UNDO::Transaction *transaction)
{
  auto pm = static_cast<PresetManager *>(getParent());
  undoableSetLoadedPresetInfo(transaction, pm->findPreset(getUUIDOfLastLoadedPreset()));
}

void EditBuffer::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator)
{
  UNDO::ActionCommand::tAction sendEditBuffer([](auto) -> void { EditBuffer::sendToLPC(); });
  transaction->addSimpleCommand(UNDO::ActionCommand::tAction(), sendEditBuffer);

  auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto group : getParameterGroups(vg))
      group->undoableRandomize(transaction, initiator, amount);

  transaction->addSimpleCommand(sendEditBuffer, UNDO::ActionCommand::tAction());
}

void EditBuffer::undoableInitSound(UNDO::Transaction *transaction)
{
  UNDO::ActionCommand::tAction sendEditBuffer([](auto) -> void { EditBuffer::sendToLPC(); });
  transaction->addSimpleCommand(UNDO::ActionCommand::tAction(), sendEditBuffer);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto group : getParameterGroups(vg))
      group->undoableClear(transaction);

  auto swap = UNDO::createSwapData(Uuid::init());
  transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
    swap->swapWith(m_lastLoadedPreset);
    m_signalPresetLoaded.send();
    onChange();
  });

  resetModifiedIndicator(transaction);

  setName(transaction, "Init Sound");
  m_recallSet.copyFromEditBuffer(transaction, this);

  transaction->addSimpleCommand(sendEditBuffer, UNDO::ActionCommand::tAction());
}

void EditBuffer::undoableSetDefaultValues(UNDO::Transaction *transaction, Preset *other)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto &g : getParameterGroups(vg))
      g->undoableSetDefaultValues(transaction, other->findParameterGroup(g->getID(), vg));
}

UNDO::Scope &EditBuffer::getUndoScope()
{
  return ParameterDualGroupSet::getUndoScope();
}

Uuid EditBuffer::getUUIDOfLastLoadedPreset() const
{
  return m_lastLoadedPreset;
}

void EditBuffer::sendToLPC()
{
  Application::get().getLPCProxy()->sendEditBuffer();
  Application::get().getAudioEngineProxy()->sendEditBuffer();
}

void EditBuffer::undoableUnlockAllGroups(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto group : getParameterGroups(vg))
      group->undoableUnlock(transaction);
}

void EditBuffer::undoableLockAllGroups(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto group : getParameterGroups(vg))
      group->undoableLock(transaction);
}

void EditBuffer::undoableToggleGroupLock(UNDO::Transaction *transaction, const Glib::ustring &groupId)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    if(auto g = getParameterGroupByID(groupId, vg))
      g->undoableToggleLock(transaction);
}

Parameter *EditBuffer::searchForAnyParameterWithLock(VoiceGroup vg) const
{
  for(auto group : getParameterGroups(vg))
  {
    for(auto parameter : group->getParameters())
    {
      if(parameter->isLocked())
        return parameter;
    }
  }
  return nullptr;
}

void EditBuffer::setMacroControlValueFromMCView(int id, double value, const Glib::ustring &uuid)
{
  if(auto mcs = getParameterGroupByID("MCs"))
  {
    if(auto mc = dynamic_cast<MacroControlParameter *>(mcs->getParameterByID(id)))
    {
      mc->setCPFromMCView(mc->getUndoScope().startTrashTransaction()->getTransaction(), value);
      mc->setLastMCViewUUID(uuid);
    }
  }
}

VoiceGroup invert(VoiceGroup vg)
{
  return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
}

void EditBuffer::undoableConvertToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  switch(m_type)
  {
    case SoundType::Split:
      undoableConvertSplitToSingle(transaction, copyFrom);
      break;
    case SoundType::Layer:
      undoableConvertLayerToSingle(transaction, copyFrom);
      break;
    case SoundType::Invalid:
    case SoundType::Single:
      break;
  }

  if(m_type == SoundType::Single)
    return;

  undoableSetType(transaction, SoundType::Single);
}

void EditBuffer::undoableConvertSplitToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{

  auto vgmasterGroup = getParameterGroupByID("VGM", copyFrom);
  auto masterGroup = getGlobalParameterGroupByID("Master");
  auto masterVolumeParameter = masterGroup->getParameterByID(247);
  auto masterTuneParameter = masterGroup->getParameterByID(247);

  auto vgVolume = vgmasterGroup->getParameterByID(10002);
  auto vgTune = vgmasterGroup->getParameterByID(10003);

  auto newVolume = masterVolumeParameter->getControlPositionValue() + vgVolume->getControlPositionValue();
  auto newTune = masterTuneParameter->getControlPositionValue() + vgTune->getControlPositionValue();

  masterVolumeParameter->setCPFromHwui(transaction, newVolume);
  masterTuneParameter->setCPFromHwui(transaction, newTune);

  undoableSetType(transaction, SoundType::Single);
}

void EditBuffer::undoableConvertLayerToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  copyVoiceGroup(transaction, copyFrom, invert(copyFrom));

  auto vgmasterGroup = getParameterGroupByID("VGM", copyFrom);
  auto masterGroup = getGlobalParameterGroupByID("Master");
  auto masterVolumeParameter = masterGroup->getParameterByID(247);
  auto masterTuneParameter = masterGroup->getParameterByID(247);

  auto vgVolume = vgmasterGroup->getParameterByID(10002);
  auto vgTune = vgmasterGroup->getParameterByID(10003);

  auto newVolume = masterVolumeParameter->getControlPositionValue() + vgVolume->getControlPositionValue();
  auto newTune = masterTuneParameter->getControlPositionValue() + vgTune->getControlPositionValue();

  masterVolumeParameter->setCPFromHwui(transaction, newVolume);
  masterTuneParameter->setCPFromHwui(transaction, newTune);

  undoableSetType(transaction, SoundType::Single);
}

void EditBuffer::undoableConvertToDual(UNDO::Transaction *transaction, SoundType type, VoiceGroup copyFrom)
{

  if(m_type == type)
    return;

  switch(type)
  {
    case SoundType::Single:
    case SoundType::Invalid:
      break;
    case SoundType::Split:
      undoableConvertToSplit(transaction, copyFrom);
      break;
    case SoundType::Layer:
      undoableConvertToLayer(transaction, copyFrom);
      break;
  }

  undoableSetType(transaction, type);

  if(copyFrom == VoiceGroup::II)
    copyVoiceGroup(transaction, copyFrom, VoiceGroup::I);

  transaction->addSimpleCommand(
      [](auto) { Application::get().getVoiceGroupSelectionHardwareUI()->setHWUIEditBufferSelection(VoiceGroup::I); });
}

void EditBuffer::undoableSetType(UNDO::Transaction *transaction, SoundType type)
{
  transaction->addUndoSwap(this, m_type, type);
  transaction->addSimpleCommand([this](auto) mutable { m_signalTypeChanged.send(); });
}

void EditBuffer::undoableLoadPresetIntoDualSound(Preset *preset, VoiceGroup target)
{
  auto scope = getUndoScope().startTransaction("Load Preset into Voicegroup");
  auto transaction = scope->getTransaction();
  loadIntoVoiceGroup(transaction, preset, target);
}

const SplitPointParameter *EditBuffer::getSplitPoint() const
{
  if(getType() == SoundType::Split)
    return dynamic_cast<const SplitPointParameter *>(findParameterByID(10001, VoiceGroup::Global));
  return nullptr;
}

SplitPointParameter *EditBuffer::getSplitPoint()
{
  if(getType() == SoundType::Split)
    return dynamic_cast<SplitPointParameter *>(findParameterByID(10001, VoiceGroup::Global));
  return nullptr;
}

void EditBuffer::undoableConvertToSplit(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  undoableSetType(transaction, SoundType::Split);

  //Copy Voice Group from one to the other -> real initialize
  copyVoiceGroup(transaction, copyFrom, invert(copyFrom));

  auto globalMaster = getGlobalParameterGroupByID("Master");
  auto vgMasterI = getParameterGroupByID("VGM", VoiceGroup::I);
  auto vgMasterII = getParameterGroupByID("VGM", VoiceGroup::II);

  //Copy Global Master to VG Master
  for(auto &ids : std::vector<std::pair<int, int>>{ { 10002, 247 }, { 10003, 248 } })
  {
    auto mI = vgMasterI->findParameterByID(ids.first);
    auto mII = vgMasterII->findParameterByID(ids.first);
    auto gI = globalMaster->findParameterByID(ids.second);

    mI->copyFrom(transaction, gI);
    mII->copyFrom(transaction, gI);
  }

  //Init Global Master
  for(auto &mp : globalMaster->getParameters())
  {
    mp->setCPFromHwui(transaction, 0.0);
  }

  auto splitParam = getSplitPoint();
  splitParam->setCPFromHwui(transaction, 0.5);
}

void EditBuffer::undoableConvertToLayer(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  undoableSetType(transaction, SoundType::Layer);

  //Copy Voice Group from one to the other -> real initialize
  copyVoiceGroup(transaction, copyFrom, invert(copyFrom));

  auto globalMaster = getGlobalParameterGroupByID("Master");

  auto vgMasterI = getParameterGroupByID("VGM", VoiceGroup::I);
  auto vgMasterII = getParameterGroupByID("VGM", VoiceGroup::II);

  //Copy Global Master to VG Master
  for(auto &ids : std::vector<std::pair<int, int>>{ { 10002, 247 }, { 10003, 248 } })
  {
    auto mI = vgMasterI->findParameterByID(ids.first);
    auto mII = vgMasterII->findParameterByID(ids.first);
    auto gI = globalMaster->findParameterByID(ids.second);

    mI->copyFrom(transaction, gI);
    mII->copyFrom(transaction, gI);
  }

  //Init Global Master
  for(auto &mp : globalMaster->getParameters())
  {
    mp->setCPFromHwui(transaction, 0.0);
  }
}

#ifdef _DEVELOPMENT_PC

void EditBuffer::runTests()
{
  EditBufferTests tester(this);
}

#endif