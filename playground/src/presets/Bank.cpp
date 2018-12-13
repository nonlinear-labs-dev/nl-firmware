#include "Bank.h"
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <serialization/PresetBankMetadataSerializer.h>
#include <assert.h>
#include <tools/TimeTools.h>
#include <device-settings/DebugLevel.h>

Bank::Bank(UpdateDocumentContributor *parent)
    : super(parent)
    , m_presets(std::bind(&Bank::onChange, this, UpdateDocumentContributor::ChangeFlags::Generic),
                std::bind(&Bank::clonePreset, this, std::placeholders::_1))
{
}

Bank::Bank(UpdateDocumentContributor *parent, const Bank &other, bool ignoeUuids)
    : Bank(parent)
{
  if(!ignoeUuids)
  {
    m_uuid = other.m_uuid;
    m_selectedPresetUuid = other.m_selectedPresetUuid;
    m_attachedToBankWithUuid = other.m_attachedToBankWithUuid;
  }

  m_name = other.m_name;
  m_serializationDate = other.m_serializationDate;
  m_x = other.m_x;
  m_y = other.m_y;
  m_attachDirection = other.m_attachDirection;
  m_attributes = other.m_attributes;
  m_presets = other.m_presets;
}

Bank::~Bank() = default;

SaveResult Bank::save(RefPtr<Gio::File> bankFolder)
{
  try
  {
    if(saveMetadata(bankFolder) == SaveResult::Finished)
      return SaveResult::Again;

    if(savePresets(bankFolder) == SaveResult::Again)
      return SaveResult::Again;

    return SaveResult::Nothing;
  }
  catch(...)
  {
    DebugLevel::error("Could not save bank", getName(true));
  }
  return SaveResult::Nothing;
}

SaveResult Bank::saveMetadata(RefPtr<Gio::File> bankFolder)
{
  if(m_metadataLastSavedForUpdateID != getUpdateIDOfLastChange())
  {
    PresetBankMetadataSerializer serializer(this);
    serializer.write(bankFolder, ".metadata");
    m_metadataLastSavedForUpdateID = getUpdateIDOfLastChange();
    return SaveResult::Finished;
  }
  return SaveResult::Nothing;
}

SaveResult Bank::savePresets(RefPtr<Gio::File> bankFolder)
{
  if(m_presetsLastSavedForUpdateID != getUpdateIDOfLastChange())
  {
    for(auto &p : m_presets.getElements())
      if(p->save(bankFolder))
        return SaveResult::Again;

    m_presetsLastSavedForUpdateID = getUpdateIDOfLastChange();
  }
  return SaveResult::Nothing;
}

Preset *Bank::clonePreset(const Preset *p)
{
  return new Preset(this, *p, true);
}

void Bank::load(UNDO::Transaction *transaction, RefPtr<Gio::File> bankFolder, int numBank, int numBanks)
{
  loadMetadata(transaction, bankFolder);
  loadPresets(transaction, bankFolder);
}

uint64_t Bank::loadMetadata(UNDO::Transaction *transaction, RefPtr<Gio::File> bankFolder)
{
  auto lastChangeOnFile
      = Serializer::read<PresetBankMetadataSerializer>(transaction, bankFolder, ".metadata", this, false);
  return lastChangeOnFile;
}

void Bank::loadPresets(UNDO::Transaction *transaction, RefPtr<Gio::File> bankFolder)
{
  m_presets.forEach([&](Preset *p) { p->load(transaction, bankFolder); });
}

UpdateDocumentContributor::tUpdateID Bank::onChange(uint64_t flags)
{
  auto ret = AttributesOwner::onChange(flags);
  m_sigBankChanged.deferedSend();
  return ret;
}

const Uuid &Bank::getUuid() const
{
  return m_uuid;
}

string Bank::getName(bool withFallback) const
{
  if(m_name.empty())
    return "<Untitled Bank>";

  return m_name;
}

string Bank::getX() const
{
  return m_x;
}

string Bank::getY() const
{
  return m_y;
}

const Uuid &Bank::getSelectedPresetUuid() const
{
  return m_selectedPresetUuid;
}

Preset *Bank::getSelectedPreset() const
{
  return findPreset(getSelectedPresetUuid());
}

const Uuid &Bank::getAttachedToBankUuid() const
{
  return m_attachedToBankWithUuid;
}

string Bank::getAttachDirection() const
{
  return m_attachDirection;
}

size_t Bank::getNumPresets() const
{
  return m_presets.size();
}

size_t Bank::getPresetPosition(const Uuid &uuid) const
{
  return m_presets.getIndexOf(uuid);
}

size_t Bank::getPresetPosition(const Preset *preset) const
{
  return m_presets.getIndexOf(preset);
}

Preset *Bank::findPreset(const Uuid &uuid) const
{
  return m_presets.find(uuid);
}

Preset *Bank::findPresetNear(const Uuid &anchorUuid, int seek) const
{
  return m_presets.findNear(anchorUuid, seek);
}

Preset *Bank::findSelectedPreset() const
{
  return findPreset(m_selectedPresetUuid);
}

Preset *Bank::getPresetAt(size_t idx) const
{
  return m_presets.getElements().at(idx).get();
}

void Bank::forEachPreset(std::function<void(Preset *)> cb) const
{
  m_presets.forEach(cb);
}

Bank *Bank::getMasterTop() const
{
  if(toAttachDirection(getAttachDirection()) == AttachmentDirection::top)
    if(auto pm = dynamic_cast<const PresetManager *>(getParent()))
      return pm->findBank(getAttachedToBankUuid());

  return nullptr;
}

Bank *Bank::getMasterLeft() const
{
  if(toAttachDirection(getAttachDirection()) == AttachmentDirection::left)
    if(auto pm = dynamic_cast<const PresetManager *>(getParent()))
      return pm->findBank(getAttachedToBankUuid());

  return nullptr;
}

Bank *Bank::getSlaveRight() const
{
  if(auto pm = dynamic_cast<const PresetManager *>(getParent()))
    for(auto bank : pm->getBanks())
      if(auto masterLeft = bank->getMasterLeft())
        if(masterLeft == this)
          return bank;

  return nullptr;
}

Bank *Bank::getSlaveBottom() const
{
  if(auto pm = dynamic_cast<const PresetManager *>(getParent()))
    for(auto bank : pm->getBanks())
      if(auto masterLeft = bank->getMasterTop())
        if(masterLeft == this)
          return bank;

  return nullptr;
}

void Bank::selectNextPreset()
{
  auto pos = getPresetPosition(getSelectedPresetUuid()) + 1;
  if(pos < getNumPresets())
  {
    auto &p = m_presets.getElements()[pos];
    auto scope = getUndoScope().startTransaction("Select Preset '%0'", p->getName());
    selectPreset(scope->getTransaction(), p->getUuid());
  }
}

void Bank::selectPreviousPreset()
{
  auto pos = getPresetPosition(getSelectedPresetUuid());
  if(pos > 0)
  {
    auto &p = m_presets.getElements()[pos - 1];
    auto scope = getUndoScope().startTransaction("Select Preset '%0'", p->getName());
    selectPreset(scope->getTransaction(), p->getUuid());
  }
}

void Bank::rename(const ustring &name)
{
  auto scope = getUndoScope().startTransaction("Rename Preset '%0'", name);
  setName(scope->getTransaction(), name);
}

void Bank::attachBank(UNDO::Transaction *transaction, const Uuid &otherBank, Bank::AttachmentDirection dir)
{
  setAttachedToBank(transaction, otherBank);
  setAttachedDirection(transaction, to_string(dir));
}

void Bank::invalidate()
{
  onChange(ChangeFlags::Generic);
}

void Bank::setName(UNDO::Transaction *transaction, const string &name)
{
  transaction->addUndoSwap(this, m_name, name);
}

void Bank::setUuid(UNDO::Transaction *transaction, const Uuid &uuid)
{
  transaction->addUndoSwap(this, m_uuid, uuid);
}

void Bank::selectPreset(UNDO::Transaction *transaction, const Uuid &uuid)
{
  if(m_selectedPresetUuid != uuid)
  {
    transaction->addUndoSwap(this, m_selectedPresetUuid, uuid);
    static_cast<PresetManager *>(getParent())->onPresetSelectionChanged();
  }
}

void Bank::selectNextPreset(UNDO::Transaction *transaction)
{
  auto pos = getPresetPosition(getSelectedPresetUuid()) + 1;
  if(pos < getNumPresets())
    selectPreset(transaction, getPresetAt(pos)->getUuid());
}

void Bank::selectPreviousPreset(UNDO::Transaction *transaction)
{
  auto pos = getPresetPosition(getSelectedPresetUuid());
  if(pos > 0)
    selectPreset(transaction, getPresetAt(pos - 1)->getUuid());
}

void Bank::ensurePresetSelection(UNDO::Transaction *transaction)
{
  if(getNumPresets() && !findPreset(getSelectedPresetUuid()))
    selectPreset(transaction, m_presets.getElements().front()->getUuid());
}

void Bank::setAttachedToBank(UNDO::Transaction *transaction, const Uuid &uuid)
{
  transaction->addUndoSwap(this, m_attachedToBankWithUuid, uuid);
}

void Bank::setAttachedDirection(UNDO::Transaction *transaction, const string &direction)
{
  transaction->addUndoSwap(this, m_attachDirection, direction);
}

void Bank::setX(UNDO::Transaction *transaction, const string &x)
{
  transaction->addUndoSwap(this, m_x, x);
}

void Bank::setY(UNDO::Transaction *transaction, const string &y)
{
  transaction->addUndoSwap(this, m_y, y);
}

Preset *Bank::appendPreset(UNDO::Transaction *transaction)
{
  return appendPreset(transaction, std::make_unique<Preset>(this));
}

Preset *Bank::appendPreset(UNDO::Transaction *transaction, std::unique_ptr<Preset> preset)
{
  return m_presets.append(transaction, std::move(preset));
}

Preset *Bank::prependPreset(UNDO::Transaction *transaction, std::unique_ptr<Preset> preset)
{
  return m_presets.prepend(transaction, std::move(preset));
}

Preset *Bank::insertPreset(UNDO::Transaction *transaction, size_t pos, std::unique_ptr<Preset> preset)
{
  return m_presets.insert(transaction, pos, std::move(preset));
}

void Bank::movePreset(UNDO::Transaction *transaction, const Preset *toMove, const Preset *before)
{
  m_presets.move(transaction, toMove, before);
}

void Bank::movePresetBetweenBanks(UNDO::Transaction *transaction, Bank *tgtBank, Preset *presetToMove,
                                  const Preset *presetAnchor)
{
  if(tgtBank == this)
  {
    movePreset(transaction, presetToMove, presetAnchor);
  }
  else
  {
    auto p = m_presets.release(transaction, presetToMove);
    assert(p == presetToMove);
    auto pos = presetAnchor ? tgtBank->getPresetPosition(presetAnchor) : tgtBank->getNumPresets();
    tgtBank->m_presets.adopt(transaction, pos, p);
  }
}

void Bank::deletePreset(UNDO::Transaction *transaction, const Uuid &uuid)
{
  m_presets.remove(transaction, uuid);
}

connection Bank::onBankChanged(sigc::slot<void> cb)
{
  return m_sigBankChanged.connectAndInit(cb);
}

void Bank::removeChangeBlocker()
{
  m_lastChangeTimestampBlocked--;
}

void Bank::addChangeBlocker()
{
  m_lastChangeTimestampBlocked++;
}

void Bank::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto pm = dynamic_cast<const PresetManager *>(getParent());
  bool changed = knownRevision < getUpdateIDOfLastChange();

  writer.writeTag("preset-bank", Attribute("uuid", m_uuid.raw()), Attribute("name", m_name), Attribute("x", getX()),
                  Attribute("y", getY()), Attribute("selected-preset", getSelectedPresetUuid().raw()),
                  Attribute("order-number", pm->getBankPosition(getUuid()) + 1), Attribute("changed", changed), [&]() {
                    if(changed)
                    {
                      AttributesOwner::writeDocument(writer, knownRevision);
                      forEachPreset([&](auto p) { p->writeDocument(writer, knownRevision); });
                      writer.writeTextElement("date-of-last-change",
                                              TimeTools::getDisplayStringFromStamp(m_lastChangedTimestamp));
                      writer.writeTextElement("attached-to", getAttachedToBankUuid().raw());
                      writer.writeTextElement("attached-direction", getAttachDirection());
                      writer.writeTextElement("state", calcStateString());
                    }
                  });
}

bool Bank::empty() const
{
  return m_presets.empty();
}

void Bank::copyFrom(UNDO::Transaction *transaction, const Bank *other, bool ignoreUuids)
{
  if(!ignoreUuids)
    setUuid(transaction, other->getUuid());

  setName(transaction, other->getName(false));
  setX(transaction, other->getX());
  setY(transaction, other->getY());

  transaction->addUndoSwap(this, m_attributes, other->m_attributes);

  other->forEachPreset([&](auto p) { m_presets.append(transaction, std::make_unique<Preset>(this, p, ignoreUuids)); });

  m_lastChangedTimestamp = TimeTools::getAdjustedTimestamp();
}

string to_string(Bank::AttachmentDirection dir)
{
  static_assert(Bank::AttachmentDirection::none == 0,
                "Nicht den Enum ändern ohne diese Funktion und Java Seite zu ändern!");
  static_assert(Bank::AttachmentDirection::top == 1,
                "Nicht den Enum ändern ohne diese Funktion und Java Seite zu ändern!");
  static_assert(Bank::AttachmentDirection::left == 2,
                "Nicht den Enum ändern ohne diese Funktion und Java Seite zu ändern!");
  static_assert(Bank::AttachmentDirection::count == 3,
                "Nicht den Enum ändern ohne diese Funktion und Java Seite zu ändern!");

  switch(dir)
  {
    case Bank::AttachmentDirection::top:
      return "top";

    case Bank::AttachmentDirection::left:
      return "left";

    case Bank::AttachmentDirection::none:
      return "";

    case Bank::AttachmentDirection::count:
      g_warn_if_reached();
  }
  return "";
}

Bank::AttachmentDirection toAttachDirection(const string &str)
{
  if(str == "top")
    return Bank::AttachmentDirection::top;

  if(str == "left")
    return Bank::AttachmentDirection::left;

  return Bank::AttachmentDirection::none;
}

int Bank::getHighestIncrementForBaseName(const Glib::ustring &baseName) const
{
  static auto regex = Glib::Regex::create("^(.*)\\s([0-9]+)$");

  bool hadMatch = false;
  int h = 0;

  m_presets.forEach([&](auto p) {
    ustring name = p->getName();

    if(name == baseName)
    {
      h = std::max(h, 1);
      hadMatch = true;
    }
    else if(name.find(baseName) == 0)
    {
      Glib::MatchInfo matchInfo;

      if(regex->match(name, matchInfo) && matchInfo.get_match_count() > 2)
      {
        auto presetsBaseName = matchInfo.fetch(1);

        if(presetsBaseName == baseName)
        {
          auto number = matchInfo.fetch(2);
          int newNumber = stoi(number);
          h = std::max(h, newNumber);
          hadMatch = true;
        }
      }
    }
  });

  if(hadMatch)
    return std::max(1, h);

  return 0;
}

void Bank::searchPresets(Writer &writer, const SearchQuery &query) const
{
  auto pm = dynamic_cast<const PresetManager *>(getParent());
  auto pos = pm->getBankPosition(getUuid());

  writer.writeTag("preset-bank", Attribute("uuid", m_uuid.raw()), Attribute("name", m_name),
                  Attribute("order-number", pos), [&]() {
                    m_presets.forEach([&](auto p) {
                      if(p->matchesQuery(query))
                        p->writeDocument(writer, 0);
                    });
                  });
}

time_t Bank::getLastChangedTimestamp() const
{
  return m_lastChangedTimestamp;
}

const Glib::ustring Bank::calcStateString() const
{
  auto timestampWithTolerance = m_lastChangedTimestamp - 5;

  std::string lastExportTimeIso = getAttribute("Date of Export File", "-1");
  std::string lastModTimeIso = TimeTools::getIsoTime(timestampWithTolerance);
  std::string lastImportTime = getAttribute("Date of Import File", "-1");

  if(lastImportTime != "-1" && lastModTimeIso <= lastImportTime)
  {
    return "Unchanged since Import";
  }
  else if(lastModTimeIso.compare(lastExportTimeIso) < 0 || lastModTimeIso == lastExportTimeIso)
  {
    return "Saved by Export";
  }
  else
  {
    return "Not Saved By Export";
  }
}

Bank *Bank::getClusterMaster()
{
  if(auto master = static_cast<PresetManager *>(getParent())->findBank(getAttachedToBankUuid()))
    if(master != this)
      return master->getClusterMaster();

  return this;
}

void Bank::resolveCyclicAttachments(UNDO::Transaction *transaction)
{
  std::vector<Bank *> banks;
  resolveCyclicAttachments(transaction, banks);
}

bool Bank::resolveCyclicAttachments(UNDO::Transaction *transaction, std::vector<Bank *> stackedBanks)
{
  if(std::find(stackedBanks.begin(), stackedBanks.end(), this) != stackedBanks.end())
  {
    return false;
  }

  stackedBanks.push_back(this);

  auto master = Application::get().getPresetManager()->findBank(getAttachedToBankUuid());

  if(master != nullptr && !master->resolveCyclicAttachments(transaction, stackedBanks))
  {
    setAttachedToBank(transaction, Uuid::none());
    setAttachedDirection(transaction, "");
  }
  stackedBanks.pop_back();
  return true;
}
