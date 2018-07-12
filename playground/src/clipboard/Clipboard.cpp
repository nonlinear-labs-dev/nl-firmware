#include "Clipboard.h"
#include "MultiplePresetSelection.h"
#include <presets/PresetManager.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>
#include <xml/Attribute.h>
#include <xml/Writer.h>
#include <Application.h>
#include <tools/StringTools.h>

Clipboard::Clipboard (UpdateDocumentContributor *parent) :
    ContentSection (parent),
    m_actions ("/clipboard/")
{
  m_actions.addAction ("copy-bank", [=](shared_ptr<NetworkRequest> request)
  {
    copyBank(request->get ("bank"));
    request->okAndComplete();
  });

  m_actions.addAction ("cut-preset", [=](shared_ptr<NetworkRequest> request)
  {
    cutPreset(request->get ("preset"));
    request->okAndComplete();
  });

  m_actions.addAction ("copy-preset", [=](shared_ptr<NetworkRequest> request)
  {
    copyPreset(request->get ("preset"));
    request->okAndComplete();
  });

  m_actions.addAction ("copy-presets", [=](shared_ptr<NetworkRequest> request)
  {
    copyPresets(request->get ("presets-csv"));
    request->okAndComplete();
  });

  m_actions.addAction ("paste-on-background", [=](shared_ptr<NetworkRequest> request)
  {
    auto x = request->get("x");
    auto y = request->get("y");

    if(containsBank())
      pasteBankOnBackground(getUndoScope().startTransaction ("Paste Bank"), x, y);
    else if(containsPreset())
      pastePresetOnBackground(x, y);
    else if(containsMultiplePresets())
      pasteMultiplePresetsOnBackground(x, y);

    request->okAndComplete();
  });

  m_actions.addAction ("paste-on-bank", [=](shared_ptr<NetworkRequest> request)
  {
    if(containsBank())
    pasteBankOnBank(getUndoScope().startTransaction ("Paste Bank"), request->get ("bank"));
    else if(containsPreset())
    pastePresetOnBank(request->get ("bank"));
    else if(containsMultiplePresets())
      pasteMultiplePresetsOnBank(request->get("bank"));

    request->okAndComplete();
  });

  m_actions.addAction ("paste-on-preset", [=](shared_ptr<NetworkRequest> request)
  {
    if(containsBank())
      pasteBankOnPreset(getUndoScope().startTransaction ("Paste Bank"), request->get ("preset"));
    else if(containsPreset())
      pastePresetOnPreset(request->get ("preset"));
    else if(containsMultiplePresets())
      pasteMultiplePresetsOnPreset(request->get("preset"));
    request->okAndComplete();
  });
}

Clipboard::~Clipboard ()
{
}

void Clipboard::handleHTTPRequest (shared_ptr<NetworkRequest> request, const Glib::ustring &path)
{
  ContentSection::handleHTTPRequest (request, path);

  if (m_actions.matches (path))
    if (m_actions.handleRequest (path, request))
      return;
}

Glib::ustring Clipboard::getPrefix () const
{
  return "clipboard";
}

bool Clipboard::containsMultiplePresets() const
{
  return dynamic_pointer_cast<MultiplePresetSelection>(m_content) != nullptr;
}

bool Clipboard::containsBank () const
{
  return dynamic_pointer_cast<PresetBank> (m_content) != nullptr;
}

bool Clipboard::containsPreset () const
{
  return dynamic_pointer_cast<Preset> (m_content) != nullptr;
}

bool Clipboard::hasContent() const
{
  return m_content.get();
}

void Clipboard::writeDocument (Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  bool changed = knownRevision < getUpdateIDOfLastChange ();

  writer.writeTag ("clipboard", Attribute ("changed", changed), [&]()
  {
    if (changed)
    {
      writer.writeTextElement("contains-bank", to_string(containsBank()));
      writer.writeTextElement("contains-preset", to_string(containsPreset()));
      writer.writeTextElement("contains-multiple-presets", to_string(containsMultiplePresets()));
    }
  });
}

void Clipboard::copyBank (const Glib::ustring &bankUuid)
{
  auto pm = Application::get ().getPresetManager ();
  if (auto bank = pm->findBank (bankUuid))
  {
    m_currentContentWasCut = false;
    auto scope = getUndoScope ().startTrashTransaction ();
    m_content.reset (new PresetBank (pm.get ()));
    static_pointer_cast<PresetBank> (m_content)->copyFrom (scope->getTransaction (), bank, false);
    onChange (UpdateDocumentContributor::ChangeFlags::Generic);
  }
}

void Clipboard::copyPresets(const Glib::ustring &csv) {

  auto pm = Application::get ().getPresetManager ();
  auto scope = getUndoScope ().startTrashTransaction ();
  m_currentContentWasCut = false;
  m_content.reset (new MultiplePresetSelection (getParent()));

  auto uuids = StringTools::splitStringOnAnyDelimiter(csv, ',');

  auto mulPresetSelection = static_pointer_cast<MultiplePresetSelection>(m_content);

  for(auto uuid: uuids) {
    if(auto preset = pm->findPreset(uuid)) {
      mulPresetSelection->addPreset(scope->getTransaction(), preset);
    }
  }
  onChange (UpdateDocumentContributor::ChangeFlags::Generic);
}

bool Clipboard::copyPreset (const Glib::ustring &presetUuid)
{
  auto pm = Application::get ().getPresetManager ();
  if (auto preset = pm->findPreset (presetUuid))
  {
    m_currentContentWasCut = false;
    auto scope = getUndoScope ().startTrashTransaction ();
    m_content = Preset::createPreset (nullptr);
    static_pointer_cast<Preset> (m_content)->copyFrom (scope->getTransaction (), preset.get (), false);
    onChange (UpdateDocumentContributor::ChangeFlags::Generic);
    return true;
  }
  return false;
}

void Clipboard::cutPreset (const Glib::ustring &presetUuid)
{
  if (copyPreset (presetUuid))
    m_currentContentWasCut = true;
}

void Clipboard::pasteOnBank (const Glib::ustring &bankUuid)
{
  if (containsPreset ())
    pastePresetOnBank (bankUuid);
  else if (containsBank ())
    pasteBankOnBank (getUndoScope ().startTransaction ("Paste Bank"), bankUuid);
  else if(containsMultiplePresets())
    pasteMultiplePresetsOnBank(bankUuid);
}

void Clipboard::pasteOnPreset (const Glib::ustring &presetUuid)
{
  if (containsPreset ())
    pastePresetOnPreset (presetUuid);
  else if (containsBank ())
    pasteBankOnPreset (getUndoScope().startTransaction ("Paste Bank"), presetUuid);
  else if (containsMultiplePresets())
    pasteMultiplePresetsOnPreset(presetUuid);
}

void Clipboard::pasteBankOnBackground (UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &x, const Glib::ustring &y)
{
  auto pm = Application::get ().getPresetManager ();
  auto transaction = scope->getTransaction ();
  auto newBank = pm->addBank (transaction, true);
  newBank->copyFrom (transaction, dynamic_pointer_cast<PresetBank> (m_content), true);
  newBank->undoableSetPosition (transaction, x, y);

  doCut (transaction);
}

std::shared_ptr<PresetBank> multiplePresetsToBank(const MultiplePresetSelection& mulPresets)
{
  auto scope = Application::get().getClipboard()->getUndoScope().startTrashTransaction();
  auto transaction = scope->getTransaction();
  auto b = std::make_shared<PresetBank>(Application::get().getPresetManager().get());
  for(auto preset: mulPresets.getPresets())
  {
    b->undoableCopyAndPrependPreset(transaction, preset);
  }
  b->undoableEnsurePresetSelection(transaction);
  return b;
}

void Clipboard::pasteMultiplePresetsOnBackground(const Glib::ustring& x, const Glib::ustring &y)
{
  auto scope = getUndoScope().startTransaction ("Paste Multiple Presets");
  auto mulSelection = dynamic_pointer_cast<MultiplePresetSelection>(m_content);
  m_content = multiplePresetsToBank(*mulSelection.get());
  pasteBankOnBackground(scope, x, y);
}

void Clipboard::pasteMultiplePresetsOnBank(const Glib::ustring &bankUuid)
{
  auto scope = getUndoScope().startTransaction ("Paste Multiple Presets");
  auto mulSelection = dynamic_pointer_cast<MultiplePresetSelection>(m_content);
  m_content = multiplePresetsToBank(*mulSelection.get());
  pasteBankOnBank(scope, bankUuid);
}

void Clipboard::pasteMultiplePresetsOnPreset(const Glib::ustring &presetUuid)
{
  auto scope = getUndoScope().startTransaction ("Paste Multiple Presets");
  auto mulSelection = dynamic_pointer_cast<MultiplePresetSelection>(m_content);
  m_content = multiplePresetsToBank(*mulSelection.get());
  pasteBankOnPreset(scope, presetUuid);
}

void Clipboard::pastePresetOnBackground (const Glib::ustring &x, const Glib::ustring &y)
{
  auto scope = getUndoScope ().startTransaction ("Paste Preset");
  auto pm = Application::get ().getPresetManager ();
  auto transaction = scope->getTransaction ();
  auto newBank = pm->addBank (transaction, true);
  newBank->undoableInsertPreset (transaction, 0);
  auto preset = newBank->getPreset (0);
  preset->copyFrom (transaction, dynamic_pointer_cast<Preset> (m_content).get (), true);
  newBank->undoableSetPosition (transaction, x, y);
  newBank->undoableEnsurePresetSelection(transaction);
  doCut (transaction);
}

void Clipboard::pasteBankOnBank (UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &bankUuid)
{
  auto pm = Application::get ().getPresetManager ();

  if (auto target = pm->findBank (bankUuid))
  {
    auto source = dynamic_pointer_cast<PresetBank> (m_content);

    if (size_t numPresets = source->getNumPresets ())
    {

      auto transaction = scope->getTransaction ();
      auto insertPos = target->getNumPresets ();

      for (size_t i = 0; i < numPresets; i++)
      {
        target->undoableInsertPreset (transaction, insertPos);
        target->getPreset (insertPos)->copyFrom (transaction, source->getPreset (i).get (), true);
        insertPos++;
      }

      doCut (transaction);
    }
  }
}

void Clipboard::pastePresetOnBank (const Glib::ustring &bankUuid)
{
  if (containsPreset ())
  {
    auto pm = Application::get ().getPresetManager ();

    if (auto target = pm->findBank (bankUuid))
    {
      auto scope = getUndoScope ().startTransaction ("Paste Preset");
      auto transaction = scope->getTransaction ();

      auto source = dynamic_pointer_cast<Preset> (m_content);
      auto insertPos = target->getNumPresets ();
      target->undoableInsertPreset (transaction, insertPos);
      target->getPreset (insertPos)->copyFrom (transaction, source.get (), true);

      doCut (transaction);
    }
  }
}

void Clipboard::pasteBankOnPreset (UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &presetUuid)
{
  auto pm = Application::get ().getPresetManager ();

  if (auto targetPreset = pm->findPreset (presetUuid))
  {
    if (auto targetBank = targetPreset->getBank ())
    {
      auto source = dynamic_pointer_cast<PresetBank> (m_content);

      if (size_t numPresets = source->getNumPresets ())
      {
        auto transaction = scope->getTransaction ();
        auto insertPos = targetBank->getPresetPosition (presetUuid);

        for (size_t i = 0; i < numPresets; i++)
        {
          targetBank->undoableInsertPreset (transaction, insertPos);
          targetBank->getPreset (insertPos)->copyFrom (transaction, source->getPreset (i).get (), true);
          insertPos++;
        }

        doCut (transaction);
      }
    }
  }
}

void Clipboard::pastePresetOnPreset (const Glib::ustring &presetUuid)
{
  auto pm = Application::get ().getPresetManager ();

  if (auto targetPreset = pm->findPreset (presetUuid))
  {
    if (auto targetBank = targetPreset->getBank ())
    {
      auto scope = getUndoScope ().startTransaction ("Paste Preset");
      auto transaction = scope->getTransaction ();

      auto insertPos = targetBank->getPresetPosition (presetUuid) + 1;
      auto source = dynamic_pointer_cast<Preset> (m_content);
      targetBank->undoableInsertPreset (transaction, insertPos);
      auto newPreset = targetBank->getPreset (insertPos);
      newPreset->copyFrom (transaction, source.get (), true);
      newPreset->undoableSelect (transaction);
      doCut (transaction);
    }
  }
}

void Clipboard::doCut (std::shared_ptr<UNDO::Transaction> transaction)
{
  if (m_currentContentWasCut)
  {
    m_currentContentWasCut = false;

    if (auto p = static_pointer_cast<Preset> (m_content))
    {
      auto uuid = p->getUuid ();

      if (auto src = Application::get ().getPresetManager ()->findPreset (uuid))
      {
        if (auto parentBank = src->getBank ())
        {
          parentBank->undoableDeletePreset (transaction, uuid);
        }
      }
    }
  }
}

UpdateDocumentContributor::tUpdateID Clipboard::onChange (uint64_t flags)
{
  auto ret = super::onChange(flags);
  m_sigChanged.send();
  return ret;
}

connection Clipboard::onClipboardChanged(slot<void> cb)
{
  return m_sigChanged.connectAndInit(cb);
}

