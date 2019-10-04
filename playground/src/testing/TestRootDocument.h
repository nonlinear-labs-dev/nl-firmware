#pragma once
#include "http/UpdateDocumentMaster.h"

class TestRootDocument : public UpdateDocumentMaster
{
 public:
  tUpdateID onChange(bool force);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};