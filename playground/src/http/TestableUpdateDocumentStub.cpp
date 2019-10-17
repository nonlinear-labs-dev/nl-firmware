#include "TestableUpdateDocumentStub.h"

void TestableUpdateDocumentStub::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}

UpdateDocumentContributor::tUpdateID TestableUpdateDocumentStub::onChange(uint64_t flags)
{
  incUpdateID();
  return getUpdateIDOfLastChange();
}

TestableUpdateDocumentStub::TestableUpdateDocumentStub()
    : UpdateDocumentContributor{ nullptr }
{
}
