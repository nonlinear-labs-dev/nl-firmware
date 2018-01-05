#pragma once

#include "playground.h"
#include <memory>
#include "tools/IntrusiveList.h"

namespace UNDO
{
  class Scope;
  class Transaction;
}

class Writer;
class UpdateDocumentMaster;

class UpdateDocumentContributor : public IntrusiveListItem<UpdateDocumentContributor*>
{
  private:
    using super = IntrusiveListItem<UpdateDocumentContributor*>;

  public:
    typedef int tUpdateID;

    UpdateDocumentContributor(UpdateDocumentContributor *parent);
    virtual ~UpdateDocumentContributor();

    UpdateDocumentContributor(const UpdateDocumentContributor& other) = delete;
    UpdateDocumentContributor& operator=(const UpdateDocumentContributor&) = delete;

    virtual tUpdateID onChange();
    void onRestore();

    bool didChangeSince(tUpdateID clientsUpdateID) const;
    tUpdateID getUpdateIDOfLastChange() const;

    virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const = 0;

    const UpdateDocumentContributor *getParent() const;
    UpdateDocumentContributor *getParent();
    UpdateDocumentMaster *getRoot();
    const UpdateDocumentMaster *getRoot() const;

    virtual UNDO::Scope &getUndoScope();
    virtual const UNDO::Scope &getUndoScope() const;

    void propagateChangeDownstream();
    void orphan();

  protected:
    void setUpdateID(tUpdateID id);
    void incUpdateID();

    void adopt(UpdateDocumentContributor *parent);

  private:
    using super::getNext;
    using super::getPrev;

    UpdateDocumentContributor *m_parent = nullptr;
    IntrusiveList<UpdateDocumentContributor*> m_children;

    tUpdateID m_updateIDOnLastChange;
};

