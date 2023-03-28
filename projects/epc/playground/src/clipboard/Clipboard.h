#pragma once

#include <http/ContentSection.h>
#include <http/RPCActionManager.h>
#include <tools/Signal.h>
#include "tools/Uuid.h"
#include <optional>

class Uuid;

namespace UNDO
{
  class Transaction;
}

class Clipboard : public ContentSection
{
  using super = ContentSection;

 public:
  explicit Clipboard(UpdateDocumentContributor *parent);
  virtual ~Clipboard();

  void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path) override;
  Glib::ustring getPrefix() const override;
  void writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const override;

  void cutPreset(const Uuid &presetUuid);
  bool copyPreset(const Uuid &presetUuid);
  void copyBank(const Uuid &bankUuid);
  void copyPresets(const Glib::ustring &csv);

  void pasteOnBank(const Uuid &bankUuid);
  void pasteOnPreset(const Uuid &presetUuid);

  bool containsMultiplePresets() const;
  bool containsBank() const;
  bool containsPreset() const;
  bool hasContent() const;
  bool hasCutPresetContent() const;

  void doCut(UNDO::Transaction *transaction);

  UpdateDocumentContributor::tUpdateID onChange(uint64_t flags
                                                = UpdateDocumentContributor::ChangeFlags::Generic) override;

  sigc::connection onClipboardChanged(sigc::slot<void> cb);

 private:
  void pastePresetOnBank(const Uuid &bankUuid);
  void pastePresetOnPreset(const Uuid &presetUuid);
  void pastePresetOnBackground(const Glib::ustring &x, const Glib::ustring &y);

  void pasteBankOnBank(const Glib::ustring &transactionName, const Uuid &bankUuid);
  void pasteBankOnPreset(const Glib::ustring &transactionName, const Uuid &presetUuid);
  void pasteBankOnBackground(const Glib::ustring &transactionName, const Glib::ustring &x, const Glib::ustring &y);
  void pasteBankOnBank(const Glib::ustring &transactionName, const Uuid &bankUuid,
                       const UpdateDocumentContributor *content);
  void pasteBankOnPreset(const Glib::ustring &transactionName, const Uuid &presetUuid,
                         const UpdateDocumentContributor *content);
  void pasteBankOnBackground(const Glib::ustring &transactionName, const Glib::ustring &x, const Glib::ustring &y,
                             const UpdateDocumentContributor *content);

  void pasteMultiplePresetsOnBackground(const Glib::ustring &x, const Glib::ustring &y);
  void pasteMultiplePresetsOnBank(const Uuid &bankUuid);
  void pasteMultiplePresetsOnPreset(const Uuid &presetUuid);

  RPCActionManager m_actions;
  std::unique_ptr<UpdateDocumentContributor> m_content;
  std::optional<Uuid> m_cutPresetUuid = std::nullopt;

  Signal<void> m_sigChanged;
};
