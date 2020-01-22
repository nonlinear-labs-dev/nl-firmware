#include "TestGroupSet.h"

TestGroupSet::TestGroupSet(TestRootDocument *root)
    : ParameterDualGroupSet(root)
{
}

void TestGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
