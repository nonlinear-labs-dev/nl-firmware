#pragma once

#include <testing/TestRootDocument.h>
#include <presets/ParameterDualGroupSet.h>

class TestGroupSet : public ParameterDualGroupSet
{
public:
  explicit TestGroupSet(TestRootDocument *root);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};
