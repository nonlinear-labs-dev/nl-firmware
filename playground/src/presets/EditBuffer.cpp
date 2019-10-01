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
#include <proxies/hwui/HWUI.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include "parameters/ModulateableParameter.h"
#include <parameters/PhysicalControlParameter.h>
#include <device-settings/DeviceName.h>
#include "device-settings/RandomizeAmount.h"
#include "device-info/DeviceInformation.h"
#include "parameters/MacroControlParameter.h"
#include <libundo/undo/Transaction.h>
#include <nltools/StringTools.h>

EditBuffer::EditBuffer(PresetManager *parent)
    : ParameterDualGroupSet(parent)
    , m_deferredJobs(100, std::bind(&EditBuffer::doDeferedJobs, this))
    , m_isModified(false)
    , m_recallSet(this)
    , m_type(SoundType::Single)
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

  hash_combine(hash, getType());

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

connection EditBuffer::onModificationStateChanged(slot<void, bool> s)
{
  return m_signalModificationState.connectAndInit(s, m_isModified);
}

connection EditBuffer::onChange(slot<void> s)
{
  return m_signalChange.connectAndInit(s);
}

connection EditBuffer::onPresetLoaded(slot<void> s)
{
  return m_signalPresetLoaded.connect(s);
}

connection EditBuffer::onLocksChanged(slot<void> s)
{
  return m_signalLocksChanged.connectAndInit(s);
}

connection EditBuffer::onRecallValuesChanged(slot<void> s)
{
  return m_recallSet.m_signalRecallValues.connect(s);
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

void EditBuffer::undoableSelectParameter(const Glib::ustring &id, VoiceGroup voiceGroup)
{
  sanitizeVoiceGroup(voiceGroup);

  if(auto p = findParameterByID(std::stoi(id), voiceGroup))
    undoableSelectParameter(p);
}

void EditBuffer::undoableSelectParameter(uint16_t id, VoiceGroup voiceGroup)
{
  sanitizeVoiceGroup(voiceGroup);

  if(auto p = findParameterByID(id, voiceGroup))
    undoableSelectParameter(p);
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, const Glib::ustring &id, VoiceGroup vg)
{
  sanitizeVoiceGroup(vg);

  if(auto p = findParameterByID(std::stoi(id), vg))
    undoableSelectParameter(transaction, p);
}

void EditBuffer::setParameter(size_t id, double cpValue, VoiceGroup vg)
{
  sanitizeVoiceGroup(vg);

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

void EditBuffer::setModulationSource(MacroControls src, VoiceGroup vg)
{
  sanitizeVoiceGroup(vg);

  auto index = static_cast<int>(vg);
  if(auto p = dynamic_cast<ModulateableParameter *>(getSelected(vg)))
  {
    auto scope = getUndoScope().startTransaction("Set MC Select for '%0'", p->getLongName());
    p->undoableSelectModSource(scope->getTransaction(), src);
  }
}

void EditBuffer::setModulationAmount(double amount, VoiceGroup vg)
{
  sanitizeVoiceGroup(vg);
  auto index = static_cast<int>(vg);
  if(auto p = dynamic_cast<ModulateableParameter *>(getSelected(vg)))
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

#warning "TODO Editbuffer API + VoiceGroup"

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

void EditBuffer::undoableSelectParameter(Parameter *p, VoiceGroup voiceGroup)
{
  sanitizeVoiceGroup(voiceGroup);
  auto vgIndex = static_cast<int>(voiceGroup);

  if(p->getID() != m_selectedParameterId)
  {
    auto newSelection = getSelected(voiceGroup);
    auto scope = getUndoScope().startContinuousTransaction(&newSelection, std::chrono::hours(1), "Select '%0'",
                                                           p->getGroupAndParameterName());
    undoableSelectParameter(scope->getTransaction(), p, voiceGroup);
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

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p, VoiceGroup voiceGroup)
{
  sanitizeVoiceGroup(voiceGroup);
  auto vgIndex = static_cast<int>(voiceGroup);

  if(m_selectedParameterId != p->getID())
  {
    auto swapData = UNDO::createSwapData(p->getID());

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      auto oldSelectedParamID = m_selectedParameterId;

      swapData->swapWith(m_selectedParameterId);

      auto oldP = findParameterByID(oldSelectedParamID, voiceGroup);
      auto newP = findParameterByID(m_selectedParameterId, voiceGroup);

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

Parameter *EditBuffer::getSelected(VoiceGroup vg) const
{
  sanitizeVoiceGroup(vg);
  return findParameterByID(m_selectedParameterId, vg);
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
      { Attribute("selected-parameter", m_selectedParameterId), Attribute("editbuffer-type", toString(m_type)),
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
  undoableConvertToType(transaction, preset->getType());
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
  UNDO::ActionCommand::tAction sendEditBuffer(std::bind(&EditBuffer::sendToLPC, this));
  transaction->addSimpleCommand(UNDO::ActionCommand::tAction(), sendEditBuffer);

  auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto group : getParameterGroups(vg))
      group->undoableRandomize(transaction, initiator, amount);

  transaction->addSimpleCommand(sendEditBuffer, UNDO::ActionCommand::tAction());
}

void EditBuffer::undoableInitSound(UNDO::Transaction *transaction)
{
  UNDO::ActionCommand::tAction sendEditBuffer(std::bind(&EditBuffer::sendToLPC, this));
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
#warning "What to do with MCView?"
  if(auto mcs = getParameterGroupByID("MCs"))
  {
    if(auto mc = dynamic_cast<MacroControlParameter *>(mcs->getParameterByID(id)))
    {
      mc->setCPFromMCView(mc->getUndoScope().startTrashTransaction()->getTransaction(), value);
      mc->setLastMCViewUUID(uuid);
    }
  }
}

void EditBuffer::undoableConvertToType(UNDO::Transaction *transaction, const SoundType &ebType)
{
  transaction->addUndoSwap(this, m_type, ebType);
}

void EditBuffer::undoableConvertToType(const SoundType &ebType)
{
  if(ebType == m_type)
    return;

  auto scope = getUndoScope().startTransaction("Convert Editbuffer to " + toString(ebType));
  auto transaction = scope->getTransaction();
  undoableConvertToType(transaction, ebType);
}

void EditBuffer::sanitizeVoiceGroup(VoiceGroup &vg)
{
  if(vg == VoiceGroup::Invalid)
    vg = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
}

VoiceGroup EditBuffer::findVoiceGroupWithParameter(const Parameter *pParameter)
{
  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
  {
    auto groups = getParameterGroups(vg);
    for(auto &group : groups)
    {
      for(auto &param : group->getParameters())
      {
        if(param == pParameter)
          return vg;
      }
    }
  }
  return VoiceGroup::Invalid;
}
