#pragma once

#include "UndoableVector.h"
#include "AttributesOwner.h"

#include <memory>
#include <vector>
#include <map>
#include <string>

#include <serialization/PresetBankSerializer.h>
#include <tools/Uuid.h>
#include <sync/SyncedItem.h>

class Preset;
class PresetManager;

using PresetPtr = std::unique_ptr<Preset>;

class Bank : public AttributesOwner, public SyncedItem
{
 private:
  using super = AttributesOwner;

 public:
  enum AttachmentDirection
  {
    none,
    top,
    left,
    count
  };

  explicit Bank(UpdateDocumentContributor *parent);
  Bank(UpdateDocumentContributor *parent, const Bank &other);
  ~Bank() override;

  void load(UNDO::Transaction *transaction, Glib::RefPtr<Gio::File> bankFolder, int numBank, int numBanks);
  SaveResult save(Glib::RefPtr<Gio::File> bankFolder);

  // supported interfaces
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  // accessors
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool isCollapsed() const;
  [[nodiscard]] std::string getName(bool withFallback) const;
  [[nodiscard]] std::string getX() const;
  [[nodiscard]] std::string getY() const;
  [[nodiscard]] const Uuid &getSelectedPresetUuid() const;
  [[nodiscard]] Preset *getSelectedPreset() const;
  [[nodiscard]] const Uuid &getAttachedToBankUuid() const;
  [[nodiscard]] std::string getAttachDirection() const;
  [[nodiscard]] size_t getNumPresets() const;
  [[nodiscard]] size_t getPresetPosition(const Uuid &uuid) const;
  [[nodiscard]] size_t getPresetPosition(const Preset *preset) const;
  [[nodiscard]] Preset *findPreset(const Uuid &uuid) const;
  [[nodiscard]] Preset *findPresetNear(const Uuid &anchorUuid, int seek) const;
  [[nodiscard]] Preset *findSelectedPreset() const;
  [[nodiscard]] Preset *getPresetAt(size_t idx) const;
  [[nodiscard]] Bank *getMasterTop() const;
  [[nodiscard]] Bank *getMasterLeft() const;
  [[nodiscard]] Bank *getSlaveRight() const;
  [[nodiscard]] Bank *getSlaveBottom() const;
  [[nodiscard]] time_t getLastChangedTimestamp() const;
  [[nodiscard]] const Uuid &getUuid() const;

  nlohmann::json serialize() const override;

  void attachBank(UNDO::Transaction *transaction, const Uuid &otherBank, AttachmentDirection dir);
  void invalidate();

  void forEachPreset(std::function<void(Preset *)> cb) const;
  Preset *clonePreset(const Preset *p);

  // transactions
  void setName(UNDO::Transaction *transaction, const std::string &name);
  void setUuid(UNDO::Transaction *transaction, const Uuid &uuid);
  void selectPreset(UNDO::Transaction *transaction, const Uuid &uuid);
  void selectPreset(UNDO::Transaction *transaction, size_t pos);
  void ensurePresetSelection(UNDO::Transaction *transaction);
  void setAttachedToBank(UNDO::Transaction *transaction, const Uuid &uuid);
  void setAttachedDirection(UNDO::Transaction *transaction, const std::string &direction);
  void setX(UNDO::Transaction *transaction, const std::string &x);
  void setY(UNDO::Transaction *transaction, const std::string &y);
  void updateLastModifiedTimestamp(UNDO::Transaction *transaction);

  Preset *appendPreset(UNDO::Transaction *transaction);
  Preset *appendPreset(UNDO::Transaction *transaction, std::unique_ptr<Preset> preset);
  Preset *prependPreset(UNDO::Transaction *transaction, std::unique_ptr<Preset> preset);
  Preset *insertPreset(UNDO::Transaction *transaction, size_t pos, std::unique_ptr<Preset> preset);

  void movePreset(UNDO::Transaction *transaction, const Preset *toMove, const Preset *before);
  void movePresetBetweenBanks(UNDO::Transaction *transaction, Preset *presetToMove, Bank *tgtBank,
                              const Preset *presetAnchor);

  void deletePreset(UNDO::Transaction *transaction, const Uuid &uuid);
  void resolveCyclicAttachments(UNDO::Transaction *transaction);

  // algorithm
  int getHighestIncrementForBaseName(const Glib::ustring &baseName) const;

  const Glib::ustring calcStateString() const;
  Bank *getClusterMaster();

  // signals
  sigc::connection onBankChanged(sigc::slot<void> cb);

  PresetManager *getPresetManager() const;

  bool isMidiSelectedBank() const;
  bool hasPresets() const;
  Glib::ustring getComment();

 private:
  using Attributes = std::map<std::string, std::string>;

  uint64_t loadMetadata(UNDO::Transaction *transaction, Glib::RefPtr<Gio::File> bankFolder,
                        std::function<void(const std::string &)> progressCB);
  void loadPresets(UNDO::Transaction *transaction, Glib::RefPtr<Gio::File> bankFolder);
  void deleteOldPresetFiles(Glib::RefPtr<Gio::File> bankFolder);

  SaveResult saveMetadata(Glib::RefPtr<Gio::File> bankFolder);
  SaveResult savePresets(Glib::RefPtr<Gio::File> bankFolder);

  bool resolveCyclicAttachments(UNDO::Transaction *transaction, std::vector<Bank *> stackedBanks);

  UpdateDocumentContributor::tUpdateID onChange(uint64_t flags) override;
  size_t getNextPresetPosition() const;
  size_t getPreviousPresetPosition() const;

  Uuid m_attachedToBankWithUuid;
  std::string m_name;
  std::string m_serializationDate;
  std::string m_x;
  std::string m_y;
  std::string m_attachDirection;

  UndoableVector<Bank, Preset> m_presets;

  uint64_t m_lastChangedTimestamp = 0;
  tUpdateID m_metadataLastSavedForUpdateID = 0;
  tUpdateID m_presetsLastSavedForUpdateID = 0;

  Signal<void> m_sigBankChanged;
  Uuid m_uuid;

  friend class PresetBankSerializer;
  friend class PresetBankMetadataSerializer;
};

std::string to_string(Bank::AttachmentDirection dir);
Bank::AttachmentDirection toAttachDirection(const std::string &str);
