#include "TestGroupSet.h"

TestGroupSet::TestGroupSet(TestRootDocument *root)
    : ParameterGroupSet(root)
{
}

void TestGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
