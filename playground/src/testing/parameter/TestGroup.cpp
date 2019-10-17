#include "TestGroup.h"

TestGroup::TestGroup(TestGroupSet *root, VoiceGroup vg)
    : ParameterGroup(root, "test", "test", "test", "test", vg)
{
}

void TestGroup::init()
{
}

void TestGroup::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}

void TestGroup::addParameter(Parameter *p) {
  appendParameter(p);
}
