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
#include <parameters/SplitPointParameter.h>
#include <nltools/Testing.h>
#include <nltools/Types.h>
#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <device-settings/DirectLoadSetting.h>
#include <presets/recall/RecallParameter.h>
#include <parameters/UnisonVoicesParameter.h>
#include <groups/GlobalParameterGroups.h>
#include <groups/MonoGroup.h>
#include <groups/UnisonGroup.h>
#include <presets/PresetParameter.h>
#include <tools/PerformanceTimer.h>
#include <device-settings/Settings.h>
#include <parameters/scale-converters/LinearBipolar48StScaleConverter.h>

EditBuffer::EditBuffer(PresetManager *parent)
    : ParameterDualGroupSet(parent)
    , m_deferredJobs(100, std::bind(&EditBuffer::doDeferedJobs, this))
    , m_isModified(false)
    , m_recallSet(this)
    , m_type(SoundType::Single)
    , m_lastSelectedParameter { 0, VoiceGroup::I }
{
  m_hashOnStore = getHash();
}

EditBuffer::~EditBuffer()
{
  m_voiceGroupConnection.disconnect();
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

  if(auto split = getSplitPoint())
    hash_combine(hash, split->getHash());

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
    auto oldState = m_isModified;

    swap->swapWith<0>(m_isModified);
    swap->swapWith<1>(m_hashOnStore);

    if(oldState != m_isModified)
      m_signalModificationState.send(m_isModified);
  });
}

bool EditBuffer::isModified() const
{
  return m_isModified;
}

sigc::connection EditBuffer::onModificationStateChanged(const sigc::slot<void, bool> &s)
{
  return m_signalModificationState.connectAndInit(s, m_isModified);
}

sigc::connection EditBuffer::onChange(const sigc::slot<void> &s)
{
  return m_signalChange.connectAndInit(s);
}

sigc::connection EditBuffer::onPresetLoaded(const sigc::slot<void> &s)
{
  return m_signalPresetLoaded.connect(s);
}

sigc::connection EditBuffer::onLocksChanged(const sigc::slot<void> &s)
{
  return m_signalLocksChanged.connectAndInit(s);
}

sigc::connection EditBuffer::onRecallValuesChanged(const sigc::slot<void> &s)
{
  return m_recallSet.m_signalRecallValues.connect(s);
}

sigc::connection EditBuffer::onSoundTypeChanged(sigc::slot<void> s)
{
  return m_signalTypeChanged.connectAndInit(s);
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
    Glib::ustring name {};
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

bool EditBuffer::findAnyParameterChanged(VoiceGroup vg) const
{
  for(auto &paramGroup : getParameterGroups(vg))
    for(auto &param : paramGroup->getParameters())
      if(param->isChangedFromLoaded())
        return true;

  if(getType() == SoundType::Split)
    return getSplitPoint()->isChangedFromLoaded();

  return false;
}

bool EditBuffer::findAnyParameterChanged() const
{
  if(m_type == SoundType::Single)
  {
    return findAnyParameterChanged(VoiceGroup::I) || findAnyParameterChanged(VoiceGroup::Global);
  }
  else
  {
    return findAnyParameterChanged(VoiceGroup::I) || findAnyParameterChanged(VoiceGroup::II)
        || findAnyParameterChanged(VoiceGroup::Global);
  }
}

void EditBuffer::resetOriginIf(const Preset *p)
{
  if(m_originCache == p)
    m_originCache = nullptr;
}

bool EditBuffer::isDual() const
{
  return getType() != SoundType::Single;
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
  auto vgINameWithSuffix = getVoiceGroupNameWithSuffix(VoiceGroup::I, true);
  auto vgIIName = getVoiceGroupName(VoiceGroup::II);
  auto vgIINameWithSuffix = getVoiceGroupNameWithSuffix(VoiceGroup::II, true);

  writer.writeTag(
      "edit-buffer",
      { Attribute("selected-parameter", m_lastSelectedParameter), Attribute("editbuffer-type", toString(m_type)),
        Attribute("loaded-preset", getUUIDOfLastLoadedPreset().raw()), Attribute("loaded-presets-name", getName()),
        Attribute("loaded-presets-bank-name", bankName), Attribute("preset-is-zombie", zombie),
        Attribute("is-modified", m_isModified), Attribute("hash", getHash()), Attribute("changed", changed),
        Attribute("vg-I-name", vgIName), Attribute("vg-II-name", vgIIName),
        Attribute("vg-I-name-with-suffix", vgINameWithSuffix), Attribute("vg-II-name-with-suffix", vgIINameWithSuffix),
        Attribute("origin-I", getAttribute("origin-I", "")), Attribute("origin-II", getAttribute("origin-II", "")),
        Attribute("origin-I-vg", getAttribute("origin-I-vg", "")),
        Attribute("origin-II-vg", getAttribute("origin-II-vg", "")) },
      [&]() {
        if(changed)
          super::writeDocument(writer, knownRevision);
        m_recallSet.writeDocument(writer, knownRevision);
      });
}

bool isLoadToPartActive()
{
  auto hwui = Application::get().getHWUI();
  return hwui->getFocusAndMode().detail == UIDetail::LoadToPart;
}

void EditBuffer::undoableLoadSelectedPreset(VoiceGroup loadInto)
{
  if(auto bank = getParent()->getSelectedBank())
  {
    if(auto preset = bank->getSelectedPreset())
    {
      auto isCurrentlyDualSound = getType() != SoundType::Single;
      auto presetToLoadIsDual = preset->getType() != SoundType::Single;

      if(isLoadToPartActive() && isCurrentlyDualSound)
      {
        if(!presetToLoadIsDual)
          undoableLoadPresetIntoDualSound(preset, loadInto);
        else
          nltools::Log::error("dual preset was not loaded!");
      }
      else
      {
        undoableLoad(preset);
      }
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
  PerformanceTimer timer(__PRETTY_FUNCTION__);

  auto ae = Application::get().getAudioEngineProxy();
  ae->toggleSuppressParameterChanges(transaction);

  setAttribute(transaction, "origin-I", preset->getUuid().raw());
  setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::I));
  setAttribute(transaction, "origin-II", preset->getUuid().raw());
  setAttribute(transaction, "origin-II-vg", toString(VoiceGroup::II));

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

  Application::get().getHWUI()->setCurrentVoiceGroupAndUpdateParameterSelection(transaction, VoiceGroup::I);
}

void EditBuffer::copyFrom(UNDO::Transaction *transaction, const Preset *preset)
{
  EditBufferSnapshotMaker::get().addSnapshotIfRequired(transaction, this);

  undoableSetType(transaction, preset->getType());
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
  transaction->addPostfixCommand([this](auto) -> void { this->sendToAudioEngine(); });

  auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &group : getParameterGroups(vg))
      group->undoableRandomize(transaction, initiator, amount);
}

void EditBuffer::undoableRandomizePart(UNDO::Transaction *transaction, VoiceGroup vg, Initiator initiator)
{
  transaction->addPostfixCommand([this](auto) -> void { this->sendToAudioEngine(); });

  auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();

  for(auto &g : getParameterGroups(vg))
    g->undoableRandomize(transaction, initiator, amount);
}

void EditBuffer::undoableInitSound(UNDO::Transaction *transaction)
{
  transaction->addPostfixCommand([this](auto) { this->sendToAudioEngine(); });

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    undoableInitPart(transaction, vg);

  auto swap = UNDO::createSwapData(Uuid::init());
  transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
    swap->swapWith(m_lastLoadedPreset);
    m_signalPresetLoaded.send();
    onChange();
  });

  resetModifiedIndicator(transaction);

  setName(transaction, "Init Sound");
  setAttribute(transaction, "origin-I", "");
  setAttribute(transaction, "origin-II", "");
  setAttribute(transaction, "origin-I-vg", "");
  setAttribute(transaction, "origin-II-vg", "");

  m_recallSet.copyFromEditBuffer(transaction, this);
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

void EditBuffer::combineSplitPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  auto masterGroup = getParameterGroupByID({ "Master", VoiceGroup::Global });

  auto originVolume = dynamic_cast<ModulateableParameter *>(findParameterByID({ 358, copyFrom }));
  auto originTune = dynamic_cast<ModulateableParameter *>(findParameterByID({ 360, copyFrom }));

  auto masterVolumeParameter
      = dynamic_cast<ModulateableParameter *>(masterGroup->getParameterByID({ 247, VoiceGroup::Global }));
  auto masterTuneParameter
      = dynamic_cast<ModulateableParameter *>(masterGroup->getParameterByID({ 248, VoiceGroup::Global }));

  // unmute both parts
  findParameterByID({ 395, VoiceGroup::I })->setCPFromHwui(transaction, 0);
  findParameterByID({ 395, VoiceGroup::II })->setCPFromHwui(transaction, 0);

  ParabolicGainDbScaleConverter dbGainConverter;

  auto vgVolumeDisplay = dbGainConverter.controlPositionToDisplay(originVolume->getControlPositionValue());
  auto masterVolumeDisplay = dbGainConverter.controlPositionToDisplay(masterVolumeParameter->getControlPositionValue());

  auto newVolume = dbGainConverter.displayToControlPosition(vgVolumeDisplay + masterVolumeDisplay);
  auto newTune = originTune->getControlPositionValue() + masterTuneParameter->getControlPositionValue();

  masterVolumeParameter->copyFrom(transaction, originVolume);
  masterVolumeParameter->setCPFromHwui(transaction, newVolume);

  masterTuneParameter->copyFrom(transaction, originTune);
  masterTuneParameter->setCPFromHwui(transaction, newTune);
}

void EditBuffer::combineLayerPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  auto masterGroup = getParameterGroupByID({ "Master", VoiceGroup::Global });

  auto originVolume = dynamic_cast<ModulateableParameter *>(findParameterByID({ 358, copyFrom }));
  auto originTune = dynamic_cast<ModulateableParameter *>(findParameterByID({ 360, copyFrom }));

  auto masterVolumeParameter
      = dynamic_cast<ModulateableParameter *>(masterGroup->getParameterByID({ 247, VoiceGroup::Global }));
  auto masterTuneParameter
      = dynamic_cast<ModulateableParameter *>(masterGroup->getParameterByID({ 248, VoiceGroup::Global }));

  // unmute both parts
  findParameterByID({ 395, VoiceGroup::I })->setCPFromHwui(transaction, 0);
  findParameterByID({ 395, VoiceGroup::II })->setCPFromHwui(transaction, 0);

  ParabolicGainDbScaleConverter dbGainConverter;

  auto vgVolumeDisplay = dbGainConverter.controlPositionToDisplay(originVolume->getControlPositionValue());
  auto masterVolumeDisplay = dbGainConverter.controlPositionToDisplay(masterVolumeParameter->getControlPositionValue());

  auto newVolume = dbGainConverter.displayToControlPosition(vgVolumeDisplay + masterVolumeDisplay);
  auto newTune = originTune->getControlPositionValue() + masterTuneParameter->getControlPositionValue();

  masterVolumeParameter->setCPFromHwui(transaction, newVolume);
  masterVolumeParameter->setModulationSource(transaction, originVolume->getModulationSource());
  masterVolumeParameter->setModulationAmount(transaction, originVolume->getModulationAmount());

  masterTuneParameter->setCPFromHwui(transaction, newTune);
  masterTuneParameter->setModulationSource(transaction, originTune->getModulationSource());
  masterTuneParameter->setModulationAmount(transaction, originTune->getModulationAmount());
}

void EditBuffer::undoableConvertDualToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  const auto oldType = getType();

  setName(transaction, getVoiceGroupName(copyFrom));
  undoableSetType(transaction, SoundType::Single);

  if(oldType == SoundType::Split)
  {
    combineSplitPartGlobalMaster(transaction, copyFrom);
  }
  else if(oldType == SoundType::Layer)
  {
    combineLayerPartGlobalMaster(transaction, copyFrom);
  }

  if(copyFrom != VoiceGroup::I)
    copyVoiceGroup(transaction, copyFrom, VoiceGroup::I);

  initToFX(transaction);
  initFadeFrom(transaction, SoundType::Single);
  initCrossFB(transaction);
  initSplitPoint(transaction);

  forEachParameter(VoiceGroup::II, [&](Parameter *p) { p->loadDefault(transaction); });

  auto vgVolume = findParameterByID({ 358, VoiceGroup::I });
  auto vgTune = findParameterByID({ 360, VoiceGroup::I });
  vgVolume->loadDefault(transaction);
  vgTune->loadDefault(transaction);

  transaction->addUndoSwap(this, m_lastLoadedPreset, Uuid::converted());
  setVoiceGroupName(transaction, "", VoiceGroup::I);
  setVoiceGroupName(transaction, "", VoiceGroup::II);

  initRecallValues(transaction);

  transaction->addPostfixCommand([this](auto) { this->sendToAudioEngine(); });
}

void EditBuffer::undoableConvertToDual(UNDO::Transaction *transaction, SoundType type)
{
  const auto oldType = m_type;

  if(oldType == type)
    return;

  setVoiceGroupName(transaction, getName(), VoiceGroup::I);
  setVoiceGroupName(transaction, getName(), VoiceGroup::II);

  undoableSetType(transaction, type);

  if(oldType == SoundType::Single && type == SoundType::Layer)
    undoableConvertSingleToLayer(transaction);
  else if(oldType == SoundType::Single && type == SoundType::Split)
    undoableConvertSingleToSplit(transaction);
  else if(oldType == SoundType::Layer && type == SoundType::Split)
    undoableConvertLayerToSplit(transaction);
  else if(oldType == SoundType::Split && type == SoundType::Layer)
    undoableConvertSplitToLayer(transaction);

  initCrossFB(transaction);
  initSplitPoint(transaction);
  undoableUnmuteLayers(transaction);

  initRecallValues(transaction);
  transaction->addUndoSwap(this, m_lastLoadedPreset, Uuid::converted());
  transaction->addPostfixCommand([this](auto) { this->sendToAudioEngine(); });
}

void EditBuffer::undoableUnisonMonoLoadDefaults(UNDO::Transaction *transaction, VoiceGroup vg)
{
  getParameterGroupByID({ "Mono", vg })->forEachParameter([&](auto p) -> void { p->loadDefault(transaction); });
  getParameterGroupByID({ "Unison", vg })->forEachParameter([&](auto p) -> void { p->loadDefault(transaction); });
}

void EditBuffer::undoableUnmuteLayers(UNDO::Transaction *transaction)
{
  findParameterByID({ 395, VoiceGroup::I })->setCPFromHwui(transaction, 0);
  findParameterByID({ 395, VoiceGroup::II })->setCPFromHwui(transaction, 0);
}

void EditBuffer::copyAndInitGlobalMasterGroupToPartMasterGroups(UNDO::Transaction *transaction)
{
  auto global = getParameterGroupByID({ "Master", VoiceGroup::Global });
  auto partI = getParameterGroupByID({ "Part", VoiceGroup::I });
  auto partII = getParameterGroupByID({ "Part", VoiceGroup::II });

  //Copy Volume and Tune
  for(auto &ids : std::vector<std::pair<int, int>> { { 358, 247 }, { 360, 248 } })
  {
    auto pI = partI->findParameterByID({ ids.first, VoiceGroup::I });
    auto pII = partII->findParameterByID({ ids.first, VoiceGroup::II });
    auto pGlobal = global->findParameterByID({ ids.second, VoiceGroup::Global });

    pI->copyFrom(transaction, pGlobal);
    pII->copyFrom(transaction, pGlobal);
  }

  for(auto &parameter : global->getParameters())
  {
    parameter->loadDefault(transaction);
  }
}

void EditBuffer::undoableSetType(UNDO::Transaction *transaction, SoundType type)
{
  if(m_type != type)
  {
    auto swap = UNDO::createSwapData(type);

    initUnisonVoicesScaling(transaction, type);

    transaction->addSimpleCommand([=](auto state) {
      swap->swapWith(m_type);
      m_signalTypeChanged.send();
      onChange();
    });
  }
}

void EditBuffer::undoableLoadPresetIntoDualSound(Preset *preset, VoiceGroup vg)
{
  auto scope = getUndoScope().startTransaction("Load Preset into Part " + toString(vg));
  auto transaction = scope->getTransaction();
  undoableLoadPresetIntoDualSound(transaction, preset, vg);
}

void EditBuffer::undoableLoadPresetIntoDualSound(UNDO::Transaction *transaction, Preset *preset, VoiceGroup vg)
{
  auto ae = Application::get().getAudioEngineProxy();
  ae->toggleSuppressParameterChanges(transaction);

  setVoiceGroupName(transaction, preset->getName(), vg);
  loadIntoVoiceGroup(transaction, preset, vg);
  loadPresetGlobalMasterIntoVoiceGroupMaster(transaction, preset, vg);
  initRecallValues(transaction);

  ae->toggleSuppressParameterChanges(transaction);
}

const SplitPointParameter *EditBuffer::getSplitPoint() const
{
  return static_cast<const SplitPointParameter *>(findParameterByID({ 356, VoiceGroup::Global }));
}

SplitPointParameter *EditBuffer::getSplitPoint()
{
  return static_cast<SplitPointParameter *>(findParameterByID({ 356, VoiceGroup::Global }));
}

Glib::ustring EditBuffer::getVoiceGroupName(VoiceGroup vg) const
{
  nltools_assertOnDevPC(vg == VoiceGroup::I || vg == VoiceGroup::II);
  return m_voiceGroupLabels[static_cast<size_t>(vg)];
}

Glib::ustring EditBuffer::getVoiceGroupNameWithSuffix(VoiceGroup vg, bool addSpace) const
{
  auto mono = findParameterByID({ 364, vg })->getControlPositionValue() > 0;
  auto unison = findParameterByID({ 249, vg })->getControlPositionValue() > 0;
  return getVoiceGroupName(vg) + (addSpace ? "\u202F" : "") + (mono ? "\uE040" : "") + (unison ? "\uE041" : "");
}

void EditBuffer::undoableLoadSelectedPresetPartIntoPart(VoiceGroup from, VoiceGroup copyTo)
{
  auto selectedPreset = getParent()->getSelectedPreset();

  if(!selectedPreset)
    return;

  auto transString = UNDO::StringTools::buildString("Load Preset Part", toString(from), "into", toString(copyTo));
  auto scope = getParent()->getUndoScope().startTransaction(transString);
  undoableLoadPresetPartIntoPart(scope->getTransaction(), selectedPreset, from, copyTo);
}

void EditBuffer::undoableLoadPresetPartIntoPart(UNDO::Transaction *transaction, const Preset *preset, VoiceGroup from,
                                                VoiceGroup copyTo)
{
  if(!preset)
    return;

  if(preset->getType() == SoundType::Single)
  {
    from = VoiceGroup::I;
  }

  if(getType() == SoundType::Single)
  {
    copyTo = VoiceGroup::I;
  }

  auto ae = Application::get().getAudioEngineProxy();
  ae->toggleSuppressParameterChanges(transaction);

  setVoiceGroupName(transaction, preset->getName(), copyTo);
  super::copyFrom(transaction, preset, from, copyTo);

  copySumOfMasterGroupToVoiceGroupMasterGroup(transaction, preset, from, copyTo);

  if(getType() == SoundType::Layer)
  {
    getParameterGroupByID({ "Unison", VoiceGroup::II })->undoableLoadDefault(transaction);
    getParameterGroupByID({ "Mono", VoiceGroup::II })->undoableLoadDefault(transaction);
  }

  initFadeParameters(transaction, copyTo);

  initRecallValues(transaction);

  ae->toggleSuppressParameterChanges(transaction);
}

void EditBuffer::initUnisonVoicesScaling(UNDO::Transaction *transaction, SoundType newType)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    if(auto unisonParam = dynamic_cast<UnisonVoicesParameter *>(findParameterByID({ 249, vg })))
      unisonParam->updateScaling(transaction, newType);
}

bool EditBuffer::isDualParameterForSoundType(const Parameter *parameter, SoundType type)
{
  auto selectedIsNotGlobal = parameter->getVoiceGroup() != VoiceGroup::Global;

  if(type == SoundType::Single)
  {
    return false;
  }

  if(type == SoundType::Layer)
  {
    if(UnisonGroup::isUnisonParameter(parameter))
      return false;

    if(MonoGroup::isMonoParameter(parameter))
      return false;

    return GlobalParameterGroups::isSplitPoint(parameter) || selectedIsNotGlobal;
  }

  if(type == SoundType::Split)
  {
    return GlobalParameterGroups::isSplitPoint(parameter) || selectedIsNotGlobal;
  }

  return false;
}

void EditBuffer::undoableInitPart(UNDO::Transaction *transaction, VoiceGroup vg)
{
  transaction->addPostfixCommand([this](auto) { this->sendToAudioEngine(); });

  for(auto &group : getParameterGroups(vg))
    group->undoableClear(transaction);

  if(vg != VoiceGroup::Global)
  {
    setVoiceGroupName(transaction, "Init", vg);
    setAttribute(transaction, vg == VoiceGroup::I ? "origin-I" : "origin-II", "");
    setAttribute(transaction, vg == VoiceGroup::I ? "origin-I-vg" : "origin-II-vg", "");
  }

  m_recallSet.copyFromEditBuffer(transaction, this, vg);
}

void EditBuffer::TEST_doDeferredJobs()
{
  doDeferedJobs();
}

void EditBuffer::initToFX(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    findParameterByID({ 362, vg })->loadDefault(transaction);
}

void EditBuffer::undoableLoadSinglePreset(Preset *preset, VoiceGroup to)
{
  if(preset && preset->getType() == SoundType::Single)
  {
    auto scope = getParent()->getUndoScope().startTransaction(
        nltools::string::concat("Load '", preset->getName(), "' into ", toString(to)));
    undoableLoadPresetPartIntoPart(scope->getTransaction(), preset, VoiceGroup::I, to);
  }
}

void EditBuffer::loadPresetGlobalMasterIntoVoiceGroupMaster(UNDO::Transaction *transaction, Preset *preset,
                                                            VoiceGroup copyTo)
{
  auto part = getParameterGroupByID({ "Part", copyTo });

  for(auto &ids : std::vector<std::pair<int, int>> { { 358, 247 }, { 360, 248 } })
  {
    auto p = part->findParameterByID({ ids.first, part->getVoiceGroup() });
    if(auto pGlobal = preset->findParameterByID({ ids.second, VoiceGroup::Global }, false))
      p->copyFrom(transaction, pGlobal);
  }
}

void EditBuffer::copySumOfMasterGroupToVoiceGroupMasterGroup(UNDO::Transaction *transaction, const Preset *preset,
                                                             VoiceGroup copyFrom, VoiceGroup copyTo)
{
  auto presetGlobalVolume = preset->findParameterByID({ 247, VoiceGroup::Global }, false);
  auto presetGlobalTune = preset->findParameterByID({ 248, VoiceGroup::Global }, false);

  auto presetPartVolume = preset->findParameterByID({ 358, copyFrom }, false);
  auto presetPartTune = preset->findParameterByID({ 360, copyFrom }, false);

  auto partVolume = dynamic_cast<ModulateableParameter *>(findParameterByID({ 358, copyTo }));
  auto partTune = dynamic_cast<ModulateableParameter *>(findParameterByID({ 360, copyTo }));

  if(presetGlobalVolume && presetGlobalTune && partVolume && partTune)
  {
    auto volumeScaleConverter
        = static_cast<const ParabolicGainDbScaleConverter *>(partVolume->getValue().getScaleConverter());
    auto globalVolumeDV = volumeScaleConverter->controlPositionToDisplay(presetGlobalVolume->getValue());
    auto partVolumeDV
        = volumeScaleConverter->controlPositionToDisplay(presetPartVolume ? presetPartVolume->getValue() : .5);

    auto newVolumeDV = globalVolumeDV + partVolumeDV;
    auto newVolumeCP = volumeScaleConverter->displayToControlPosition(newVolumeDV);
    partVolume->setCPFromHwui(transaction, newVolumeCP);
    partTune->setCPFromHwui(transaction,
                            presetGlobalTune->getValue() + (presetPartTune ? presetPartTune->getValue() : 0));

    if(preset->getType() == SoundType::Single)
    {
      partTune->setModulationAmount(transaction, presetGlobalTune->getModulationAmount());
      partTune->setModulationSource(transaction, presetGlobalTune->getModulationSource());

      partVolume->setModulationSource(transaction, presetGlobalVolume->getModulationSource());
      partVolume->setModulationAmount(transaction, presetGlobalVolume->getModulationAmount());
    }
  }
  else if(partTune && partVolume)
  {
    partTune->loadDefault(transaction);
    partVolume->loadDefault(transaction);
  }
}

void EditBuffer::initSplitPoint(UNDO::Transaction *transaction)
{
  auto splitPoint = findParameterByID({ 356, VoiceGroup::Global });
  splitPoint->loadDefault(transaction);
}

void EditBuffer::initFadeFrom(UNDO::Transaction *transaction, SoundType newType)
{
  findParameterByID({ 396, VoiceGroup::I })->loadDefault(transaction);
  findParameterByID({ 397, VoiceGroup::I })->loadDefault(transaction);
  findParameterByID({ 396, VoiceGroup::II })->loadDefault(transaction);
  findParameterByID({ 397, VoiceGroup::II })->loadDefault(transaction);
}

EditBuffer::PartOrigin EditBuffer::getPartOrigin(VoiceGroup vg) const
{
  PartOrigin ret;

  if(vg == VoiceGroup::I)
  {
    ret.presetUUID = getAttribute("origin-I", "");
    ret.sourceGroup = to<VoiceGroup>(getAttribute("origin-I-vg", "Global"));
  }
  else if(vg == VoiceGroup::II)
  {
    ret.presetUUID = getAttribute("origin-II", "");
    ret.sourceGroup = to<VoiceGroup>(getAttribute("origin-II-vg", "Global"));
  }

  return ret;
}

void EditBuffer::calculateFadeParamsFromSplitPoint(UNDO::Transaction *transaction)
{
  findParameterByID({ 396, VoiceGroup::I })->setCPFromHwui(transaction, getSplitPoint()->getControlPositionValue());
  findParameterByID({ 396, VoiceGroup::II })->setCPFromHwui(transaction, getSplitPoint()->getControlPositionValue());
  findParameterByID({ 397, VoiceGroup::I })->loadDefault(transaction);
  findParameterByID({ 397, VoiceGroup::II })->loadDefault(transaction);
}

void EditBuffer::copyVoicesGroups(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to)
{
  for(auto &gid : { "Unison", "Mono" })
  {
    for(auto &target : getParameterGroupByID({ gid, to })->getParameters())
    {
      auto src = findParameterByID({ target->getID().getNumber(), from });
      target->copyFrom(transaction, src);
    }
  }
}

void EditBuffer::initFadeParameters(UNDO::Transaction *transaction, VoiceGroup group)
{
  findParameterByID({ 396, group })->loadDefault(transaction);
  findParameterByID({ 397, group })->loadDefault(transaction);
}

void EditBuffer::initCrossFB(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto paramNum : { 346, 348, 350, 352, 354 })
    {
      findParameterByID({ paramNum, vg })->loadDefault(transaction);
    }
  }
}

void EditBuffer::undoableConvertSingleToSplit(UNDO::Transaction *transaction)
{
  initToFX(transaction);
  copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);
  copyAndInitGlobalMasterGroupToPartMasterGroups(transaction);
  initFadeFrom(transaction, SoundType::Split);
}

void EditBuffer::undoableConvertSingleToLayer(UNDO::Transaction *transaction)
{
  initToFX(transaction);
  copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
  copyAndInitGlobalMasterGroupToPartMasterGroups(transaction);
  initFadeFrom(transaction, SoundType::Layer);
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
}

void EditBuffer::undoableConvertLayerToSplit(UNDO::Transaction *transaction)
{
  copyVoicesGroups(transaction, VoiceGroup::I, VoiceGroup::II);
  initFadeFrom(transaction, SoundType::Split);
}

void EditBuffer::undoableConvertSplitToLayer(UNDO::Transaction *transaction)
{
  auto currentVG = Application::get().getHWUI()->getCurrentVoiceGroup();
  copyVoicesGroups(transaction, currentVG, invert(currentVG));
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
  calculateFadeParamsFromSplitPoint(transaction);
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
}
