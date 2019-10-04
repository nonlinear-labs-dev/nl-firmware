#pragma once
#include "UpdateDocumentContributor.h"

class TestableUpdateDocumentStub : public UpdateDocumentContributor {
public:
  TestableUpdateDocumentStub();
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  tUpdateID onChange(uint64_t flags) override;
};