#pragma once

#include "AttributesOwner.h"
#include "PresetDualParameterGroups.h"
#include "PresetParameterGroup.h"
#include "ParameterId.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <tools/Uuid.h>
#include <tools/Signal.h>
#include <set>
#include "sync/SyncedItem.h"

class EditBuffer;
class Bank;
class PresetSerializer;
class ParameterGroupsSerializer;

class PresetParameterGroup;
class PresetParameter;
class PresetParameterGroupsSerializer;

class GroupId;

class Preset : public PresetDualParameterGroups, public SyncedItem
{
 private:
  using super = PresetDualParameterGroups;

 public:
  explicit Preset(UpdateDocumentContributor *parent);
  Preset(UpdateDocumentContributor *parent, const Preset &other);
  Preset(UpdateDocumentContributor *parent, const EditBuffer &editBuffer);
  Preset(UpdateDocumentContributor *parent, const EditBuffer &editBuffer, Uuid uuid);
  ~Preset() override;

  // supported interfaces
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  void load(UNDO::Transaction *transaction, const Glib::RefPtr<Gio::File> &presetPath);
  bool save(const Glib::RefPtr<Gio::File> &bankPath);
  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;
  void setAttribute(UNDO::Transaction *transaction, const std::string &key, const Glib::ustring &value) override;
  void clear(UNDO::Transaction *transaction) override;
  void invalidate();

  Glib::ustring getVoiceGroupName(VoiceGroup vg) const;
  void undoableSetVoiceGroupName(UNDO::Transaction *transaction, VoiceGroup vg, const Glib::ustring &name);

  // accessors
  Glib::ustring getName() const;
  Glib::ustring getHashtags() const;
  const Uuid &getUuid() const;
  bool isDual() const;
  Glib::ustring getDisplayNameWithSuffixes(bool addSpace) const;

  // transactions
  void setUuid(UNDO::Transaction *transaction, const Uuid &uuid);
  void setName(UNDO::Transaction *transaction, const Glib::ustring &name);
  void setType(UNDO::Transaction *transaction, SoundType type) override;
  void guessName(UNDO::Transaction *transaction);
  void setAutoGeneratedAttributes(UNDO::Transaction *transaction);

  // algorithms
  Glib::ustring buildUndoTransactionTitle(const Glib::ustring &prefix) const;
  Glib::ustring getTypeUnicode() const;

  void writeDiff(Writer &writer, const Preset *other, VoiceGroup vgOfThis, VoiceGroup vgOfOther) const;

  // signals
  sigc::connection onChanged(sigc::slot<void> cb);

  Glib::ustring getComment();
  std::optional<std::string> getOriginDescription() const;

  nlohmann::json serialize() const override;

  static Glib::ustring getDefaultProperties();

 private:
  void copyFrom(UNDO::Transaction *transaction, const Preset *other);
  void copyFrom(UNDO::Transaction *transaction, EditBuffer *edit);

  void lockAutoGeneratedAttributes();
  void unlockAutoGeneratedAttributes();

  EditBuffer *getEditBuffer() const;
  size_t getHash() const = delete;
  void updateBanksLastModifiedTimestamp(UNDO::Transaction *transaction);

  Glib::ustring m_name;
  std::array<Glib::ustring, 2> m_voiceGroupLabels;

  tUpdateID m_lastSavedUpdateID = 0;
  int m_autoGeneratedAttributesLockCount = 0;

  Signal<void> m_onChanged;

  // e.g. Preset was generated from editbuffer which has been initialized by "3: Pianos / 5: HonkyTonk"
  std::optional<std::string> m_originDescription;

  Uuid m_uuid;

  friend class PresetSerializer;
  friend class RecallEditBufferSerializer;
  friend class PresetParameterGroupsSerializer;
  friend class PresetAutoGeneratedAttributesLock;

  friend class PresetManagerUseCases;
  friend class PresetUseCases;

  friend class RecallParameterGroups;
};
