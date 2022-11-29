#pragma once

#include "UndoableVector.h"

#include <memory>
#include <vector>
#include <string>

#include <tools/ScopedGuard.h>
#include <nltools/threading/Throttler.h>
#include <tools/Uuid.h>
#include <presets/SearchQuery.h>
#include <tools/RecursionGuard.h>
#include <http/ContentSection.h>
#include <tools/Signal.h>
#include <nltools/Types.h>

#include <sync/SyncedItem.h>
#include <serialization/Serializer.h>

class Bank;
class Preset;
class EditBuffer;
class Parameter;
class PresetManagerSerializer;
class RPCActionManager;
class NetworkRequest;
class AudioEngineProxy;
class Options;
class Settings;

class PresetManager : public UpdateDocumentContributor, public SyncedItem
{
  using SaveSubTask = std::function<SaveResult()>;

 public:
  explicit PresetManager(UpdateDocumentContributor *parent, bool readOnly, const Options &options, Settings &settings,
                         std::unique_ptr<AudioEngineProxy> &aeProxyContainer);
  ~PresetManager() override;

  void init(AudioEngineProxy *aeProxy, Settings &settings, Serializer::Progress progress);
  void invalidate();

  // debug
  void stress(int numTransactions);
  void stressAllParams(int numParamChangedForEachParameter);
  void stressParam(UNDO::Transaction *transaction, Parameter *param);
  void stressBlocking(int numTransactions);
  void stressLoad(int numTransactions);
  void incAllParamsFine();

  // supported interfaces
  UpdateDocumentContributor::tUpdateID onChange(uint64_t flags = ChangeFlags::Generic) override;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const;

  bool isLoading() const;
  std::shared_ptr<ScopedGuard::Lock> getLoadingLock();

  // accessors
  Bank *findBank(const Uuid &uuid) const;
  Uuid getMidiSelectedBank() const;
  Bank *findMidiSelectedBank() const;
  Preset *findPreset(const Uuid &uuid) const;
  Bank *findBankWithPreset(const Uuid &uuid) const;
  size_t getNumBanks() const;
  std::list<Bank *> getBanks() const;
  Bank *getBankAt(size_t idx) const;
  const Uuid &getSelectedBankUuid() const;
  Bank *getSelectedBank() const;
  EditBuffer *getEditBuffer() const;
  void forEachBank(std::function<void(Bank *b)> cb) const;
  void selectMidiBank(UNDO::Transaction *trans, const Uuid &uuid);

  void onPresetStored();

  std::shared_ptr<ScopedGuard::Lock> lockLoading();

  // algorithms
  std::pair<double, double> calcDefaultBankPositionForNewBank() const;
  std::pair<double, double> calcDefaultBankPositionFor(const Bank *bank) const;
  size_t getBankPosition(const Uuid &uuid) const;

  // transactions
  Bank *addBank(UNDO::Transaction *transaction);
  Bank *addBank(UNDO::Transaction *transaction, std::unique_ptr<Bank> bank);
  void moveBank(UNDO::Transaction *transaction, const Bank *bankToMove, const Bank *moveBefore);
  void deleteBank(UNDO::Transaction *transaction, const Uuid &uuid);
  void handleDockingOnBankDelete(UNDO::Transaction *transaction, const Uuid &uuid);
  void selectBank(UNDO::Transaction *transaction, const Uuid &uuid);
  bool selectPreviousBank(UNDO::Transaction *transaction);
  bool selectNextBank(UNDO::Transaction *transaction);
  void sortBanks(UNDO::Transaction *transaction, const std::vector<Bank *> &banks);
  void storeInitSound(UNDO::Transaction *transaction);
  void resetInitSound(UNDO::Transaction *transaction);
  void loadInitSound(UNDO::Transaction *transaction);
  void clear(UNDO::Transaction *transaction);
  void setOrderNumber(UNDO::Transaction *transaction, const Uuid &bank, size_t targetPos);
  void sanitizeBankClusterRelations(UNDO::Transaction *transaction);
  void resolveCyclicAttachments(UNDO::Transaction *transaction) const;
  void ensureBankSelection(UNDO::Transaction *transaction);

  // algorithms
  Glib::ustring createPresetNameBasedOn(const Glib::ustring &basedOn) const;
  void searchPresets(Writer &writer, const Glib::ustring &q, const Glib::ustring &mode,
                     std::vector<SearchQuery::Fields> &&fieldsToSearch) const;

  // signals
  sigc::connection onBankSelection(sigc::slot<void, Uuid> cb);
  sigc::connection onNumBanksChanged(sigc::slot<void, size_t> cb);
  sigc::connection onRestoreHappened(sigc::slot<void> cb);
  sigc::connection onPresetStoreHappened(const sigc::slot<void>& cb);
  sigc::connection onMidiBankSelectionHappened(sigc::slot<void, Uuid> cb);
  sigc::connection onLoadHappened(const sigc::slot<void>& cb);

  const Preset *getSelectedPreset() const;
  Preset *getSelectedPreset();

 private:
  nlohmann::json serialize() const override;
  void loadMetadataAndSendEditBufferToPlaycontroller(UNDO::Transaction *transaction,
                                                     const Glib::RefPtr<Gio::File> &pmFolder, AudioEngineProxy *aeProxy,
                                                     Serializer::Progress progress);
  void loadInitSound(UNDO::Transaction *transaction, const Glib::RefPtr<Gio::File> &pmFolder,
                     Serializer::Progress progress);
  void loadBanks(UNDO::Transaction *transaction, Glib::RefPtr<Gio::File> pmFolder, Serializer::Progress progress);
  void fixMissingPresetSelections(UNDO::Transaction *transaction);
  Glib::ustring getBaseName(const Glib::ustring &basedOn) const;

  std::list<PresetManager::SaveSubTask> createListOfSaveSubTasks();
  SaveResult saveMetadata(Glib::RefPtr<Gio::File> pmFolder);
  SaveResult saveInitSound(Glib::RefPtr<Gio::File> pmFolder);
  SaveResult saveBanks(Glib::RefPtr<Gio::File> pmFolder);

  void doSaveTask();
  void popSaveTaskAndRecurseSynchronously();
  void popSaveTaskAndRecurseAsynchronously();
  void recurseSaveAsynchronously();
  void scheduleSave();

  size_t getPreviousBankPosition() const;
  size_t getNextBankPosition() const;
  bool selectBank(UNDO::Transaction *transaction, size_t idx);

  UndoableVector<PresetManager, Bank> m_banks;

  std::unique_ptr<EditBuffer> m_editBuffer;
  std::unique_ptr<Preset> m_initSound;
  Uuid m_midiSelectedBank;

  ScopedGuard m_isLoading;
  SignalWithCache<void, Uuid> m_sigBankSelection;
  SignalWithCache<void, size_t> m_sigNumBanksChanged;
  Signal<void> m_sigRestoreHappened;
  Signal<void> m_sigLoadHappened;
  Signal<void> m_presetStoreHappened;
  Signal<void, Uuid> m_sigMidiBankSelection;

  Expiration m_saveJob;
  tUpdateID m_lastSavedInitSoundUpdateID = 0;
  tUpdateID m_lastSavedMetaDataUpdateID = 0;

  std::list<SaveSubTask> m_saveTasks;
  bool m_savingBanksOnShutdown = false;
  bool m_saveRequestDuringSave = false;
  bool m_readOnly = false;

  friend class PresetManagerSerializer;
  friend class PresetManagerUseCases;

  const Options& m_options;
  std::function<void(const std::string &)> getProgressDecorator();
};
