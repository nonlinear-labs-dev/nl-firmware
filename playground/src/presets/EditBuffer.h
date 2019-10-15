#pragma once

#include "ParameterDualGroupSet.h"
#include "presets/recall/RecallParameterGroups.h"
#include <nltools/threading/Expiration.h>
#include <tools/DelayedJob.h>
#include <tools/Uuid.h>

class Application;
class Writer;
class PresetManager;

class EditBuffer : public ParameterDualGroupSet
{
 private:
  typedef ParameterDualGroupSet super;

 public:
  EditBuffer(PresetManager *parent);
  ~EditBuffer() override;

  Glib::ustring getName(VoiceGroup vg) const;
  Glib::ustring getName() const;
  size_t getHash() const;
  const Preset *getOrigin() const;
  Parameter *getSelected(VoiceGroup voiceGroup = VoiceGroup::Invalid) const;
  bool isZombie() const;

  void setMacroControlValueFromMCView(int id, double value, const Glib::ustring &uuid);

  static void sanitizeVoiceGroup(VoiceGroup &vg);

  void undoableSelectParameter(const Glib::ustring &id);
  void undoableSelectParameter(uint16_t id);
  void undoableSelectParameter(uint16_t id, VoiceGroup vg);
  void undoableSelectParameter(Parameter *p);
  void undoableSelectParameter(UNDO::Transaction *transaction, Parameter *p);

  void undoableLoad(UNDO::Transaction *transaction, Preset *preset);
  void undoableLoad(Preset *preset, VoiceGroup target = VoiceGroup::Invalid);

  void undoableLoadSelectedPreset();
  void undoableSetLoadedPresetInfo(UNDO::Transaction *transaction, Preset *preset);
  void undoableUpdateLoadedPresetInfo(UNDO::Transaction *transaction);
  void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator);
  void undoableInitSound(UNDO::Transaction *transaction);
  void undoableSetDefaultValues(UNDO::Transaction *transaction, Preset *values);
  void undoableLockAllGroups(UNDO::Transaction *transaction);
  void undoableUnlockAllGroups(UNDO::Transaction *transaction);
  void undoableToggleGroupLock(UNDO::Transaction *transaction, const Glib::ustring &groupId);

  void setName(UNDO::Transaction *transaction, const Glib::ustring &name);

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  Uuid getUUIDOfLastLoadedPreset() const;
  PresetManager *getParent();
  const PresetManager *getParent() const;

  void resetModifiedIndicator(UNDO::Transaction *transaction);
  void resetModifiedIndicator(UNDO::Transaction *transaction, size_t hash);

  void copyFrom(UNDO::Transaction *transaction, const Preset *preset) override;

  //void copyFrom(UNDO::Transaction *transaction, const LayerPreset *preset);
  //void copyFrom(UNDO::Transaction *transaction, const SplitPreset *preset);

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;

  bool hasLocks(VoiceGroup vg = VoiceGroup::Invalid) const;
  bool anyParameterChanged() const;
  void resetOriginIf(const Preset *p);

  // CALLBACKS
  sigc::connection onSelectionChanged(const slot<void, Parameter *, Parameter *> &s);
  sigc::connection onModificationStateChanged(slot<void, bool> s);
  sigc::connection onChange(slot<void> s);
  sigc::connection onPresetLoaded(slot<void> s);
  sigc::connection onLocksChanged(slot<void> s);
  sigc::connection onRecallValuesChanged(slot<void> s);
  sigc::connection onSoundTypeChanged(slot<void> s);

  bool isModified() const;
  void sendToLPC();

  //RECALL
  RecallParameterGroups &getRecallParameterSet();
  void initRecallValues(UNDO::Transaction *t);

  SoundType getType() const;

  void undoableConvertToSingle(UNDO::Transaction *transaction);
  void undoableConvertToDual(UNDO::Transaction *transaction, SoundType type, VoiceGroup copyFrom = VoiceGroup::I);

 private:
  bool anyParameterChanged(VoiceGroup vg) const;

  Parameter *searchForAnyParameterWithLock(VoiceGroup vg = VoiceGroup::Invalid) const;

  UNDO::Scope &getUndoScope() override;

  void setParameter(size_t id, double cpValuem, VoiceGroup target = VoiceGroup::Invalid);

  void undoableSelectParameter(UNDO::Transaction *transaction, const Glib::ustring &id);
  void setModulationSource(MacroControls src, VoiceGroup vg = VoiceGroup::Invalid);
  void setModulationAmount(double amount, VoiceGroup vg = VoiceGroup::Invalid);

  void doDeferedJobs();
  void checkModified();

  Signal<void, Parameter *, Parameter *> m_signalSelectedParameter;
  SignalWithCache<void, bool> m_signalModificationState;
  Signal<void> m_signalChange;
  Signal<void> m_signalPresetLoaded;
  Signal<void> m_signalLocksChanged;
  Signal<void> m_signalTypeChanged;

  Parameter::ID m_selectedParameterId;

  friend class EditBufferSerializer;
  friend class RecallEditBufferSerializer;
  friend class RecallEditBufferSerializer2;
  friend class EditBufferActions;
  friend class PresetParameterGroups;

  Uuid m_lastLoadedPreset;

  Glib::ustring m_name;
  DelayedJob m_deferredJobs;

  bool m_isModified;
  SoundType m_type;
  size_t m_hashOnStore;

  mutable Preset *m_originCache{ nullptr };
  RecallParameterGroups m_recallSet;

  friend class PresetManager;
  friend class LastLoadedPresetInfoSerializer;

  void undoableSetType(UNDO::Transaction *transaction, SoundType type);
};
