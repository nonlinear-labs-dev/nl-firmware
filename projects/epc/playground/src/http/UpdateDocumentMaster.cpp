#include "UpdateDocumentMaster.h"
#include "libundo/undo/ContinuousTransaction.h"

UpdateDocumentMaster::UpdateDocumentMaster()
    : UpdateDocumentContributor(nullptr)
{
}

UpdateDocumentMaster::~UpdateDocumentMaster()
= default;

UpdateDocumentMaster::tUpdateID UpdateDocumentMaster::onChange(uint64_t flags)
{
  incUpdateID();
  return getUpdateIDOfLastChange();
}
