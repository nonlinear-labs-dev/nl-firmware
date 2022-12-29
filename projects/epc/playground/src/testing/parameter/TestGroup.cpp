#include "TestGroup.h"
#include <parameters/Parameter.h>

TestGroup::TestGroup(TestGroupSet *root, VoiceGroup vg)
    : ParameterGroup(root, { "test", vg }, "test", "test", "test")
{
}

void TestGroup::init()
{
}

void TestGroup::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}

void TestGroup::addParameter(Parameter *p)
{
  appendParameter(p);
}
