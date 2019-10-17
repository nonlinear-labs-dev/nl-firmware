#include "TestRootDocument.h"

UpdateDocumentContributor::tUpdateID TestRootDocument::onChange(bool force)
{
  return 0;
}

void TestRootDocument::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
