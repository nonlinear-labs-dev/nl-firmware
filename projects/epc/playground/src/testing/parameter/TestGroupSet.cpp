#include "TestGroupSet.h"

TestGroupSet::TestGroupSet(UpdateDocumentContributor *root)
    : ParameterGroupSet(root)
{
}

void TestGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
