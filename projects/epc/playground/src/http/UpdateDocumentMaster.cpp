#include "UpdateDocumentMaster.h"
#include "libundo/undo/ContinuousTransaction.h"
#include <sync/SyncMaster.h>

UpdateDocumentMaster::UpdateDocumentMaster()
    : UpdateDocumentContributor(nullptr)
{
}

UpdateDocumentMaster::~UpdateDocumentMaster() = default;

void UpdateDocumentMaster::enableSync(nltools::msg::API::Backend apiImpl)
{
  m_sync = std::make_unique<SyncMaster>(apiImpl);
}

SyncMaster &UpdateDocumentMaster::getSyncMaster() const
{
  return *m_sync;
}

UpdateDocumentMaster::tUpdateID UpdateDocumentMaster::onChange(uint64_t flags)
{
  m_flags |= flags;
  incUpdateID();
  return getUpdateIDOfLastChange();
}

uint64_t UpdateDocumentMaster::getAndResetFlags()
{
  return std::exchange(m_flags, 0);
}
