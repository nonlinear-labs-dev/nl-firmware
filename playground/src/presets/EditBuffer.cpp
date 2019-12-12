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
#include "proxies/hwui/HWUI.h"
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
  m_voiceGroupConnection.disconnect();
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

Glib::ustring EditBuffer::getName() const
{
  return getEditBufferName();
}

Glib::ustring EditBuffer::getEditBufferName() const
{
  if(auto o = getOrigin())
    return o->getName();

  return m_name;
}

size_t EditBuffer::getHash() const
{
  size_t hash = AttributesOwner::getHash();

  for(auto &vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(const auto g : getParameterGroups(vg))
      hash_combine(hash, g->getHash());

  if(getSplitPoint())
    hash_combine(hash, getSplitPoint()->getHash());

  hash_combine(hash, static_cast<int>(getType()));

  return hash;
}

void EditBuffer::connectToHWUI(HWUI *hwui)
{
  m_voiceGroupConnection = hwui->onCurrentVoiceGroupChanged(sigc::mem_fun(this, &EditBuffer::onVoiceGroupChanged));
}

void EditBuffer::onVoiceGroupChanged(VoiceGroup newVoiceGroup)
{
  auto selected = getSelected();
  auto id = selected->getID();

  if(id.getVoiceGroup() != VoiceGroup::Global)
  {
    undoableSelectParameter({ id.getNumber(), newVoiceGroup });
  }
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

connection EditBuffer::onModificationStateChanged(const sigc::slot<void, bool> &s)
{
  return m_signalModificationState.connectAndInit(s, m_isModified);
}

connection EditBuffer::onChange(const sigc::slot<void> &s)
{
  return m_signalChange.connectAndInit(s);
}

connection EditBuffer::onPresetLoaded(const sigc::slot<void> &s)
{
  return m_signalPresetLoaded.connect(s);
}

connection EditBuffer::onLocksChanged(const sigc::slot<void> &s)
{
  return m_signalLocksChanged.connectAndInit(s);
}

connection EditBuffer::onRecallValuesChanged(const sigc::slot<void> &s)
{
  return m_recallSet.m_signalRecallValues.connect(s);
}

connection EditBuffer::onSoundTypeChanged(sigc::slot<void> s)
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

sigc::connection EditBuffer::onSelectionChanged(const sigc::slot<void, Parameter *, Parameter *> &s)
{
  return m_signalSelectedParameter.connectAndInit(s, nullptr, getSelected());
}

void EditBuffer::undoableSelectParameter(ParameterId id)
{
  if(auto p = findParameterByID(id))
    undoableSelectParameter(p);
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, const ParameterId &id)
{
  if(auto p = findParameterByID(id))
    undoableSelectParameter(transaction, p);
  else
    throw std::runtime_error("could not select parameter: " + id.toString());
}

void EditBuffer::setParameter(ParameterId id, double cpValue)
{
  if(auto p = findParameterByID(id))
  {
    DebugLevel::gassy("EditBuffer::setParameter", id, cpValue);
    Glib::ustring name{};
    if(m_type == SoundType::Single)
      name = UNDO::StringTools::formatString("Set '%0'", p->getGroupAndParameterName());
    else
      name = UNDO::StringTools::formatString("Set '%0' [%1]", p->getGroupAndParameterName(), id.toString());

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
    for(auto &param : paramGroup->getParameters())
      if(param->isChangedFromLoaded())
        return true;

  if(getType() == SoundType::Split)
    return getSplitPoint()->isChangedFromLoaded();

  return false;
}

bool EditBuffer::anyParameterChanged() const
{
  if(m_type == SoundType::Single)
  {
    return anyParameterChanged(VoiceGroup::I) || anyParameterChanged(VoiceGroup::Global);
  }
  else
  {
    return anyParameterChanged(VoiceGroup::I) || anyParameterChanged(VoiceGroup::II)
        || anyParameterChanged(VoiceGroup::Global);
  }
}

void EditBuffer::resetOriginIf(const Preset *p)
{
  if(m_originCache == p)
    m_originCache = nullptr;
}

void EditBuffer::undoableSelectParameter(Parameter *p)
{
  if(p->getID() != m_lastSelectedParameter)
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

bool isInSoundScreen()
{
  auto fum = Application::get().getHWUI()->getFocusAndMode();
  return fum.focus == UIFocus::Sound && fum.mode == UIMode::Select;
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p)
{
  if(m_lastSelectedParameter != p->getID())
  {
    auto swapData = UNDO::createSwapData(p->getID());

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      auto oldSelection = m_lastSelectedParameter;

      swapData->swapWith(m_lastSelectedParameter);

      auto oldP = findParameterByID(oldSelection);
      auto newP = findParameterByID(m_lastSelectedParameter);

      m_signalSelectedParameter.send(oldP, newP);

      if(oldP)
        oldP->onUnselected();

      if(newP)
        newP->onSelected();

      if(!getParent()->isLoading())
      {
        if(auto hwui = Application::get().getHWUI())
        {
          if(hwui->getFocusAndMode().focus == UIFocus::Sound)
          {
            if(oldP->getID() != newP->getID())
              hwui->setFocusAndMode(UIFocus::Parameters);
          }
          else
          {
            hwui->setFocusAndMode(UIFocus::Parameters);
          }
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

Parameter *EditBuffer::getSelected() const
{
  return findParameterByID(m_lastSelectedParameter);
}

void EditBuffer::setName(UNDO::Transaction *transaction, const Glib::ustring &name)
{
  transaction->addUndoSwap(this, m_name, name);
}

void EditBuffer::setVoiceGroupName(UNDO::Transaction *transaction, const Glib::ustring &name, VoiceGroup vg)
{
  nltools_assertOnDevPC(vg == VoiceGroup::I || vg == VoiceGroup::II);
  transaction->addUndoSwap(this, m_voiceGroupLabels[static_cast<size_t>(vg)], name);
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
  auto vgIName = getVoiceGroupName(VoiceGroup::I);
  auto vgIIName = getVoiceGroupName(VoiceGroup::II);

  writer.writeTag(
      "edit-buffer",
      { Attribute("selected-parameter", m_lastSelectedParameter), Attribute("editbuffer-type", toString(m_type)),
        Attribute("loaded-preset", getUUIDOfLastLoadedPreset().raw()), Attribute("loaded-presets-name", getName()),
        Attribute("loaded-presets-bank-name", bankName), Attribute("preset-is-zombie", zombie),
        Attribute("is-modified", m_isModified), Attribute("hash", getHash()), Attribute("changed", changed),
        Attribute("vg-I-name", vgIName), Attribute("vg-II-name", vgIIName) },
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

void EditBuffer::undoableLoad(Preset *preset)
{
  UNDO::Scope::tTransactionScopePtr scope = getUndoScope().startTransaction(preset->buildUndoTransactionTitle("Load"));
  undoableLoad(scope->getTransaction(), preset);
}

void EditBuffer::undoableLoad(UNDO::Transaction *transaction, Preset *preset)
{
  auto ae = Application::get().getAudioEngineProxy();
  ae->toggleSuppressParameterChanges(transaction);

  copyFrom(transaction, preset);
  undoableSetLoadedPresetInfo(transaction, preset);

  if(auto bank = dynamic_cast<Bank *>(preset->getParent()))
  {
    auto pm = static_cast<PresetManager *>(getParent());
    bank->selectPreset(transaction, preset->getUuid());
    pm->selectBank(transaction, bank->getUuid());
  }

  ae->toggleSuppressParameterChanges(transaction);
  resetModifiedIndicator(transaction, getHash());

#warning "Revisit!"
  //Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::I);
}

void EditBuffer::copyFrom(UNDO::Transaction *transaction, const Preset *preset)
{
  EditBufferSnapshotMaker::get().addSnapshotIfRequired(transaction);

  transaction->addUndoSwap(this, m_type, preset->getType());
  super::copyFrom(transaction, preset);
  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::undoableSetLoadedPresetInfo(UNDO::Transaction *transaction, Preset *preset)
{
  Uuid newId = Uuid::none();
  if(preset)
  {
    setName(transaction, preset->getName());
    setVoiceGroupName(transaction, preset->getVoiceGroupName(VoiceGroup::I), VoiceGroup::I);
    setVoiceGroupName(transaction, preset->getVoiceGroupName(VoiceGroup::II), VoiceGroup::II);
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
  UNDO::ActionCommand::tAction sendEditBuffer([](auto) -> void { EditBuffer::sendToAudioEngine(); });
  transaction->addSimpleCommand(UNDO::ActionCommand::tAction(), sendEditBuffer);

  auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &group : getParameterGroups(vg))
      group->undoableRandomize(transaction, initiator, amount);

  transaction->addSimpleCommand(sendEditBuffer, UNDO::ActionCommand::tAction());
}

void EditBuffer::undoableInitSound(UNDO::Transaction *transaction)
{
  UNDO::ActionCommand::tAction sendEditBuffer([](auto) -> void { EditBuffer::sendToAudioEngine(); });
  transaction->addSimpleCommand(UNDO::ActionCommand::tAction(), sendEditBuffer);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &group : getParameterGroups(vg))
      group->undoableClear(transaction);

  auto swap = UNDO::createSwapData(Uuid::init());
  transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
    swap->swapWith(m_lastLoadedPreset);
    m_signalPresetLoaded.send();
    onChange();
  });

  resetModifiedIndicator(transaction);

  setVoiceGroupName(transaction, "\uE071", VoiceGroup::I);
  setVoiceGroupName(transaction, "\uE072", VoiceGroup::II);
  setName(transaction, "Init Sound");

  m_recallSet.copyFromEditBuffer(transaction, this);

  transaction->addSimpleCommand(sendEditBuffer, UNDO::ActionCommand::tAction());
}

void EditBuffer::undoableSetDefaultValues(UNDO::Transaction *transaction, Preset *other)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto &g : getParameterGroups(vg))
      if(auto otherGroup = other->findParameterGroup(g->getID()))
        g->undoableSetDefaultValues(transaction, otherGroup);
}

UNDO::Scope &EditBuffer::getUndoScope()
{
  return ParameterDualGroupSet::getUndoScope();
}

Uuid EditBuffer::getUUIDOfLastLoadedPreset() const
{
  return m_lastLoadedPreset;
}

void EditBuffer::sendToAudioEngine()
{
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
    if(auto g = getParameterGroupByID({ groupId, vg }))
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

void EditBuffer::setMacroControlValueFromMCView(ParameterId id, double value, const Glib::ustring &uuid)
{
  if(auto mc = dynamic_cast<MacroControlParameter *>(findParameterByID(id)))
  {
    mc->setCPFromMCView(mc->getUndoScope().startTrashTransaction()->getTransaction(), value);
    mc->setLastMCViewUUID(uuid);
  }
}

VoiceGroup invert(VoiceGroup vg)
{
  return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
}

void EditBuffer::undoableConvertToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  if(getType() != SoundType::Single)
    undoableConvertDualToSingle(transaction, copyFrom);
}

void EditBuffer::undoableConvertDualToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  setName(transaction, getVoiceGroupName(copyFrom));

  auto masterGroup = getParameterGroupByID({ "Master", VoiceGroup::Global });

  auto originVolume = findParameterByID({ 358, copyFrom });
  auto originTune = findParameterByID({ 360, copyFrom });

  auto masterVolumeParameter = masterGroup->getParameterByID({ 247, VoiceGroup::Global });
  auto masterTuneParameter = masterGroup->getParameterByID({ 248, VoiceGroup::Global });

  auto newVolume = originVolume->getControlPositionValue() + masterVolumeParameter->getControlPositionValue();
  auto newTune = originTune->getControlPositionValue() + masterTuneParameter->getControlPositionValue();

  masterVolumeParameter->setCPFromHwui(transaction, newVolume);
  masterTuneParameter->setCPFromHwui(transaction, newTune);

  for(auto v : { VoiceGroup::I, VoiceGroup::II })
  {
    auto vgVolume = findParameterByID({ 358, v });
    auto vgTune = findParameterByID({ 360, v });

    vgVolume->setDefaultFromHwui(transaction);
    vgTune->setDefaultFromHwui(transaction);
  }

  if(copyFrom != VoiceGroup::I)
    copyVoiceGroup(transaction, copyFrom, VoiceGroup::I);

  transaction->addUndoSwap(this, m_lastLoadedPreset, Uuid::converted());
  setVoiceGroupName(transaction, "", VoiceGroup::I);
  setVoiceGroupName(transaction, "", VoiceGroup::II);

  initRecallValues(transaction);
  undoableSetType(transaction, SoundType::Single);

  transaction->addPostfixCommand([this](auto state) { sendToAudioEngine(); });
}

void EditBuffer::undoableConvertToDual(UNDO::Transaction *transaction, SoundType type)
{
  if(m_type == type)
    return;

  switch(type)
  {
    case SoundType::Split:
      undoableConvertToSplit(transaction);
      break;
    case SoundType::Layer:
      undoableConvertToLayer(transaction);
      break;
    case SoundType::Single:
    case SoundType::Invalid:
      break;
  }

  setVoiceGroupName(transaction, getName(), VoiceGroup::I);
  setVoiceGroupName(transaction, getName(), VoiceGroup::II);

  undoableSetType(transaction, type);
  copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);
  initRecallValues(transaction);

  transaction->addUndoSwap(this, m_lastLoadedPreset, Uuid::converted());

  transaction->addPostfixCommand([this](auto state) { sendToAudioEngine(); });
}

void EditBuffer::undoableSetType(UNDO::Transaction *transaction, SoundType type)
{
  auto swap = UNDO::createSwapData(std::move(type));

  transaction->addSimpleCommand([=](auto) {
    swap->swapWith(m_type);
    m_signalTypeChanged.send();
    onChange();
  });
}

void EditBuffer::undoableLoadPresetIntoDualSound(Preset *preset, VoiceGroup vg)
{
  auto scope = getUndoScope().startTransaction("Load Preset into Voicegroup");
  auto transaction = scope->getTransaction();
  undoableLoadPresetIntoDualSound(transaction, preset, vg);
}

void EditBuffer::undoableLoadPresetIntoDualSound(UNDO::Transaction *transaction, Preset *preset, VoiceGroup vg)
{
  setVoiceGroupName(transaction, preset->getName(), vg);
  loadIntoVoiceGroup(transaction, preset, vg);
  initRecallValues(transaction);
}

const SplitPointParameter *EditBuffer::getSplitPoint() const
{
  if(getType() == SoundType::Split)
    return dynamic_cast<const SplitPointParameter *>(findParameterByID({ 356, VoiceGroup::Global }));
  return nullptr;
}

SplitPointParameter *EditBuffer::getSplitPoint()
{
  if(getType() == SoundType::Split)
    return dynamic_cast<SplitPointParameter *>(findParameterByID({ 356, VoiceGroup::Global }));
  return nullptr;
}

void EditBuffer::undoableConvertToSplit(UNDO::Transaction *transaction)
{
  undoableSetType(transaction, SoundType::Split);

  //Copy Voice Group from one to the other -> real initialize
  copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);

  auto globalMaster = getParameterGroupByID({ "Master", VoiceGroup::Global });
  auto vgMasterI = getParameterGroupByID({ "PART", VoiceGroup::I });
  auto vgMasterII = getParameterGroupByID({ "PART", VoiceGroup::II });

  //Copy Global Master to VG Master
  for(auto &ids : std::vector<std::pair<int, int>>{ { 358, 247 }, { 360, 248 } })
  {
    auto mI = vgMasterI->findParameterByID({ ids.first, VoiceGroup::I });
    auto mII = vgMasterII->findParameterByID({ ids.first, VoiceGroup::II });
    auto gI = globalMaster->findParameterByID({ ids.second, VoiceGroup::Global });

    mI->copyFrom(transaction, gI);
    mII->copyFrom(transaction, gI);
  }

  //Init Global Master
  for(auto &mp : globalMaster->getParameters())
  {
    mp->setDefaultFromHwui(transaction);
  }

  auto splitParam = getSplitPoint();
  splitParam->setCPFromHwui(transaction, 0.5);
}

void EditBuffer::undoableConvertToLayer(UNDO::Transaction *transaction)
{
  undoableSetType(transaction, SoundType::Layer);

  //Copy Voice Group from one to the other -> real initialize
  copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);

  auto globalMaster = getParameterGroupByID({ "Master", VoiceGroup::Global });
  auto vgMasterI = getParameterGroupByID({ "PART", VoiceGroup::I });
  auto vgMasterII = getParameterGroupByID({ "PART", VoiceGroup::II });

  //Copy Global Master to VG Master
  for(auto &ids : std::vector<std::pair<int, int>>{ { 358, 247 }, { 360, 248 } })
  {
    auto mI = vgMasterI->findParameterByID({ ids.first, VoiceGroup::I });
    auto mII = vgMasterII->findParameterByID({ ids.first, VoiceGroup::II });
    auto gI = globalMaster->findParameterByID({ ids.second, VoiceGroup::Global });

    mI->copyFrom(transaction, gI);
    mII->copyFrom(transaction, gI);
  }

  //Init Global Master
  for(auto &mp : globalMaster->getParameters())
  {
    mp->setDefaultFromHwui(transaction);
  }
}

Glib::ustring EditBuffer::getVoiceGroupName(VoiceGroup vg) const
{
  nltools_assertOnDevPC(vg == VoiceGroup::I || vg == VoiceGroup::II);
  return m_voiceGroupLabels[static_cast<size_t>(vg)];
}
