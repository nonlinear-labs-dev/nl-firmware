#include "SyncMasterMockRoot.h"

SyncMasterMockRoot::SyncMasterMockRoot()
    : m_scope(this)
{
  enableSync(nltools::msg::API::Backend::Mock);
}

SyncMasterMockRoot& SyncMasterMockRoot::get()
{
  static SyncMasterMockRoot ret;
  return ret;
}

UNDO::Scope& SyncMasterMockRoot::getUndoScope()
{
  return m_scope;
}

void SyncMasterMockRoot::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
