#pragma once

#include "ParameterGroupSet.h"
#include "presets/recall/RecallParameterGroups.h"
#include "nltools/GenericScopeGuard.h"
#include "parameters/names/ParameterDB.h"
#include <nltools/threading/Expiration.h>
#include <sync/SyncedItem.h>
#include <tools/DelayedJob.h>
#include <tools/ScopedGuard.h>
#include <tools/Uuid.h>
#include <utility>

class Application;
class Writer;
class PresetManager;
class HWUI;
class SplitPointParameter;
class LoadedPresetLog;
class Settings;
class AudioEngineProxy;
class Setting;

class EditBuffer : public ParameterGroupSet, public SyncedItem
{
 private:
  typedef ParameterGroupSet super;

 public:
  explicit EditBuffer(PresetManager *parent, Settings &settings, std::unique_ptr<AudioEngineProxy> &aeProxyContainer);
  ~EditBuffer() override;

  Glib::ustring getName() const;
  Glib::ustring getNameWithSuffix() const;
  Glib::ustring getVoiceGroupName(VoiceGroup vg) const;
  Glib::ustring getVoiceGroupNameWithSuffix(VoiceGroup vg, bool addSpace) const;
  size_t getHash() const;
  const Preset *getOrigin() const;
  Parameter *getSelected(VoiceGroup voiceGroup) const;
  int getSelectedParameterNumber() const;
  std::string getPresetOriginDescription() const;
  std::vector<ParameterId> findAllParametersOfType(C15::Descriptors::ParameterType type);
  std::vector<ParameterId> findAllParametersOfType(const std::vector<C15::Descriptors::ParameterType> &types);

  ParameterId getLastSelectedMacroId() const;

  LoadedPresetLog *getLoadedPresetLog() const;

  bool isZombie() const;

  void setMacroControlValueFromMCView(const ParameterId &id, double value, const Glib::ustring &uuid);

  void undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p, bool sendReselectionSignal);
  void undoableSelectParameter(UNDO::Transaction *transaction, const ParameterId &id, bool sendReselectionSignal);

  void undoableLoadToPart(UNDO::Transaction *trans, const Preset *p, VoiceGroup from, VoiceGroup to);

  void undoableLoadSelectedToPart(VoiceGroup from, VoiceGroup to);

  void fakeParameterSelectionSignal(VoiceGroup oldGroup, VoiceGroup group);
  void undoableSetLoadedPresetInfo(UNDO::Transaction *transaction, const Preset *preset, bool resetRecall = true);
  void undoableUpdateLoadedPresetInfo(UNDO::Transaction *transaction);

  void undoableSetDefaultValues(UNDO::Transaction *transaction, Preset *values);
  void undoableLockAllGroups(UNDO::Transaction *transaction);
  void undoableUnlockAllGroups(UNDO::Transaction *transaction);
  void undoableToggleGroupLock(UNDO::Transaction *transaction, const Glib::ustring &groupId);

  void setName(UNDO::Transaction *transaction, const Glib::ustring &name);
  void setVoiceGroupName(UNDO::Transaction *transaction, const Glib::ustring &name, VoiceGroup vg);

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  Uuid getUUIDOfLastLoadedPreset() const;

  PresetManager *getParent();
  const PresetManager *getParent() const;
  AudioEngineProxy &getAudioEngineProxy() const;
  Settings &getSettings() const;

  void resetModifiedIndicator(UNDO::Transaction *transaction);
  void resetModifiedIndicator(UNDO::Transaction *transaction, size_t hash);

  void copyFrom(UNDO::Transaction *transaction, const Preset *preset) override;
  void copyFromSuper(UNDO::Transaction *transaction, const Preset *preset, VoiceGroup from, VoiceGroup to);

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;

  std::vector<double> setHWSourcesToDefaultValues(UNDO::Transaction *transaction);
  void setHWSourcesToOldPositions(UNDO::Transaction *transaction, const std::vector<double> &oldPositions);

  bool hasLocks() const;
  bool hasLocks(VoiceGroup vg) const;
  bool findAnyParameterChanged() const;
  void resetOriginIf(const Preset *p);
  bool isDual() const;

  // CALLBACKS
  sigc::connection onSelectionChanged(const sigc::slot<void, Parameter *, Parameter *> &s,
                                      std::optional<VoiceGroup> initialVG);
  sigc::connection onParameterReselected(const sigc::slot<void, Parameter *> &s);
  sigc::connection onModificationStateChanged(const sigc::slot<void, bool> &s);
  sigc::connection onChange(const sigc::slot<void> &s, bool init = true);
  sigc::connection onPresetLoaded(const sigc::slot<void> &s);
  sigc::connection onLocksChanged(const sigc::slot<void> &s);
  sigc::connection onRecallValuesChanged(const sigc::slot<void> &s);
  sigc::connection onSoundTypeChanged(const sigc::slot<void, SoundType> &s);
  sigc::connection onSoundTypeChanged(const sigc::slot<void, SoundType> &s, bool init);
  sigc::connection onEditBufferConverted(const sigc::slot<void, SoundType> &s);

  bool isModified() const;

  //RECALL
  RecallParameterGroups &getRecallParameterSet();
  void initRecallValues(UNDO::Transaction *t);

  SoundType getType() const;

  static bool isDualParameterForSoundType(const Parameter *parameter, SoundType type);

  void TEST_doDeferredJobs();

  struct PartOrigin
  {
    PartOrigin(Uuid preset, VoiceGroup vg)
        : presetUUID { std::move(preset) }
        , sourceGroup { vg }
    {
    }

    PartOrigin()
        : presetUUID { Uuid::none() }
        , sourceGroup { VoiceGroup::Global }
    {
    }

    Uuid presetUUID;
    VoiceGroup sourceGroup;
  };

  PartOrigin getPartOrigin(VoiceGroup vg) const;
  ParameterDB &getParameterDB();

  std::shared_ptr<ScopedGuard::Lock> getParameterFocusLockGuard();
  bool isParameterFocusLocked() const;
  void init(Settings *settings) override;

 private:
  friend class PresetManager;
  friend class LastLoadedPresetInfoSerializer;
  friend class EditBufferUseCases;
  friend class BankUseCases;
  friend class PresetManagerUseCases;
  friend class SoundUseCases;
  friend class EditBufferSerializer;
  friend class RecallEditBufferSerializer;
  friend class RecallEditBufferSerializer2;
  friend class EditBufferActions;
  friend class PresetParameterGroups;
  friend class EditBufferUseCases;
  friend class SoundUseCases;
  friend class EditBufferConverter;
  friend class EditBufferToPartLoader;

  nlohmann::json serialize() const override;

  Glib::ustring getEditBufferName() const;
  bool findAnyParameterChanged(VoiceGroup vg) const;

  Parameter *searchForAnyParameterWithLock(VoiceGroup vg) const;
  UNDO::Scope &getUndoScope() override;

  //Load
  void undoableLoad(UNDO::Transaction *transaction, const Preset *preset, bool sendToAudioEngine);

  //init
  void undoableInitSound(UNDO::Transaction *transaction, Defaults mode);
  void undoableInitPart(UNDO::Transaction *transaction, VoiceGroup group, Defaults mode);

  //randomize
  void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount);
  void undoableRandomizePart(UNDO::Transaction *transaction, VoiceGroup currentVoiceGroup, Initiator initiator,
                             double amount);

  //Convert
  void undoableConvertToDual(UNDO::Transaction *transaction, SoundType type, VoiceGroup currentVG);
  void undoableConvertToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void undoableSetType(UNDO::Transaction *transaction, SoundType type);
  void undoableSetTypeFromConvert(UNDO::Transaction *transaction, SoundType type);

  void doDeferedJobs();
  void checkModified();

  void initUnisonVoicesScaling(SoundType newType);

  void copyVoicesGroups(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to);

  void cleanupParameterSelectionOnSoundTypeChange(UNDO::Transaction *transaction, SoundType oldType, SoundType newType);
  bool isMonoEnabled(const VoiceGroup &vg) const;
  bool hasMoreThanOneUnisonVoice(const VoiceGroup &vg) const;

  bool isPartLabelChanged(VoiceGroup group) const;
  void setSyncSplitSettingAccordingToLoadedPreset(UNDO::Transaction *transaction);
  void setHWSourcesToLoadRulePositionsAndModulate(UNDO::Transaction *transaction);

  void sendPresetLoadSignal();

  void connectToFocusAndMode();
  void resetReturningParametersOnFocusAndModeChange(const Setting *s);

  void updateRibbonsFromBoundMacros();

  Signal<void, Parameter *, Parameter *> m_signalSelectedParameter;
  Signal<void, Parameter *> m_signalReselectParameter;
  SignalWithCache<void, bool> m_signalModificationState;
  Signal<void> m_signalChange;
  Signal<void> m_signalPresetLoaded;
  Signal<void> m_signalLocksChanged;
  Signal<void, SoundType> m_signalTypeChanged;
  Signal<void, SoundType> m_signalConversionHappened;

  sigc::connection m_voiceGroupConnection;

  ParameterDB m_parameterDB;
  Uuid m_lastLoadedPreset;

  Glib::ustring m_name;
  std::string m_presetOriginDescription;
  std::array<Glib::ustring, 2> m_voiceGroupLabels;

  DelayedJob m_deferredJobs;

  bool m_isModified;
  RecallParameterGroups m_recallSet;
  SoundType m_type;
  ParameterId m_lastSelectedParameter;
  size_t m_hashOnStore;

  mutable Preset *m_originCache { nullptr };

  std::unique_ptr<LoadedPresetLog> m_loadedPresetLog;
  ScopedGuard m_parameterFocusLock;
  Settings &m_settings;
  std::unique_ptr<AudioEngineProxy> &m_audioEngineProxyContainer;
  ParameterId m_lastSelectedMacroControl;
};
