#include "UpdateDocumentMaster.h"
#include "libundo/undo/ContinuousTransaction.h"

UpdateDocumentMaster::UpdateDocumentMaster() :
  UpdateDocumentContributor (NULL)
{
}

UpdateDocumentMaster::~UpdateDocumentMaster()
{
}

UpdateDocumentMaster::tUpdateID UpdateDocumentMaster::onChange ()
{
  incUpdateID();
  return getUpdateIDOfLastChange();
}

