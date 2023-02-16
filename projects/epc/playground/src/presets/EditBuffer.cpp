#include "EditBuffer.h"
#include "parameters/Parameter.h"
#include "xml/Writer.h"
#include <http/ContentManager.h>
#include "Application.h"
#include <presets/Preset.h>
#include <presets/Bank.h>
#include "presets/PresetManager.h"
#include "EditBufferSnapshotMaker.h"
#include "proxies/playcontroller/PlaycontrollerProxy.h"
#include "proxies/hwui/HWUI.h"
#include <proxies/audio-engine/AudioEngineProxy.h>
#include "parameters/ModulateableParameter.h"
#include "device-info/DeviceInformation.h"
#include "parameters/MacroControlParameter.h"
#include <libundo/undo/Transaction.h>
#include <parameters/SplitPointParameter.h>
#include <nltools/Testing.h>
#include <nltools/Types.h>
#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include <presets/recall/RecallParameter.h>
#include <parameters/UnisonVoicesParameter.h>
#include <presets/PresetParameter.h>
#include <tools/PerformanceTimer.h>
#include <device-settings/Settings.h>
#include <parameters/ScopedLock.h>
#include <tools/StringTools.h>
#include <parameter_declarations.h>
#include <presets/SendEditBufferScopeGuard.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <device-settings/SyncSplitSettingUseCases.h>
#include <libundo/undo/ContinuousTransaction.h>
#include "LoadedPresetLog.h"
#include "parameters/ScopedLockByParameterTypes.h"
#include "use-cases/SettingsUseCases.h"
#include "parameters/ParameterFactory.h"
#include <sync/JsonAdlSerializers.h>
#include <use-cases/EditBufferUseCases.h>
#include <groups/MacroControlsGroup.h>
#include <presets/EditBufferConverter.h>
#include <presets/EditBufferToPartLoader.h>

EditBuffer::EditBuffer(PresetManager *parent, Settings &settings, std::unique_ptr<AudioEngineProxy> &aeContainer)
    : ParameterGroupSet(parent)
    , SyncedItem(parent->getRoot()->getSyncMaster(), "/editbuffer")
    , m_parameterDB(*this)
    , m_deferredJobs(100, [this]() { doDeferedJobs(); })
    , m_isModified(false)
    , m_recallSet(this)
    , m_type(SoundType::Single)
    , m_lastSelectedParameter { 0, VoiceGroup::I }
    , m_loadedPresetLog(std::make_unique<LoadedPresetLog>())
    , m_settings { settings }
    , m_audioEngineProxyContainer { aeContainer }
    , m_lastSelectedMacroControl { C15::PID::MC_A, VoiceGroup::Global }
{
  m_hashOnStore = getHash();
}

void EditBuffer::init(Settings *settings)
{
  ParameterGroupSet::init(settings);
  m_recallSet.init();
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

sigc::connection EditBuffer::onChange(const sigc::slot<void> &s, bool init)
{
  if(init)
    return m_signalChange.connectAndInit(s);
  else
    return m_signalChange.connect(s);
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

sigc::connection EditBuffer::onSoundTypeChanged(const sigc::slot<void, SoundType> &s)
{
  return m_signalTypeChanged.connectAndInit(s, m_type);
}

sigc::connection EditBuffer::onSoundTypeChanged(const sigc::slot<void, SoundType> &s, bool init)
{
  if(init)
    return m_signalTypeChanged.connectAndInit(s, m_type);
  else
    return m_signalTypeChanged.connect(s);
}

sigc::connection EditBuffer::onEditBufferConverted(const sigc::slot<void, SoundType> &s)
{
  return m_signalConversionHappened.connect(s);
}

UpdateDocumentContributor::tUpdateID EditBuffer::onChange(uint64_t flags)
{
  m_deferredJobs.trigger();
  m_signalChange.send();

  if(flags & ChangeFlags::LockState)
  {
    m_signalLocksChanged.deferedSend();
  }

  SyncedItem::setDirty();
  return ParameterGroupSet::onChange(flags);
}

void EditBuffer::doDeferedJobs()
{
  checkModified();
  m_loadedPresetLog->update(m_presetOriginDescription);
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
  return dynamic_cast<PresetManager *>(super::getParent());
}

const PresetManager *EditBuffer::getParent() const
{
  return dynamic_cast<const PresetManager *>(super::getParent());
}

sigc::connection EditBuffer::onSelectionChanged(const sigc::slot<void, Parameter *, Parameter *> &s,
                                                std::optional<VoiceGroup> initialVG)
{
  if(initialVG.has_value())
  {
    return m_signalSelectedParameter.connectAndInit(s, nullptr, getSelected(initialVG.value()));
  }
  else
  {
    return m_signalSelectedParameter.connect(s);
  }
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, const ParameterId &id,
                                         bool sendReselectionSignal)
{
  if(auto p = findParameterByID(id))
    undoableSelectParameter(transaction, p, sendReselectionSignal);
  else
    throw std::runtime_error("could not select parameter: " + id.toString());
}

bool EditBuffer::hasLocks() const
{
  return hasLocks(VoiceGroup::I) || hasLocks(VoiceGroup::II) || hasLocks(VoiceGroup::Global);
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

  return false;
}

bool EditBuffer::findAnyParameterChanged() const
{
  if(m_type == SoundType::Single)
  {
    return findAnyParameterChanged(VoiceGroup::I) || findAnyParameterChanged(VoiceGroup::II)
        || findAnyParameterChanged(VoiceGroup::Global);
  }
  else
  {
    auto labelChanged = isPartLabelChanged(VoiceGroup::I) || isPartLabelChanged(VoiceGroup::II);

    return labelChanged || findAnyParameterChanged(VoiceGroup::I) || findAnyParameterChanged(VoiceGroup::II)
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

namespace nlohmann
{
  template <> struct adl_serializer<ParameterGroup *>
  {
    static void to_json(json &j, const ParameterGroup *v)
    {
      j = v->getID();
    }
  };
}

nlohmann::json EditBuffer::serialize() const
{
  auto pm = static_cast<const PresetManager *>(getParent());
  if(pm->isLoading())
    return {};

  auto origin = pm->findPreset(getUUIDOfLastLoadedPreset());
  auto zombie = isZombie();
  auto bank = origin ? dynamic_cast<const Bank *>(origin->getParent()) : nullptr;
  auto bankName = bank ? bank->getName(true) : "";
  auto vgIName = getVoiceGroupName(VoiceGroup::I);
  auto vgINameWithSuffix = getVoiceGroupNameWithSuffix(VoiceGroup::I, true);
  auto vgIIName = getVoiceGroupName(VoiceGroup::II);
  auto vgIINameWithSuffix = getVoiceGroupNameWithSuffix(VoiceGroup::II, true);

  return { { "selected-parameter", m_lastSelectedParameter },
           { "editbuffer-type", toString(m_type) },
           { "loaded-preset", getUUIDOfLastLoadedPreset() },
           { "loaded-presets-name", getName() },
           { "loaded-presets-bank-name", bankName },
           { "preset-is-zombie", zombie },
           { "is-modified", m_isModified },
           { "hash", getHash() },
           { "vg-I-name", vgIName },
           { "vg-II-name", vgIIName },
           { "vg-I-name-with-suffix", vgINameWithSuffix },
           { "vg-II-name-with-suffix", vgIINameWithSuffix },
           { "origin-I", getAttribute("origin-I", "") },
           { "origin-II", getAttribute("origin-II", "") },
           { "origin-I-vg", getAttribute("origin-I-vg", "") },
           { "origin-II-vg", getAttribute("origin-II-vg", "") },
           { "parametergroups", getParameters() },
           { "attributes", AttributesOwner::toJson() } };
}

void EditBuffer::undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p, bool sendReselectionSignal)
{
  if(m_lastSelectedParameter != p->getID())
  {
    auto swapData = UNDO::createSwapData(p->getID());
    ParameterId newMCID
        = MacroControlsGroup::isMacroControl(p->getID().getNumber()) ? p->getID() : m_lastSelectedMacroControl;
    auto incomingMCSwapData = UNDO::createSwapData(newMCID);

    p->resetWasDefaulted(transaction);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      auto oldSelection = m_lastSelectedParameter;

      swapData->swapWith(m_lastSelectedParameter);
      incomingMCSwapData->swapWith(m_lastSelectedMacroControl);

      auto oldP = findParameterByID(oldSelection);
      auto newP = findParameterByID(m_lastSelectedParameter);

      m_signalSelectedParameter.send(oldP, newP);

      if(oldP)
        oldP->onUnselected();

      if(newP)
        newP->onSelected();

      onChange();
    });
  }
  else if(sendReselectionSignal)
  {
    transaction->addSimpleCommand([=](auto) mutable { m_signalReselectParameter.send(p); });
  }
}

Parameter *EditBuffer::getSelected(VoiceGroup voiceGroup) const
{
  if(getType() == SoundType::Single && (voiceGroup == VoiceGroup::II))
  {
    if(auto p = findParameterByID(m_lastSelectedParameter))
    {
      if(p->isPolyphonic())
      {
        voiceGroup = VoiceGroup::I;
      }
    }
  }

  if(ParameterId::isGlobal(m_lastSelectedParameter.getNumber()))
    voiceGroup = VoiceGroup::Global;
  else if(voiceGroup == VoiceGroup::Global)
    voiceGroup = VoiceGroup::I;

  if(getType() == SoundType::Layer)
  {
    static std::vector<C15::PID::ParameterID> layerPTIOnly { C15::PID::Unison_Voices,   C15::PID::Unison_Detune,
                                                             C15::PID::Unison_Pan,      C15::PID::Unison_Phase,
                                                             C15::PID::Mono_Grp_Enable, C15::PID::Mono_Grp_Glide,
                                                             C15::PID::Mono_Grp_Legato, C15::PID::Mono_Grp_Prio };

    if(std::find(layerPTIOnly.begin(), layerPTIOnly.end(), m_lastSelectedParameter.getNumber()) != layerPTIOnly.end())
    {
      voiceGroup = VoiceGroup::I;
    }
  }

  return findParameterByID({ m_lastSelectedParameter.getNumber(), voiceGroup });
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
  auto changed = knownRevision < ParameterGroupSet::getUpdateIDOfLastChange();
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

void EditBuffer::undoableLoad(UNDO::Transaction *transaction, const Preset *preset, bool sendToAudioEngine)
{
  if(auto p = dynamic_cast<UNDO::ContinuousTransaction *>(transaction))
    p->stopContinuation();  // if transaction was created for a select operation, direct-load has to stop replacing the transaction

  auto parameterFocusLock = getParameterFocusLockGuard();

  PerformanceTimer timer(__PRETTY_FUNCTION__);
  SendEditBufferScopeGuard scope(transaction, sendToAudioEngine);

  const auto oldType = getType();

  setAttribute(transaction, "origin-I", preset->getUuid().raw());
  setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::I));
  setAttribute(transaction, "origin-II", preset->getUuid().raw());
  setAttribute(transaction, "origin-II-vg", toString(VoiceGroup::II));

  copyFrom(transaction, preset);
  undoableSetLoadedPresetInfo(transaction, preset, false);

  if(auto bank = dynamic_cast<Bank *>(preset->getParent()))
  {
    auto pm = static_cast<PresetManager *>(getParent());
    bank->selectPreset(transaction, preset->getUuid());
    pm->selectBank(transaction, bank->getUuid());
  }

  setSyncSplitSettingAccordingToLoadedPreset(transaction);
  cleanupParameterSelectionOnSoundTypeChange(transaction, oldType, preset->getType());
  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::undoableLoadToPart(UNDO::Transaction *trans, const Preset *p, VoiceGroup from, VoiceGroup to)
{
  EditBufferToPartLoader loader(*this);
  loader.undoableLoadPresetPartIntoPart(trans, p, from, to);
}

void EditBuffer::copyFrom(UNDO::Transaction *transaction, const Preset *preset)
{
  EditBufferSnapshotMaker::get().addSnapshotIfRequired(transaction, this);
  undoableSetType(transaction, preset->getType());
  super::copyFrom(transaction, preset);
  setHWSourcesToLoadRulePositionsAndModulate(transaction);
  initRecallValues(transaction);
  resetModifiedIndicator(transaction, getHash());
}

void EditBuffer::undoableSetLoadedPresetInfo(UNDO::Transaction *transaction, const Preset *preset, bool resetRecall)
{
  Uuid newId = Uuid::none();
  std::string presetOriginDescription = "Init";

  if(preset)
  {
    setName(transaction, preset->getName());
    setVoiceGroupName(transaction, preset->getVoiceGroupName(VoiceGroup::I), VoiceGroup::I);
    setVoiceGroupName(transaction, preset->getVoiceGroupName(VoiceGroup::II), VoiceGroup::II);
    newId = preset->getUuid();

    if(auto d = preset->getOriginDescription(); d.has_value() && !d.value().empty())
    {
      presetOriginDescription = d.value();
    }
    else
    {
      if(auto bank = dynamic_cast<Bank *>(preset->getParent()))
      {
        auto pPos = bank->getPresetPosition(preset);
        auto bPos = getParent()->getBankPosition(bank->getUuid());
        auto bName = bank->getName(true);
        presetOriginDescription
            = StringTools::buildString(bPos + 1, ": ", bName, " / ", pPos + 1, ": ", preset->getName());
      }
      else
      {
        presetOriginDescription = preset->getName();
      }
    }
  }

  resetModifiedIndicator(transaction);

  auto swap = UNDO::createSwapData(std::move(newId), std::move(presetOriginDescription));

  transaction->addSimpleCommand([=](auto) {
    swap->swapWith<0>(m_lastLoadedPreset);
    swap->swapWith<1>(m_presetOriginDescription);
    sendPresetLoadSignal();
    onChange();
  });

  if(resetRecall)
    initRecallValues(transaction);
}

void EditBuffer::undoableUpdateLoadedPresetInfo(UNDO::Transaction *transaction)
{
  auto pm = static_cast<PresetManager *>(getParent());
  undoableSetLoadedPresetInfo(transaction, pm->findPreset(getUUIDOfLastLoadedPreset()));
}

void EditBuffer::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &group : getParameterGroups(vg))
      group->undoableRandomize(transaction, initiator, amount);
}

void EditBuffer::undoableRandomizePart(UNDO::Transaction *transaction, VoiceGroup currentVoiceGroup,
                                       Initiator initiator, double amount)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);

  for(auto &g : getParameterGroups(currentVoiceGroup))
    g->undoableRandomize(transaction, initiator, amount);
}

void EditBuffer::undoableInitSound(UNDO::Transaction *transaction, Defaults mode)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    undoableInitPart(transaction, vg, mode);

  auto swap = UNDO::createSwapData(Uuid::init());
  transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
    swap->swapWith(m_lastLoadedPreset);
    sendPresetLoadSignal();
    onChange();
  });

  resetModifiedIndicator(transaction);

  setName(transaction, "Init Sound");
  setAttribute(transaction, "origin-I", "");
  setAttribute(transaction, "origin-II", "");
  setAttribute(transaction, "origin-I-vg", "");
  setAttribute(transaction, "origin-II-vg", "");

  setAttribute(transaction, "color", "");
  setAttribute(transaction, "Comment", "");

  m_recallSet.copyFromEditBuffer(transaction, this);
}

void EditBuffer::undoableSetDefaultValues(UNDO::Transaction *transaction, Preset *other)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &g : getParameterGroups(vg))
      if(auto otherGroup = other->findParameterGroup(g->getID()))
        g->undoableSetDefaultValues(transaction, otherGroup);
}

UNDO::Scope &EditBuffer::getUndoScope()
{
  return ParameterGroupSet::getUndoScope();
}

Uuid EditBuffer::getUUIDOfLastLoadedPreset() const
{
  return m_lastLoadedPreset;
}

void EditBuffer::undoableUnlockAllGroups(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto group : getParameterGroups(vg))
      group->undoableUnlock(transaction);
}

void EditBuffer::undoableLockAllGroups(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto group : getParameterGroups(vg))
      group->undoableLock(transaction);
}

void EditBuffer::undoableToggleGroupLock(UNDO::Transaction *transaction, const Glib::ustring &groupId)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
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

void EditBuffer::setMacroControlValueFromMCView(const ParameterId &id, double value, const Glib::ustring &uuid)
{
  if(auto mc = dynamic_cast<MacroControlParameter *>(findParameterByID(id)))
  {
    auto scope = UNDO::Scope::startTrashTransaction();
    mc->setCPFromMCView(scope->getTransaction(), value);
    mc->setLastMCViewUUID(uuid);
  }
}

namespace
{
  VoiceGroup invert(VoiceGroup vg)
  {
    return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
  }
}

void EditBuffer::undoableConvertToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  if(getType() != SoundType::Single)
  {
    EditBufferConverter converter(*this);
    converter.undoableConvertDualToSingle(transaction, copyFrom);
  }
}

void EditBuffer::undoableConvertToDual(UNDO::Transaction *transaction, SoundType type, VoiceGroup currentVG)
{
  EditBufferConverter converter(*this);
  converter.convertToDual(transaction, type, currentVG);
}

void EditBuffer::undoableSetType(UNDO::Transaction *transaction, SoundType type)
{
  if(m_type != type)
  {
    auto swap = UNDO::createSwapData(type);

    cleanupParameterSelectionOnSoundTypeChange(transaction, m_type, type);

    transaction->addSimpleCommand([=](auto state) {
      swap->swapWith(m_type);
      initUnisonVoicesScaling(m_type);
      m_signalTypeChanged.send(m_type);
      onChange();
    });
  }
}

Glib::ustring EditBuffer::getVoiceGroupName(VoiceGroup vg) const
{
  nltools_assertOnDevPC(vg == VoiceGroup::I || vg == VoiceGroup::II);
  return m_voiceGroupLabels[static_cast<size_t>(vg)];
}

Glib::ustring EditBuffer::getVoiceGroupNameWithSuffix(VoiceGroup vg, bool addSpace) const
{
  auto monoUnisonVoiceGroup = vg;
  auto isLayer = getType() == SoundType::Layer;
  if(isLayer)
    monoUnisonVoiceGroup = VoiceGroup::I;
  bool mono = isMonoEnabled(monoUnisonVoiceGroup);
  bool unison = hasMoreThanOneUnisonVoice(monoUnisonVoiceGroup);

  return getVoiceGroupName(vg) + (addSpace ? "\u202F" : "") + (mono ? "\uE040" : "") + (unison ? "\uE041" : "");
}

bool EditBuffer::hasMoreThanOneUnisonVoice(const VoiceGroup &vg) const
{
  if(auto p = findParameterByID({ C15::PID::Unison_Voices, vg }))
    return p->getControlPositionValue() > 0;
  nltools::Log::error(__PRETTY_FUNCTION__, "parameter not found!");
  return false;
}

bool EditBuffer::isMonoEnabled(const VoiceGroup &vg) const
{
  if(auto p = findParameterByID({ C15::PID::Mono_Grp_Enable, vg }))
    return p->getControlPositionValue() > 0;
  nltools::Log::error(__PRETTY_FUNCTION__, "parameter not found!");
  return false;
}

Glib::ustring EditBuffer::getNameWithSuffix() const
{
  auto hasMono = false;
  auto hasUnison = false;
  switch(getType())
  {
    case SoundType::Layer:
    case SoundType::Single:
      hasMono |= isMonoEnabled(VoiceGroup::I);
      hasUnison |= hasMoreThanOneUnisonVoice(VoiceGroup::I);
      break;
    case SoundType::Split:
      hasMono |= isMonoEnabled(VoiceGroup::I);
      hasMono |= isMonoEnabled(VoiceGroup::II);
      hasUnison |= hasMoreThanOneUnisonVoice(VoiceGroup::I);
      hasUnison |= hasMoreThanOneUnisonVoice(VoiceGroup::II);
      break;
    default:
      break;
  }

  return getName() + " " + (hasMono ? "\uE040" : "") + (hasUnison ? "\uE041" : "");
}

void EditBuffer::initUnisonVoicesScaling(SoundType newType)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    if(auto unisonParam = dynamic_cast<UnisonVoicesParameter *>(findParameterByID({ C15::PID::Unison_Voices, vg })))
      unisonParam->updateScaling(newType);
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
    if(ParameterFactory::isUnisonParameter(parameter))
      return false;

    if(ParameterFactory::isMonoParameter(parameter))
      return false;

    return ParameterFactory::isSplitPoint(parameter) || selectedIsNotGlobal;
  }

  if(type == SoundType::Split)
  {
    return ParameterFactory::isSplitPoint(parameter) || selectedIsNotGlobal;
  }

  return false;
}

void EditBuffer::undoableInitPart(UNDO::Transaction *transaction, VoiceGroup vg, Defaults mode)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);

  for(auto &group : getParameterGroups(vg))
    group->undoableLoadDefault(transaction, mode);

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

EditBuffer::PartOrigin EditBuffer::getPartOrigin(VoiceGroup vg) const
{
  PartOrigin ret;

  if(vg == VoiceGroup::I)
  {
    ret.presetUUID = Uuid { getAttribute("origin-I", "") };
    ret.sourceGroup = to<VoiceGroup>(getAttribute("origin-I-vg", "Global"));
  }
  else if(vg == VoiceGroup::II)
  {
    ret.presetUUID = Uuid { getAttribute("origin-II", "") };
    ret.sourceGroup = to<VoiceGroup>(getAttribute("origin-II-vg", "Global"));
  }

  return ret;
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

void EditBuffer::undoableLoadSelectedToPart(VoiceGroup from, VoiceGroup to)
{
  if(auto selectedPreset = getParent()->getSelectedPreset())
  {
    EditBufferUseCases useCase(*this);
    useCase.loadToPart(selectedPreset, from, to);
  }
}

void EditBuffer::cleanupParameterSelectionOnSoundTypeChange(UNDO::Transaction *transaction, SoundType oldType,
                                                            SoundType newType)
{
  using ParameterNumberMap = std::unordered_map<int, int>;
  using From = SoundType;
  using To = SoundType;
  using Conversion = std::pair<From, To>;
  using ConversionMap = std::map<Conversion, ParameterNumberMap>;

  static ConversionMap conversions {
    { { From::Layer, To::Split },
      { { C15::PID::FB_Mix_Osc, C15::PID::FB_Mix_Osc },
        { C15::PID::FB_Mix_Osc_Src, C15::PID::FB_Mix_Osc },
        { C15::PID::FB_Mix_Comb_Src, C15::PID::FB_Mix_Comb },
        { C15::PID::FB_Mix_SVF_Src, C15::PID::FB_Mix_SVF },
        { C15::PID::Voice_Grp_Fade_From, C15::PID::Voice_Grp_Volume },
        { C15::PID::Voice_Grp_Fade_Range, C15::PID::Voice_Grp_Volume } } },
    { { From::Layer, To::Single },
      { { C15::PID::FB_Mix_Osc, C15::PID::FB_Mix_Osc },
        { C15::PID::FB_Mix_Osc_Src, C15::PID::FB_Mix_Osc },
        { C15::PID::FB_Mix_Comb_Src, C15::PID::FB_Mix_Comb },
        { C15::PID::FB_Mix_SVF_Src, C15::PID::FB_Mix_SVF },
        { C15::PID::Voice_Grp_Volume, C15::PID::Master_Volume },
        { C15::PID::Voice_Grp_Tune, C15::PID::Master_Tune },
        { C15::PID::Voice_Grp_Fade_From, C15::PID::Master_Volume },
        { C15::PID::Voice_Grp_Fade_Range, C15::PID::Master_Volume } } },
    { { From::Split, To::Layer }, { { C15::PID::Split_Split_Point, C15::PID::Voice_Grp_Volume } } },
    { { From::Split, To::Single },
      { { C15::PID::Split_Split_Point, C15::PID::Master_Volume },
        { C15::PID::Voice_Grp_Tune, C15::PID::Master_Tune },
        { C15::PID::Voice_Grp_Volume, C15::PID::Master_Volume } } },
    { { From::Single, To::Layer }, { { C15::PID::Master_FX_Mix, C15::PID::Master_Volume } } },
    { { From::Single, To::Split }, { { C15::PID::Master_FX_Mix, C15::PID::Master_Volume } } }
  };

  if(Application::exists())
  {
    auto vgManager = Application::get().getVGManager();
    auto currentVg = vgManager->getCurrentVoiceGroup();

    auto itMap = conversions.find({ oldType, newType });
    if(itMap != conversions.end())
    {
      const auto &conv = itMap->second;
      auto id = getSelectedParameterNumber();
      auto itConv = conv.find(id);
      if(itConv != conv.end())
      {
        auto vg = ParameterId::isGlobal(itConv->second) ? VoiceGroup::Global : currentVg;

        if(newType == SoundType::Single && vg == VoiceGroup::II)
          vg = VoiceGroup::I;

        undoableSelectParameter(transaction, { itConv->second, vg }, false);
        vgManager->setCurrentVoiceGroup(transaction, vg, true);
      }
    }

    if(newType == SoundType::Single && currentVg == VoiceGroup::II)
    {
      auto selNum = getSelectedParameterNumber();
      if(!ParameterId::isGlobal(selNum))
        undoableSelectParameter(transaction, { selNum, VoiceGroup::I }, false);

      vgManager->setCurrentVoiceGroup(transaction, VoiceGroup::I, true);
    }
  }
}

int EditBuffer::getSelectedParameterNumber() const
{
  return m_lastSelectedParameter.getNumber();
}

std::string EditBuffer::getPresetOriginDescription() const
{
  if(m_lastLoadedPreset == Uuid::init())
    return "Init";

  if(m_lastLoadedPreset == Uuid::converted())
    return "Converted";

  return m_presetOriginDescription;
}

LoadedPresetLog *EditBuffer::getLoadedPresetLog() const
{
  return m_loadedPresetLog.get();
}

void EditBuffer::fakeParameterSelectionSignal(VoiceGroup oldGroup, VoiceGroup group)
{
  m_signalSelectedParameter.send(getSelected(oldGroup), getSelected(group));
}

bool EditBuffer::isPartLabelChanged(VoiceGroup group) const
{
  if(auto preset = getOrigin())
  {
    return preset->getVoiceGroupName(group) != getVoiceGroupName(group);
  }
  return false;
}

void EditBuffer::setSyncSplitSettingAccordingToLoadedPreset(UNDO::Transaction *transaction)
{
  if(getType() == SoundType::Split)
  {
    const auto sI = findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });

    SyncSplitSettingUseCases useCase(*m_settings.getSetting<SplitPointSyncParameters>(), *getParent());

    if(sI->hasOverlap())
      useCase.disableSyncSetting(transaction);
    else
      useCase.enableSyncSetting(transaction);
  }
}

sigc::connection EditBuffer::onParameterReselected(const sigc::slot<void, Parameter *> &s)
{
  return m_signalReselectParameter.connect(s);
}

void EditBuffer::undoableSetTypeFromConvert(UNDO::Transaction *transaction, SoundType type)
{
  if(m_type != type)
  {
    auto swap = UNDO::createSwapData(type);

    cleanupParameterSelectionOnSoundTypeChange(transaction, m_type, type);

    transaction->addSimpleCommand([=](auto state) {
      swap->swapWith(m_type);
      initUnisonVoicesScaling(m_type);
      m_signalTypeChanged.send(m_type);
      m_signalConversionHappened.send(m_type);
      onChange();
    });
  }
}

std::shared_ptr<ScopedGuard::Lock> EditBuffer::getParameterFocusLockGuard()
{
  return m_parameterFocusLock.lock();
}

bool EditBuffer::isParameterFocusLocked() const
{
  return m_parameterFocusLock.isLocked();
}

AudioEngineProxy &EditBuffer::getAudioEngineProxy() const
{
  if(auto ae = m_audioEngineProxyContainer.get())
    return *ae;

  nltools_assertAlways(false);
}

Settings &EditBuffer::getSettings() const
{
  return m_settings;
}

std::vector<double> EditBuffer::setHWSourcesToDefaultValues(UNDO::Transaction *transaction)
{
  std::vector<double> ret;
  for(auto &hw : getParameterGroupByID({ "CS", VoiceGroup::Global })->getParameters())
  {
    if(auto hwParam = dynamic_cast<PhysicalControlParameter *>(hw))
    {
      ret.emplace_back(hwParam->getControlPositionValue());
      if(hwParam->getReturnMode() != ReturnMode::None)
      {
        hwParam->setCPFromHwui(transaction, hwParam->getDefValueAccordingToMode());
      }
    }
  }
  return ret;
}

void EditBuffer::setHWSourcesToOldPositions(UNDO::Transaction *transaction, const std::vector<double> &oldPositions)
{
  auto group = getParameterGroupByID({ "CS", VoiceGroup::Global });
  int idx = 0;
  for(auto &hw : group->getParameters())
  {
    if(auto hwParam = dynamic_cast<PhysicalControlParameter *>(hw))
    {
      hwParam->setCPFromHwui(transaction, oldPositions[idx]);
      idx++;
    }
  }
}

void EditBuffer::setHWSourcesToLoadRulePositionsAndModulate(UNDO::Transaction *transaction)
{
  for(auto &p : getParameterGroupByID({ "CS", VoiceGroup::Global })->getParameters())
  {
    if(auto hw = dynamic_cast<PhysicalControlParameter *>(p))
    {
      if(hw->getID().getNumber() == C15::PID::Ribbon_1 || hw->getID().getNumber() == C15::PID::Ribbon_2
         || hw->getID().getNumber() == C15::PID::Ribbon_3 || hw->getID().getNumber() == C15::PID::Ribbon_4)
      {
        auto oldMode = hw->getLastReturnModeBeforePresetLoad();
        auto newMode = hw->getReturnMode();
        auto oldPos = hw->getLastControlPositionValueBeforePresetLoad();

        if(newMode == ReturnMode::Center && oldMode == ReturnMode::Center)
        {
          hw->setCPFromLoad(transaction, oldPos, false);
        }
      }
      else
      {
        hw->setCPFromLoad(transaction, hw->getLastControlPositionValueBeforePresetLoad(), false);
      }
    }
  }
}

ParameterDB &EditBuffer::getParameterDB()
{
  return m_parameterDB;
}

ParameterId EditBuffer::getLastSelectedMacroId() const
{
  return m_lastSelectedMacroControl;
}

std::vector<ParameterId> EditBuffer::findAllParametersOfType(C15::Descriptors::ParameterType type)
{
  std::vector<ParameterId> ret;
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    for(auto g : getParameterGroups(vg))
    {
      for(auto p : g->getParameters())
      {
        if(p->getType() == type)
          ret.emplace_back(p->getID());
      }
    }
  }
  return ret;
}

namespace
{
  template <typename T> std::vector<T> combine(const std::vector<T> &lhs, const std::vector<T> &rhs)
  {
    std::vector<T> ret;
    for(const auto &v : { lhs, rhs })
      for(auto &l : v)
        ret.emplace_back(l);
    return ret;
  }
}

std::vector<ParameterId> EditBuffer::findAllParametersOfType(const std::vector<C15::Descriptors::ParameterType> &types)
{
  std::vector<ParameterId> ret;
  for(auto t : types)
  {
    ret = combine(ret, findAllParametersOfType(t));
  }
  return ret;
}

void EditBuffer::sendPresetLoadSignal()
{
  SettingsUseCases useCases(getSettings());
  useCases.setRibbonSelection(SelectedRibbons::Ribbon1_2);
  m_signalPresetLoaded.send();
}

void EditBuffer::copyFromSuper(UNDO::Transaction *transaction, const Preset *preset, VoiceGroup from, VoiceGroup to)
{
  super::copyFrom(transaction, preset, from, to);
}
