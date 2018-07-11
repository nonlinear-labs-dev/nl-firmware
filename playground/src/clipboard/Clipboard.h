#pragma once

#include <http/ContentSection.h>
#include <http/RPCActionManager.h>
#include <tools/Signal.h>
#include <libundo/undo/Scope.h>

class Clipboard : public ContentSection
{
    using super = ContentSection;

  public:
    Clipboard(UpdateDocumentContributor *parent);
    virtual ~Clipboard();

    void handleHTTPRequest(shared_ptr<NetworkRequest> request, const Glib::ustring &path) override;
    Glib::ustring getPrefix() const override;
    void writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const override;

    void cutPreset(const Glib::ustring &presetUuid);
    bool copyPreset(const Glib::ustring &presetUuid);
    void copyBank(const Glib::ustring &bankUuid);
    void copyPresets(const Glib::ustring &csv);

    void pasteOnBank(const Glib::ustring &bankUuid);
    void pasteOnPreset(const Glib::ustring &presetUuid);

    bool containsMultiplePresets() const;
    bool containsBank() const;
    bool containsPreset() const;
    bool hasContent() const;

    UpdateDocumentContributor::tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;

    connection onClipboardChanged(slot<void> cb);

  private:
    void pastePresetOnBank(const Glib::ustring &bankUuid);
    void pastePresetOnPreset(const Glib::ustring &presetUuid);
    void pastePresetOnBackground(const Glib::ustring &x, const Glib::ustring &y);

    void pasteBankOnBank(UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &bankUuid);
    void pasteBankOnPreset(UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &presetUuid);
    void pasteBankOnBackground(UNDO::Scope::tTransactionScopePtr scope, const Glib::ustring &x, const Glib::ustring &y);

    void pasteMultiplePresetsOnBackground(const Glib::ustring& x, const Glib::ustring &y);
    void pasteMultiplePresetsOnBank(const Glib::ustring &bankUuid);
    void pasteMultiplePresetsOnPreset(const Glib::ustring &presetUuid);

    void doCut(std::shared_ptr<UNDO::Transaction> transaction);

    RPCActionManager m_actions;
    shared_ptr<UpdateDocumentContributor> m_content;
    bool m_currentContentWasCut = false;

    Signal<void> m_sigChanged;
};

