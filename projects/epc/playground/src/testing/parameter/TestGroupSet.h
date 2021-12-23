#pragma once


#include <presets/ParameterGroupSet.h>

class TestGroupSet : public ParameterGroupSet
{
 public:
  explicit TestGroupSet(UpdateDocumentContributor *root);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};
