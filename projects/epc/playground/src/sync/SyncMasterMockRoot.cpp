#include "SyncMasterMockRoot.h"

SyncMasterMockRoot::SyncMasterMockRoot()
{
  enableSync(nltools::msg::API::Backend::Mock);
}

SyncMasterMockRoot& SyncMasterMockRoot::get()
{
  static SyncMasterMockRoot ret;
  return ret;
}

void SyncMasterMockRoot::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
